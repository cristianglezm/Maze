#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <random>
#include <future>
#include <stack>
#include "Tile.hpp"
#include "Graph.hpp"

using grid = std::vector<std::vector<Tile>>;

void benchmark(const unsigned& testCount);

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
int main(){
    bool Continue = true;
    while(Continue){
        unsigned int row = 5, col = 5;
        float randomness = 0.9f,binomial = 0.85f;
        std::string isIso;
        std::cout << "Introduce the number of Rows: " << std::endl;
        std::cin >> row;
        std::cout << "Introduce the number of columns: " << std::endl;
        std::cin >> col;
        std::cout << "Randomness: " << std::endl;
        std::cin >> randomness;
        std::cout << "Binomial: " << std::endl;
        std::cin >> binomial;
        std::cout << "Isometric / topDown? (iso|top)" << std::endl;
        std::cin >> isIso;
        // textures
        std::vector<sf::Texture> texs;
        bool isIsometric = isIso == "iso";
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
        Continue = false;
        auto m = createGrid(texs,row,col,isIsometric);
        Graph g;
        createGraph(&g,m);
        sf::VideoMode vm = sf::VideoMode(800, 600);
        sf::RenderWindow App(vm, "Maze",sf::Style::Default);
        Tile* origen = nullptr;
        Tile* dest = nullptr;
        // result is for the program to wait until the threads are finished.
        std::future<void> result[6];
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
                                result[0] = std::async(std::launch::async,[&](){
                                           sf::Clock c;
                                           auto path = g.bfs(g.getNode(origen),g.getNode(dest));
                                           std::cout << " BFS: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                           auto pathSize = path.size();
                                           std::cout << "Path Size: " << pathSize << std::endl;
                                });
                            }
                        }
                        if(event.key.code == sf::Keyboard::A){
                            if(origen && dest){
                                result[1] = std::async(std::launch::async,[&](){
                                           sf::Clock c;
                                           auto path = g.aStar(g.getNode(origen),g.getNode(dest));
                                           std::cout << " A*: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                           auto pathSize = path.size();
                                           std::cout << "Path Size: " << pathSize << std::endl;
                                });
                            }
                        }
                        if(event.key.code == sf::Keyboard::G){
                            if(origen && dest){
                                result[2] = std::async(std::launch::async,[&](){
                                            sf::Clock c;
                                            auto path = g.gbfs(g.getNode(origen),g.getNode(dest));
                                            std::cout << " GBFS: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                            auto pathSize = path.size();
                                            std::cout << "Path Size: " << pathSize << std::endl;
                                });
                            }
                        }
                        if(event.key.code == sf::Keyboard::D){
                            if(origen && dest){
                                result[3] = std::async(std::launch::async,[&](){
                                            sf::Clock c;
                                            auto path = g.dfs(g.getNode(origen),g.getNode(dest));
                                            std::cout << " DFS: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                            auto pathSize = path.size();
                                            std::cout << "Path Size: " << pathSize << std::endl;
                                });
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
                                result[4] = std::async(std::launch::async,[&](){
                                            sf::Clock c;
                                            g.genMaze(g.getNode(origen),randomness,sf::Sprite(texs[1]),binomial);
                                            std::cout << "Origen GenMaze: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                });
                            }
                            if(dest){
                                result[5] = std::async(std::launch::async,[&](){
                                            sf::Clock c;
                                            g.genMaze(g.getNode(dest),randomness,sf::Sprite(texs[1]),binomial);
                                            std::cout << "Dest GenMaze: -> elapsed time: " << c.restart().asSeconds() << "s" << std::endl;
                                });
                            }
                        }
                        if(event.key.code == sf::Keyboard::S){
                            sf::Vector2u windowSize = App.getSize();
                            sf::Texture texture;
                            texture.create(windowSize.x, windowSize.y);
                            texture.update(App);
                            sf::Image capture = texture.copyToImage();
                            static int counter = 0;
                            capture.saveToFile("screenshot" + std::to_string(counter) + ".png");
                            ++counter;
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
                                origen = getTile(m,d);
                                if(origen){
                                    std::cout << "origen: "<< g.getNode(origen).index <<std::endl;
                                    std::cout << "Position: -> [" << origen->getPosition().x << ","<< origen->getPosition().y << "]" << std::endl;
                                    origen->setColor(sf::Color::Cyan);
                                }
                                if(dest){
                                    dest->setColor(sf::Color::Red);
                                }
                            }
                            if(event.mouseButton.button == sf::Mouse::Right){
                                sf::Vector2f d = static_cast<sf::Vector2f>(App.mapPixelToCoords(sf::Mouse::getPosition(App),App.getView()));
                                dest = getTile(m,d);
                                if(dest){
                                        std::cout << "dest: " << g.getNode(dest).index <<std::endl;
                                        std::cout << "Position: -> [" << dest->getPosition().x << ","<< dest->getPosition().y << "]" << std::endl;
                                        dest->setColor(sf::Color::Red);
                                }
                                if(origen)
                                    origen->setColor(sf::Color::Cyan);
                            }
                        break;
                }
            }
            App.clear(sf::Color::Black);
            for(auto i=0u;i<col;++i){
                for(auto j=0u;j<row;++j){
                    App.draw(m[i][j].getSprite());
                }
            }
            App.display();
        }
        App.close();
        std::cout << "Do you want to replay?(y/n)" << std::endl;
        std::string answer;
        std::cin >> answer;
        std::cout << "Processing Pending tasks...." << std::endl;
        Continue = (answer == "y");
    }
    std::cout << "Run Benchmark?" << std::endl;
    std::string answer;
    std::cin >> answer;
    if(answer == "y"){
        benchmark(150);
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
            texs.emplace_back(t);
            sf::Texture t2;
            t2.loadFromFile("data/Shrub.jpg");
            texs.emplace_back(t2);
        }
    for(auto i=1u;i<=testCount;i+=100){
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
