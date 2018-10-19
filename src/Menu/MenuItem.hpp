#ifndef MENU_ITEM_HPP
#define MENU_ITEM_HPP

#include "../Helpers/typedefs.hpp"

class MenuScreen;

typedef struct MenuItem {

    MenuItem();
    MenuItem(char *text, s32 value, MenuScreen::Id id);

    char *text[15];
    s32 value;
    MenuScreen::Id link;
} MenuItem;

#endif