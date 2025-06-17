#include "timer.hpp"

Timer::Timer(Point coordinate, int width, int height, int initial_seconds)
    : Drawable(coordinate, width, height),
      initial_duration_seconds(initial_seconds),
      current_remaining_seconds(initial_seconds) { // Inicializa com o tempo total
    start_time = std::chrono::high_resolution_clock::now();
    // Garante que a string formatada inicial seja gerada
    // Chamada a update() para inicializar o estado
    update(); 
}

// update() é o responsável por calcular e atualizar o tempo e a string.
// Este método não é const.
void Timer::update() {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed_duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();

    int new_remaining_seconds = initial_duration_seconds - static_cast<int>(elapsed_duration_seconds);
    if (new_remaining_seconds < 0) {
        new_remaining_seconds = 0;
    }

    // Só atualiza a string se o segundo realmente mudou para evitar processamento desnecessário
    if (new_remaining_seconds != current_remaining_seconds || current_formatted_time_str.empty()) {
        int minutes = new_remaining_seconds / 60;
        int seconds = new_remaining_seconds % 60;

        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << minutes << ":"
           << std::setw(2) << std::setfill('0') << seconds;
        
        current_formatted_time_str = ss.str();
    }
    
    // Atualiza o tempo restante armazenado
    current_remaining_seconds = new_remaining_seconds;
}

// Getters agora são const e apenas retornam os valores armazenados.
int Timer::getRemainingSeconds() const {
    return current_remaining_seconds;
}

std::string Timer::getFormattedTime() const {
    return current_formatted_time_str;
}

void Timer::draw(const IDrawer& drawer) const {
    drawer.drawTimer(*this);
}

bool Timer::isFinished() const {
    return current_remaining_seconds <= 0;
}

void Timer::reset() {
    current_remaining_seconds = initial_duration_seconds;
    start_time = std::chrono::high_resolution_clock::now();  
    update();  
}
