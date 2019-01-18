#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include <Arduino.h>
#include "../config.hpp"
#include "../Helpers/typedefs.hpp"
#include "../Events/Event.hpp"
#include "./MenuItem.hpp"

class MenuScreen {

    public:

        typedef enum Id {
            Main,
            Settings,
            Statistics,
            None
        } Id;

        MenuScreen(String title, uint8_t numOfItems, ... );
        ~MenuScreen();

        bool selectItem(uint8_t index);
        void handleEvents();

        void enterEditMode();
        
        MenuItem *getSelectedItem();
        void update(time_ms ms);
        void render();

        void onEnter();

    protected:

        String _title;

        MenuItem **_items;           ///< List of all MenuItems for this screen.
        uint8_t _num_of_items;       ///< Number of elements in _items array
        uint8_t _max_num_of_items;   ///< Maximum number of elements in _items array
        uint8_t _selected_item;      ///< Index of currently selected item in _items array
        bool _edit_mode;        ///< Is selected item being edited?
        bool _changed_since_last_render; ///< Was there any change since last rendering?

};

#endif
