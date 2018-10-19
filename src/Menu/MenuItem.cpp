#include "MenuItem.hpp"
#include "MenuScreen.hpp"

MenuItem::MenuItem(char *text, s32 value, MenuScreen::Id id) {
    this->text = text;
    this->value = value;
    this->link = id;
}