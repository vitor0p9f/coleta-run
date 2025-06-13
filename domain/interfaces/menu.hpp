#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>

class Menu {
private:
    std::vector<std::string> options;
    int selectedOption;

public:
    Menu(const std::vector<std::string>& options);

    void moveUp();
    void moveDown();
    int getSelectedOption() const;
    const std::vector<std::string>& getOptions() const;
};

#endif
