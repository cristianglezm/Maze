#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <future>
#include "Tile.hpp"
#include "Menu.hpp"
#include "Graph.hpp"
#include "Button.hpp"
#include <android/log.h>

using grid = std::vector<std::vector<Tile>>;

namespace android{
    void logInfo(std::string tag, std::string str);
}

Tile* getTile(grid& m,sf::Vector2f pos);
std::vector<Button> loadGUI(const sf::Texture& buttonTexture, const sf::Font& font);

int main(void){
    bool Continue = true;
    sf::VideoMode vm = sf::VideoMode(800, 600);
    sf::RenderWindow App(vm, "Maze",sf::Style::Default);
    while(Continue){
        sf::View view = App.getDefaultView();
        App.setView(view);
        unsigned int row = 100;
        unsigned int col = 100;
        float randomness = 0.5f;
        float binomial = 0.0f;
        bool isIsometric = false;
        {
            Menu modeSelection(App);
            while(!modeSelection.isReady()){
                modeSelection.handleInput();
                modeSelection.render();
            }
            isIsometric = modeSelection.isIsometric();
            Continue = modeSelection.wantToContinue();
            if(!Continue){
                break;
            }
        }
        // textures
        std::vector<sf::Texture> texs;
        if(isIsometric){
            sf::Texture t;
            t.loadFromFile("data/landscapeTiles_067.png");
            texs.emplace_back(t);
            sf::Texture t2;
            t2.loadFromFile("data/landscapeTiles_036.png");
            texs.emplace_back(t2);
        }else{
            sf::Texture t;
            t.loadFromFile("data/floor.jpg");
            texs.emplace_back(t);
            sf::Texture t2;
            t2.loadFromFile("data/Shrub.jpg");
            texs.emplace_back(t2);
        }
        //row = App.getSize().x / (texs[0].getSize().x / 2) * 5;
        //col = App.getSize().y / (texs[0].getSize().y / 2) * 5;
        auto m = createGrid(texs,row,col,isIsometric);
        Graph g;
        auto cgThread = std::thread([&](){
            createGraph(&g,m);
        });
        sf::Texture buttonTexture;
        buttonTexture.loadFromFile("data/button.png");
        sf::Font font;
        font.loadFromFile("data/Outwrite.ttf");
        auto buttons = loadGUI(buttonTexture,font);
        Tile* origen = nullptr;
        Tile* dest = nullptr;
        bool running = true;
        unsigned int currentFingerID = 0;
        int currentX = 0;
        int currentY = 0;
        float zoomLevel = 0;
        float oldDistance = 0;
        bool settingOrigen = false;
        bool settingDest = false;
        std::vector<sf::Event::TouchEvent> touchEvents;
        cgThread.join();
        /// set buttons actions
        buttons[0].setAction([&](){
            if(origen){
                origen->setColor(sf::Color::Cyan);
                std::thread([&](){
                    g.genMaze(g.getNode(origen),randomness,sf::Sprite(texs[1]),binomial);
                }).detach();
            }else if(dest){
                dest->setColor(sf::Color::Cyan);
                std::thread([&](){
                    g.genMaze(g.getNode(dest),randomness,sf::Sprite(texs[1]),binomial);
                }).detach();
            }
        });
        buttons[1].setAction([&](){
            settingOrigen = true;
        });
        buttons[2].setAction([&](){
            settingDest = true;
        });
        buttons[3].setAction([&](){
           if(origen && dest){
               std::thread([&](){
                    g.aStar(g.getNode(origen),g.getNode(dest)); 
               }).detach();
           }
        });
        buttons[4].setAction([&](){
            if(origen && dest){
               std::thread([&](){
                    g.bfs(g.getNode(origen),g.getNode(dest)); 
               }).detach();
           }
        });
        buttons[5].setAction([&](){
            if(origen && dest){
               std::thread([&](){
                    g.gbfs(g.getNode(origen),g.getNode(dest)); 
               }).detach();
           }
        });
        buttons[6].setAction([&](){
            if(origen && dest){
               std::thread([&](){
                    g.dfs(g.getNode(origen),g.getNode(dest)); 
               }).detach();
           }
        });
        while(running){
            sf::Event event;
            while(App.pollEvent(event)){
                switch(event.type){
                    case sf::Event::Closed:
                            running = false;
                        break;
                    case sf::Event::MouseEntered:
                    case sf::Event::KeyReleased:
                        if(event.key.code == sf::Keyboard::Escape){
                            running = false;
                        }
                        if(event.key.code == sf::Keyboard::Down){
                            auto v = App.getView();
                            v.move(0,100);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Up){
                            auto v = App.getView();
                            v.move(0,-100);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Right){
                            auto v = App.getView();
                            v.move(100,0);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Left){
                            auto v = App.getView();
                            v.move(-100,0);
                            App.setView(v);
                        }
                        break;
                    case sf::Event::LostFocus:
                    case sf::Event::MouseLeft:
                            running = false;
                        break;
                    case sf::Event::TouchBegan:
                            touchEvents.emplace_back(event.touch);
                        break;
                    case sf::Event::TouchMoved:{
                            if(touchEvents.size() >= 2){
                                touchEvents[event.touch.finger].x = event.touch.x;
                                touchEvents[event.touch.finger].y = event.touch.y;
                                auto diffX = touchEvents[0].x - touchEvents[1].x;
                                auto diffY = touchEvents[0].y - touchEvents[1].y;
                                auto newDistance = (diffX*diffX+diffY*diffY);
                                const auto zoomLimit = 15.0;
                                if((newDistance - oldDistance) > 10){
                                    view = App.getView();
                                    if(zoomLevel < zoomLimit){
                                        ++zoomLevel;
                                        view.zoom(0.9);
                                    }
                                    App.setView(view);
                                }else if((newDistance - oldDistance) < -10){
                                    view = App.getView();
                                    if(zoomLevel > -zoomLimit){
                                        --zoomLevel;
                                        view.zoom(1.1);
                                    }
                                    App.setView(view);
                                }
                                oldDistance = newDistance;
                            }else{
                                currentFingerID = event.touch.finger;
                                currentX = event.touch.x;
                                currentY = event.touch.y;
                                view = App.getView();
                                auto amountX = touchEvents[currentFingerID].x-currentX;
                                auto amountY = touchEvents[currentFingerID].y-currentY;
                                touchEvents[currentFingerID].x = currentX;
                                touchEvents[currentFingerID].y = currentY;
                                view.move(amountX,amountY);
                                App.setView(view);
                            }
                    }   break;
                    case sf::Event::TouchEnded:{
                            touchEvents.clear();
                            auto pos = static_cast<sf::Vector2f>(App.mapPixelToCoords(sf::Vector2i(event.touch.x,event.touch.y), App.getView()));
                            if(settingOrigen){
                                origen = getTile(m,pos);
                                if(origen){
                                    origen->setColor(sf::Color::Red);
                                }
                                settingOrigen = false;
                            }
                            if(settingDest){
                                dest = getTile(m,pos);
                                if(dest){
                                    dest->setColor(sf::Color::Blue);
                                }
                                settingDest = false;
                            }
                            view = App.getView();
                            App.setView(App.getDefaultView());
                            pos = static_cast<sf::Vector2f>(App.mapPixelToCoords(sf::Vector2i(event.touch.x,event.touch.y), App.getView()));
                            App.setView(view);
                            for(auto& b:buttons){
                                if(b.contains(pos)){
                                    b.getAction()();
                                }
                            }
                        }
                        break;
                    case sf::Event::SensorChanged:
                        break;
                    default:
                        break;
                }
            }
            App.clear(sf::Color::Black);
            for(auto i=0u;i<col;++i){
                for(auto j=0u;j<row;++j){
                    App.draw(m[i][j].getSprite());
                }
            }
            for(auto b:buttons){
                view = App.getView();
                App.setView(App.getDefaultView());
                App.draw(b);
                App.setView(view);
            }
            App.display();
        }
    }
    App.close();
    return 0;
}

