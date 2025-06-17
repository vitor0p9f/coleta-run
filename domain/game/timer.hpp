#pragma once

#include "../interfaces/drawable.hpp"
#include "types.hpp"
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>

class Timer : public Drawable {
public:
    Timer(Point coordinate, int width, int height, int initial_seconds);

    // Este método é responsável por calcular o tempo restante
    // e ATUALIZAR os membros internos do timer. Não é const.
    void update();

    // Retorna o tempo restante em segundos. É const porque apenas LÊ o estado atual.
    int getRemainingSeconds() const;

    // Retorna o tempo restante formatado como string "MM:SS". É const porque apenas LÊ o estado atual.
    std::string getFormattedTime() const;

    // Verifica se o tempo acabou. É const porque apenas LÊ o estado atual.
    bool isFinished() const;

    // Desenha o timer. É const porque apenas LÊ o estado para desenhar.
    void draw(const IDrawer& drawer) const override;

    void reset();

private:
    std::chrono::high_resolution_clock::time_point start_time;
    int initial_duration_seconds;

    // Estes membros armazenam o estado atual do timer,
    // e serão atualizados SOMENTE pelo método update().
    int current_remaining_seconds;
    std::string current_formatted_time_str;
};
