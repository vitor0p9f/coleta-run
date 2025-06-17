#include "core.hpp"
#include "map/map.hpp"
#include "timer.hpp"
#include "trash_bag.hpp"
#include "trash_can.hpp"
#include "types.hpp"
#include <cstdio>
#include <optional>
#include <random>
#include <chrono>
#include <vector>
#include "../domain/interfaces/windows_config.hpp"

extern Game game;
extern GameState gameState;

Game::Game(
  const IDrawer& drawer,
  IController& controller
) : drawer(drawer), controller(controller){}

GameState* game_state_ptr = nullptr;
GameResult game_result = GameResult::Ongoing;

void Game::setGameState(GameState* state) {
    game_state_ptr = state;
}

GameResult Game::getResult() const {
    return game_result;
}

void Game::spawnElement(Drawable& element, Map& map){
  static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  int map_width = map.getWidth();
  int map_height = map.getHeight();
  const WalkableMap& walkable_map = map.getWalkableMap();

  int element_width = element.getWidth();
  int element_height = element.getHeight();

  std::uniform_int_distribution<int> distribution_x(0, map_width - element_width);
  std::uniform_int_distribution<int> distribution_y(0, map_height - element_height);
  

  const int MAX_ATTEMPTS = 100;
  int attempts = 0;
  bool spawned = false;

  int map_max_x = map_width - 1;
  int map_max_y = map_height - 1;

  while (attempts < MAX_ATTEMPTS && !spawned) {
    int test_x = distribution_x(rng);
    int test_y = distribution_y(rng);

    bool area_is_walkable = true;

    int element_end_x = test_x + element_width;
    int element_end_y = test_y + element_height;

    for (int current_y = test_y; current_y < element_end_y; ++current_y) {
      for (int current_x = test_x; current_x < element_end_x; ++current_x) {
        if (current_x < 0 || current_x > map_max_x ||
            current_y < 0 || current_y > map_max_y ||
            !walkable_map[current_y][current_x])
        {
          area_is_walkable = false;
          break;
        }
      }
      if (!area_is_walkable) break;
    }

    if (area_is_walkable) {
      element.setCoordinate({test_x, test_y});
      spawned = true;
    }
    attempts++;
  }

  if (!spawned) {
    printf(
      "Warning: Could not find a walkable area for element after %d attempts.\n", MAX_ATTEMPTS
    );
  }
}

void Game::init(){
  controller.addElement(player_1);
  controller.addElement(player_2);

  for (TrashCan& trash_can : trash_cans) {
    spawnElement(trash_can, map);
  }

  spawnElement(player_1, map);
  spawnElement(player_2, map);

  for (int count = 0; count <= 4; count++) {
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    const int NUM_CATEGORIES = ORGANIC + 1;

    std::uniform_int_distribution<int> category_dist(0, NUM_CATEGORIES - 1);

    Category random_category = static_cast<Category>(category_dist(rng));

    TrashBag new_bag(Point{0,0}, TRASH_BAG_SIZE, TRASH_BAG_SIZE, random_category);

    spawnElement(new_bag, map);

    trash_bags.emplace_back(new_bag);
  }
}

void Game::draw(){
  map.draw(drawer);

  for (const TrashCan& trash_can : trash_cans) {
    trash_can.draw(drawer);
  }

  player_1.draw(drawer);
  player_2.draw(drawer);

  for (const TrashBag& trash_bag : trash_bags) {
    trash_bag.draw(drawer);
  }
}

void Game::spawnTrashBags(){
  if (trash_bags.size() >= max_trash_bags) {
    return;
  }

  auto current_time = std::chrono::high_resolution_clock::now();
  long long time_since_last_spawn_ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_spawn_time).count();

  if (time_since_last_spawn_ms < spawn_interval_ms) {
    return;
  }

  static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  const int NUM_CATEGORIES = ORGANIC + 1;

  std::uniform_int_distribution<int> category_dist(0, NUM_CATEGORIES - 1);

  Category random_category = static_cast<Category>(category_dist(rng));

  TrashBag new_bag(Point{0,0}, TRASH_BAG_SIZE, TRASH_BAG_SIZE, random_category);

  spawnElement(new_bag, map);

  trash_bags.emplace_back(new_bag);

  last_spawn_time = current_time;
}

bool Game::isColliding(const Drawable& obj1, const Drawable& obj2) {
  return obj1.getCoordinate().x < (obj2.getCoordinate().x + obj2.getWidth()) &&
         (obj1.getCoordinate().x + obj1.getWidth()) > obj2.getCoordinate().x &&
         obj1.getCoordinate().y < (obj2.getCoordinate().y + obj2.getHeight()) &&
         (obj1.getCoordinate().y + obj1.getHeight()) > obj2.getCoordinate().y;
}

void Game::update(){
  if (!game_state_ptr || *game_state_ptr != GameState::PLAYING) {
    return;  
    }
  game_timer.update();

   if (game_timer.isFinished()) {
        int player_1_points = player_1.getScore();
        int player_2_points = player_2.getScore();

        if (player_1_points > player_2_points){
            game_result = GameResult::Player1Win;
        } else if (player_2_points > player_1_points){
            game_result = GameResult::Player2Win;
        } else {
            game_result = GameResult::Tie;
        }

        *game_state_ptr = GameState::END;
        return; 
    }

  spawnTrashBags();
  handleCollisions();
}

void Game::handleCollisions() {
  Player* players[] = {&player_1, &player_2};

  for (Player* current_player : players) {
    static auto last_action_time = std::chrono::high_resolution_clock::now();
    const long long action_cooldown_ms = 200;

    bool is_on_cooldown = std::chrono::duration_cast<std::chrono::milliseconds>(
                                  std::chrono::high_resolution_clock::now() - last_action_time
                              ).count() < action_cooldown_ms;

    if (current_player->carried_bag.has_value() && !is_on_cooldown) {
      for (const TrashCan& current_can : trash_cans) {
        if (isColliding(*current_player, current_can)) {
          if (current_player->carried_bag->category == current_can.category) {
            current_player->detachBag();

            current_player->increaseScore(2);

            last_action_time = std::chrono::high_resolution_clock::now();
            break;
          }
        }
      }
    }

    if (!current_player->carried_bag.has_value() && !is_on_cooldown) {
      for (size_t i = 0; i < trash_bags.size(); ++i) {
        const TrashBag& current_bag = trash_bags[i];

        if (isColliding(*current_player, current_bag)) {
          current_player->attachBag(current_bag);

          trash_bags.erase(trash_bags.begin() + i);
          i--;
          last_action_time = std::chrono::high_resolution_clock::now();
          break;
        }
      }
    }
  }
}

void Game::resetGame() {
    // Reinicia os jogadores
    game.getPlayer1().setCoordinate(Point{100, 100});
    game.getPlayer1().score = 0;
    game.getPlayer1().detachBag();  
    
    game.getPlayer2().setCoordinate(Point{320, 520});
    game.getPlayer2().score = 0;
    game.getPlayer2().detachBag();  

    //game.getMap().reset(); 
    

    game.getTimer().reset();  
    
      gameState = GameState::PLAYING;
}
