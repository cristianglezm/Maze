#include "Button.hpp"

        Button::Button()
        :position(0.f,0.f)
        ,size(100.f,100.f){
            action = [](){};
        }
        Button::Button(sf::Vector2f position,sf::Vector2f size,sf::Text text, std::function<void()>&& action){
            this->position = position;
            this->size = size;
            this->action = action;
            this->text = text;
            this->text.setPosition(position);
            this->text.setFillColor(sf::Color::Black);
        }
        Button::Button(sf::Vector2f position,sf::Vector2f size,sf::Sprite image, std::function<void()>&& action){
            this->position = position;
            this->size = size;
            this->action = action;
            this->image = image;
            this->image.setPosition(position);
        }
        Button::Button(sf::Vector2f position,sf::Vector2f size,sf::Sprite image,sf::Text text, std::function<void()>&& action){
            this->position = position;
            this->size = size;
            this->action = action;
            this->image = image;
            this->text = text;
            this->image.setPosition(position);
            this->image.setScale(size);
            this->text.setPosition(position);
            this->text.setFillColor(sf::Color::Black);
        }
        void Button::setAction(std::function<void()>&& action){
            this->action = action;
        }
        void Button::setPosition(sf::Vector2f pos){
            position = pos;
            this->image.setPosition(position);
            this->image.setScale(size);
            this->text.setPosition(position);
            this->text.setScale(size);
        }
        void Button::setSize(sf::Vector2f size){
            this->size = size;
        }
        void Button::setImage(sf::Sprite sprite){
            image = sprite;
            this->image.setPosition(position);
            this->image.setScale(size);
        }
        void Button::setText(sf::Text text){
            this->text = text;
            this->text.setPosition(position);
            this->text.setScale(size);
        }
        bool Button::contains(sf::Vector2f point){
            auto v = image.getGlobalBounds();
            return v.contains(point);
        }
        void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const{
            target.draw(image);
            target.draw(text);
        }