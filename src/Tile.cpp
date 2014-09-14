#include "Tile.hpp"

    Tile::Tile(const sf::Vector2f& pos,const Type& t,const sf::Sprite& s){
        sprite = s;
        position = pos;
        type = t;
        sprite.setPosition(pos);
        bounds.y = sprite.getGlobalBounds().height;
        bounds.x = sprite.getGlobalBounds().width;
    }
    Tile& Tile::setType(const Type& t){
        type = t;
        return *this;
    }
    Tile& Tile::setSprite(const sf::Sprite& s){
        auto scale = sprite.getScale();
        sprite = s;
        sprite.setScale(scale);
        sprite.setPosition(position);
        bounds.y = sprite.getGlobalBounds().height;
        bounds.x = sprite.getGlobalBounds().width;
        return *this;
    }
    Tile& Tile::setPosition(const sf::Vector2f& pos){
        position = pos;
        sprite.setPosition(pos);
        return *this;
    }
    Tile& Tile::translate(const sf::Vector2f& amount){
        position +=amount;
        sprite.setPosition(position);
        return *this;
    }
    Tile& Tile::setColor(const sf::Color& c){
        sprite.setColor(c);
        return *this;
    }
    bool Tile::Contains(const sf::Vector2f& pos){
        sf::FloatRect b(position,bounds);
        return b.contains(pos);

    }
    Tile& Tile::setBounds(const sf::Vector2f& b){
        bounds = b;
        return *this;
    }
