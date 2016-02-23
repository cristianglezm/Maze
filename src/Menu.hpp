#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

class Menu final{
    public:
        explicit Menu(sf::RenderWindow& win);
        void handleInput();
        void render();
        inline bool isIsometric() const noexcept{
            return isometric;
        }
        inline bool isReady() const noexcept{
            return ready;
        }
        inline bool wantToContinue() const noexcept{
            return Continue;
        }
        ~Menu() = default;
    private:
        sf::RenderWindow& window;
        sf::Texture iso;
        sf::Texture top;
        sf::Sprite Isometric;
        sf::Sprite Topdown;
        bool isometric;
        bool ready;
        bool Continue;
};

#endif // MENU_HPP
