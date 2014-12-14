#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <random>
#include <thread>
#include <stack>
#include "Tile.hpp"
#include "Graph.hpp"

void benchmark(const unsigned& testCount);

int main(){
    bool Continue = true;
    std::vector<sf::Texture> texs;
        {
            sf::Texture t;
            t.loadFromFile("data/landscapeTiles_067.png");
            texs.push_back(t);
            sf::Texture t2;
            t2.loadFromFile("data/landscapeTiles_036.png");
            texs.push_back(t2);
            sf::Texture t3;
            t3.loadFromFile("data/Ant.png");
            texs.push_back(t3);
        }
    while(Continue){
        unsigned int row = 5, col = 5;
        float randomness = 0.9f,binomial = 0.85f;
        std::cout << "Introduce the number of Rows: " << std::endl;
        std::cin >> row;
        std::cout << "Introduce the number of columns: " << std::endl;
        std::cin >> col;
        std::cout << "Randomness: " << std::endl;
        std::cin >> randomness;
        std::cout << "Binomial: " << std::endl;
        std::cin >> binomial;
        Continue = false;
        auto m = createGrid(texs,row,col);
        Graph g;
        createGraph(&g,m);

        sf::RenderWindow App(sf::VideoMode(1920, 1080), "Maze",sf::Style::Default);
        Tile* origen = nullptr;
        Tile* dest = nullptr;
        auto spr = sf::Sprite(texs[2]);
        spr.setScale(0.5f,0.5f);
        auto bot = Tile(sf::Vector2f(0,0),Tile::BOT,spr);
        bool running = true;
        while(running){
            sf::Event event;
            while(App.pollEvent(event)){
                switch(event.type){
                    case sf::Event::Closed:
                            running = false;
                        break;
                    case sf::Event::KeyReleased:
                        if(event.key.code == sf::Keyboard::Escape){
                            running = false;
                        }
                        if(event.key.code == sf::Keyboard::B){
                            if(origen && dest){
                                auto t1 = std::thread([&](){
                                                    bot.setPosition(origen->getPosition());
                                                    sf::Clock c;
                                                        auto path = g.bfs(g.getNode(origen),g.getNode(dest));
                                                    std::cout << " BFS: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                                    auto pathSize = path.size();
                                                    std::cout << "Path Size: " << pathSize << std::endl;
                                                    for(unsigned int i = 0;i<pathSize;){
                                                        std::this_thread::sleep_for(std::chrono::milliseconds(90));
                                                        bot.setPosition(path[i]->tile->getPosition());
                                                        ++i;
                                                    }
                                                      });
                                                      t1.detach();
                            }
                        }
                        if(event.key.code == sf::Keyboard::G){
                            if(origen && dest){
                                auto t1 = std::thread([&](){
                                                    bot.setPosition(origen->getPosition());
                                                    sf::Clock c;
                                                        auto path = g.gbfs(g.getNode(origen),g.getNode(dest));
                                                    std::cout << " GBFS: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                                    auto pathSize = path.size();
                                                    std::cout << "Path Size: " << pathSize << std::endl;
                                                    for(unsigned int i = 0;i<pathSize;){
                                                        std::this_thread::sleep_for(std::chrono::milliseconds(90));
                                                        bot.setPosition(path[i]->tile->getPosition());
                                                        ++i;
                                                    }
                                                      });
                                                      t1.detach();
                            }
                        }
                        if(event.key.code == sf::Keyboard::D){
                            if(origen && dest){
                                auto t1 = std::thread([&](){
                                                    bot.setPosition(origen->getPosition());
                                                    sf::Clock c;
                                                        auto path = g.dfs(g.getNode(origen),g.getNode(dest));
                                                    std::cout << " DFS: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                                    auto pathSize = path.size();
                                                    std::cout << "Path Size: " << pathSize << std::endl;
                                                    for(unsigned int i = 0;i<pathSize;){
                                                        std::this_thread::sleep_for(std::chrono::milliseconds(90));
                                                        bot.setPosition(path[i]->tile->getPosition());
                                                        ++i;
                                                    }
                                                      });
                                                      t1.detach();
                            }
                        }
                        if(event.key.code == sf::Keyboard::R){
                            if(origen){
                                origen->setSprite(sf::Sprite(texs[1]));
                                g.removeNode(g.getNode(origen));
                            }
                        }
                        if(event.key.code == sf::Keyboard::C){
                            if(origen && dest){
                                if(g.areConnected(g.getNode(origen),g.getNode(dest))){
                                    std::cout << "Origen And Destination are connected"<< std::endl;
                                }else{
                                    std::cout << "Origen And Destination are not connected"<< std::endl;
                                }
                            }
                        }
                        if(event.key.code == sf::Keyboard::M){
                            if(origen){
                                auto t1 = std::thread([&](){
                                                    sf::Clock c;
                                                        g.genMaze(g.getNode(origen),randomness,sf::Sprite(texs[1]),binomial);
                                                    std::cout << "Origen GenMaze: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                                      });
                                                      t1.detach();
                            }
                            if(dest){
                                auto t2 = std::thread([&](){
                                                    sf::Clock c;
                                                        g.genMaze(g.getNode(dest),randomness,sf::Sprite(texs[1]),binomial);
                                                    std::cout << "Dest GenMaze: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                                      });
                                                      t2.detach();
                            }
                        }
                        if(event.key.code == sf::Keyboard::S){
                            sf::Image capture = App.capture();
                            capture.saveToFile("ScreenShot.png");
                        }
                        if(event.key.code == sf::Keyboard::Add){
                            sf::View v = App.getView();
                            v.zoom(0.5);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Subtract){
                            sf::View v = App.getView();
                            v.zoom(1.5);
                            App.setView(v);
                        }
                        break;
                    case sf::Event::KeyPressed:
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
                    case sf::Event::MouseButtonReleased:
                            if(event.mouseButton.button == sf::Mouse::Left){
                                sf::Vector2f d = static_cast<sf::Vector2f>(App.mapPixelToCoords(sf::Mouse::getPosition(App),App.getView()));
                                for(auto& vec: m){
                                    for(auto& t: vec){
                                        t.setColor(sf::Color::White);
                                        if(t.Contains(d)){
                                            origen = &t;
                                            std::cout << "origen: "<< g.getNode(origen).index <<std::endl;
                                            std::cout << "Position: -> [" << t.getPosition().x << ","<< t.getPosition().y << "]" << std::endl;
                                            bot.setPosition(t.getPosition());
                                            t.setColor(sf::Color::Cyan);
                                            if(dest){
                                                dest->setColor(sf::Color::Red);
                                            }
                                        }
                                    }
                                }
                            }
                            if(event.mouseButton.button == sf::Mouse::Right){
                                sf::Vector2f d = static_cast<sf::Vector2f>(App.mapPixelToCoords(sf::Mouse::getPosition(App),App.getView()));
                                for(auto& vec: m){
                                    for(auto& t: vec){
                                        t.setColor(sf::Color::White);
                                        if(t.Contains(d)){
                                            dest = &t;
                                            if(origen){origen->setColor(sf::Color::Cyan);}
                                            std::cout << "dest: " << g.getNode(dest).index <<std::endl;
                                            std::cout << "Position: -> [" << t.getPosition().x << ","<< t.getPosition().y << "]" << std::endl;
                                            t.setColor(sf::Color::Red);
                                        }
                                    }
                                }
                            }
                        break;
                }
            }
            App.clear(sf::Color::Black);
            for(unsigned int i=0;i<col;++i){
                for(unsigned int j=0;j<row;++j){
                    App.draw(m[i][j].getSprite());
                }
            }
            App.draw(bot.getSprite());
            App.display();
        }
        App.close();
        std::cout << "Do you want to replay?(y/n)" << std::endl;
        std::string answer;
        std::cin >> answer;
        Continue = (answer == "y");
    }
    std::cout << "Run Benchmark?" << std::endl;
    std::string answer;
    std::cin >> answer;
    if(answer == "y"){
        benchmark(501);
    }
    return 0;
}

void benchmark(const unsigned& testCount){
    auto total = 0.0f;
    std::cout << "Running Benchmark genMaze..." << std::endl;
    std::vector<sf::Texture> texs;
        {
            sf::Texture t;
            t.loadFromFile("data/floor.jpg");
            texs.push_back(t);
            sf::Texture t2;
            t2.loadFromFile("data/Shrub.jpg");
            texs.push_back(t2);
            sf::Texture t3;
            t3.loadFromFile("data/Ant.png");
            texs.push_back(t3);
        }
    for(int i=1;i<=testCount;i+=100){
        auto testGrid = createGrid(texs,i,i);
        Graph g;
        createGraph(&g,testGrid,false);
        std::cout << "Processing grid: " << i<<"x"<< i;
        sf::Clock c;
        g.genMaze(g.getNode(&testGrid[0][0]),0.5f,{},0.2f);
        auto timeTaken = c.restart().asSeconds();
        total += timeTaken;
        std::cout<< " Completed In: " << timeTaken << " seconds" << std::endl;
    }
    std::cout << "Total: " << total << " seconds" << std::endl;
    std::cout << "avg mean: " << (total / testCount) << " seconds" << std::endl;
}
