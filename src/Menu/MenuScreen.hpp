#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "../config.hpp"
#include "../Helpers/typedefs.hpp"
#include "../Events/Event.hpp"
#include "MenuItem.hpp"
#include "../App.hpp"

class MenuScreen {

    public:

        typedef enum Id {
            Main,
            Settings,
            Statistics,
            None
        } Id;

        MenuScreen(const char *title, MenuItem *items[] );
        void setup(App *app);

        ~MenuScreen();

        bool selectItem(u8 index);
        void handleEvents();
        
        MenuItem *getSelectedItem();
        void update(time_ms ms);
        void render();

    protected:

        App *_app;

        char _title[MENU_HEADER_LENGTH];

        MenuItem **_items;           ///< List of all MenuItems for this screen.
        u8 _num_of_items;       ///< Number of elements in _items array
        u8 _max_num_of_items;   ///< Maximum number of elements in _items array
        u8 _selected_item;      ///< Index of currently selected item in _items array
        bool _edit_mode;        ///< Is selected item being edited?

};

#endif