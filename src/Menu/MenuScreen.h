#ifndef GARDUINO_MENU_SCREEN_H
#define GARDUINO_MENU_SCREEN_H

#include <Arduino.h>
#include "../config.h"
#include "./MenuItem.h"

class MenuScreen {

    public:

        MenuScreen(String title, uint8_t numOfItems, ... );
        ~MenuScreen();

        bool selectItem(uint8_t index);
        void handleEvents();

        void enterEditMode();
        
        MenuItem *getSelectedItem();
        void update(bool renderOverride = false);

    protected:

        void render(bool renderOverride = false);

        String _title;

        uint8_t _multiplier;        ///< Value multiplier in edit mode
        MenuItem **_items;           ///< List of all MenuItems for this screen.
        uint8_t _num_of_items;      ///< Number of elements in _items array
        uint8_t _max_num_of_items;  ///< Maximum number of elements in _items array
        uint8_t _selected_item;     ///< Index of currently selected item in _items array
        bool _changed_since_last_render;
        bool _edit_mode;            ///< Is selected item being edited?

};

#endif
