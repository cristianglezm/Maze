#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>

/**
 * @class Button
 * @author Cristian
 * @brief class from AntFarm
 */
class Button : public sf::Drawable{
    public:
        Button();
        Button(sf::Vector2f position,sf::Vector2f size,sf::Text text, std::function<void()>&& action);
        Button(sf::Vector2f position,sf::Vector2f size,sf::Sprite image, std::function<void()>&& action);
        Button(sf::Vector2f position,sf::Vector2f size,sf::Sprite image,sf::Text text, std::function<void()>&& action);
        void setAction(std::function<void()>&& action);
        inline std::function<void()> getAction(){ return action; }
        void setPosition(sf::Vector2f pos);
        inline sf::Vector2f getPosition(){ return position; }
        void setSize(sf::Vector2f size);
        inline sf::Vector2f getSize(){ return size; }
        void setImage(sf::Sprite sprite);
        inline sf::Sprite getSprite(){ return image;}
        void setText(sf::Text text);
        inline sf::Text getText(){ return text; }
        bool contains(sf::Vector2f point);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual ~Button() = default;
    private:
        std::function<void()> action;
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Sprite image;
        sf::Text text;
};
#endif // BUTTON_HPP
