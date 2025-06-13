#include "../interfaces/menu.hpp"

Menu::Menu(const std::vector<std::string>& options)
    : options(options), selectedOption(0) {}

void Menu::moveUp() {
    if (selectedOption > 0) selectedOption--;
}

void Menu::moveDown() {
    if (selectedOption < static_cast<int>(options.size()) - 1) selectedOption++;
}

int Menu::getSelectedOption() const {
    return selectedOption;
}

const std::vector<std::string>& Menu::getOptions() const {
    return options;
}
