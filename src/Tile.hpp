#ifndef TILE_HPP
#define TILE_HPP
#include <SFML/Graphics/Sprite.hpp>

/**
 * @brief Simple tile
 * @author Cristian Glez <cristian.glez.m@gmail.com>
 */
class Tile{
    public:
        /**
         * @brief Type of the tile
         */
        enum Type{GRASS,WATER,BOT};
        /**
         * @brief Basic constructor.
         */
        explicit Tile(const sf::Vector2f& pos,const Type& t,const sf::Sprite& s,bool isIsometric = false);
        /**
         * @brief Setter for the type Of the Tile
         * @param t Tile::Type
         * @return Tile &
         */
        Tile& setType(const Type& t);
        /**
         * @brief Getter of Type
         * @return Type  & const
         */
        inline const Type& getType(){ return type; }
        /**
         * @brief Setter for the Sprite
         * @param s sf::Sprite
         * @return Tile &
         */
        Tile& setSprite(const sf::Sprite& s);
        /**
         * @brief Getter for Sprite
         * @return sf::Sprite &
         */
        inline sf::Sprite& getSprite(){ return sprite; }
        /**
         * @brief Setter for the position.
         * @param pos sf::Vector2f
         * @return Tile&
         */
        Tile& setPosition(const sf::Vector2f& pos);
        /**
         * @brief 
         * @param amount
         * @return 
         */
        Tile& translate(const sf::Vector2f& amount);
        /**
         * @brief 
         * @return 
         */
        inline const sf::Vector2f& getPosition(){ return position; }
        /**
         * @brief 
         * @param c
         * @return 
         */
        Tile& setColor(const sf::Color& c);
        /**
         * @brief 
         * @param pos
         * @return 
         */
        bool Contains(const sf::Vector2f& pos);
        /**
         * @brief 
         * @param b
         * @return 
         */
        Tile& setBounds(const sf::Vector2f& b);
        inline const sf::Vector2f& getBounds(){ return bounds; }
        Tile& setScale(const sf::Vector2f& s);
        inline const sf::Vector2f& getScale(){ return sprite.getScale(); }
        inline const bool isIsometric(){ return isometric; }
    private:
        sf::Sprite sprite;
        sf::Vector2f position;
        sf::Vector2f bounds;
        Type type;
        bool isometric;
};

sf::Vector2f Cart2DToIso(sf::Vector2f pos);

#endif // TILE_HPP
