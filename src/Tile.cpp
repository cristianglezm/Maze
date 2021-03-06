#include "Tile.hpp"

    sf::Vector2f Cart2DToIso(sf::Vector2f pos){
        sf::Vector2f tmp(pos.x - pos.y,(pos.x + pos.y) / 2);
        return tmp;
    }
    Tile::Tile(const sf::Vector2f& pos,const Type& t,const sf::Sprite& s,bool isIsometric){
        sprite = s;
        type = t;
        isometric = isIsometric;
        if(isIsometric){
            position = Cart2DToIso(pos);
        }else{
            position = pos;
        }
        sprite.setPosition(position);
        bounds.y = sprite.getGlobalBounds().height;
        bounds.x = sprite.getGlobalBounds().width;
    }
    Tile& Tile::setType(const Type& t){
        type = t;
        return *this;
    }
    Tile& Tile::setSprite(const sf::Sprite& s){
        auto scale = sprite.getScale();
        auto oldbounds = sprite.getGlobalBounds();
        sprite = s;
        sprite.setScale(scale);
        bounds.y = sprite.getGlobalBounds().height;
        bounds.x = sprite.getGlobalBounds().width;
        if(isometric){
            position.x += oldbounds.width - bounds.x;
            position.y += oldbounds.height - bounds.y;
        }
        sprite.setPosition(position);
        return *this;
    }
    Tile& Tile::setPosition(const sf::Vector2f& pos){
        if(isometric){
            position = Cart2DToIso(pos);
        }else{
            position = pos;
        }
        sprite.setPosition(position);
        return *this;
    }
    Tile& Tile::translate(const sf::Vector2f& amount){
        if(isometric){
            position += Cart2DToIso(amount);
        }else{
            position += amount;
        }
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
    Tile& Tile::setScale(const sf::Vector2f& s){
        sprite.setScale(s);
        return *this;
    }