std::vector<Button> loadGUI(const sf::Texture& buttonTexture, const sf::Font& font){
    std::vector<Button> buttons;
    sf::Sprite buttonSprite(buttonTexture);
    auto height = buttonSprite.getGlobalBounds().height * 3;
    sf::Text txtGenMaze("gm",font);
    Button btnGenMaze(sf::Vector2f(5.0,5.0),sf::Vector2f(4,3),buttonSprite,txtGenMaze,[](){});
    buttons.emplace_back(btnGenMaze);
    sf::Text txtSetOrigen("so",font);
    Button btnSetOrigen(sf::Vector2f(5.0,height + 15.0),sf::Vector2f(4,3),buttonSprite,txtSetOrigen, [](){});
    buttons.emplace_back(btnSetOrigen);
    sf::Text txtSetDest("sd",font);
    Button btnSetDest(sf::Vector2f(5.0,(height * 2) + 15.0),sf::Vector2f(4,3),buttonSprite,txtSetDest, [](){});
    buttons.emplace_back(btnSetDest);
    sf::Text txtAStar("as",font);
    Button btnAStar(sf::Vector2f(5.0,(height * 3) + 15.0),sf::Vector2f(4,3),buttonSprite,txtAStar, [](){});
    buttons.emplace_back(btnAStar);
    sf::Text txtBFS("bfs",font);
    Button btnBFS(sf::Vector2f(5.0,(height * 4) + 15.0),sf::Vector2f(4,3),buttonSprite,txtBFS, [](){});
    buttons.emplace_back(btnBFS);
    sf::Text txtGBFS("gbfs",font);
    Button btnGBFS(sf::Vector2f(5.0,(height * 5) + 15.0),sf::Vector2f(4,3),buttonSprite,txtGBFS, [](){});
    buttons.emplace_back(btnGBFS);
    sf::Text txtDFS("dfs",font);
    Button btnDFS(sf::Vector2f(5.0,(height * 6) + 15.0),sf::Vector2f(4,3),buttonSprite,txtDFS, [](){});
    buttons.emplace_back(btnDFS);
    return buttons;
}

Tile* getTile(grid& m,sf::Vector2f pos){
    Tile* res = nullptr;
    for(auto& vec: m){
        for(auto& t: vec){
            t.setColor(sf::Color::White);
            if(t.Contains(pos)){
                res = &t;
            }
        }
    }
    return res;
}

void android::logInfo(std::string tag, std::string str){
    __android_log_write(ANDROID_LOG_INFO, tag.c_str(), str.c_str());
}
