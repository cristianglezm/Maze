#include "Menu.hpp"

Menu::Menu(sf::RenderWindow& win)
: window(win)
, iso()
, top()
, Isometric()
, Topdown()
, isometric(false)
, ready(false)
, Continue(true){
    iso.loadFromFile("data/Isometric.png");
    Isometric.setTexture(iso);
    Isometric.setScale(2.0,2.0);
    auto size = window.getSize();
    auto isoPos = sf::Vector2f(size.x/2,size.y/2);
    auto isoSize = Isometric.getLocalBounds();
    Isometric.setPosition(isoPos);
    Isometric.setOrigin(isoSize.width/2, isoSize.height/2);
    top.loadFromFile("data/Topdown.png");
    Topdown.setTexture(top);
    Topdown.setScale(2.0,2.0);
    auto topPos = sf::Vector2f(size.x/2,(size.y/2) + isoSize.height*2 + 15);
    auto topSize = Topdown.getLocalBounds();
    Topdown.setPosition(topPos);
    Topdown.setOrigin(topSize.width/2,topSize.height / 2);
}
void Menu::handleInput(){
    sf::Event event;
    while(window.pollEvent(event)){
        switch(event.type){
            case sf::Event::Closed:
                    Continue = false;
                    ready = true;
                break;
            case sf::Event::KeyReleased:
                    if(event.key.code == sf::Keyboard::Escape){
                        ready = true;
                        Continue = false;
                    }
                break;
            case sf::Event::TouchEnded:
                    auto x = event.touch.x;
                    auto y = event.touch.y;
                    sf::Vector2f clickPos = static_cast<sf::Vector2f>(window.mapPixelToCoords(sf::Vector2i(x,y),window.getView()));
                    auto rIso = Isometric.getGlobalBounds();
                    auto rTop = Topdown.getGlobalBounds();
                    if(rIso.contains(clickPos)){
                            ready = true;
                            isometric = true;
                    }else if(rTop.contains(clickPos)){
                            ready = true;
                            isometric = false;
                    }
                break;
        }
    }
}
void Menu::render(){
    window.clear();
    window.draw(Isometric);
    window.draw(Topdown);
    window.display();
}


