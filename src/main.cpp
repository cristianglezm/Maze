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

 std::vector<std::vector<Tile>> genGrid(std::vector<sf::Texture>& textures,unsigned int row,unsigned int col){
    auto m = std::vector<std::vector<Tile>>(col);
    sf::Vector2f b(0.0f,0.0f);
    auto type = Tile::Type::GRASS;
    sf::Sprite s;
    for(unsigned int i=0;i<col;++i){
        m[i] = std::vector<Tile>(row,Tile(b,type,s));
        b.x = 0.0f;
        for(unsigned int j=0;j<row;++j){
            s = sf::Sprite(textures[type]);
            s.setScale(0.5f,0.5f);
            m[i][j] = Tile(sf::Vector2f(b.y,b.x),type,s);
            b.x += m[i][j].getBounds().x;
        }
        b.y += m[i][row-1].getBounds().y;
    }
    return std::move(m);
}

void createNodesFromGrid(Graph* g,std::vector<std::vector<Tile>>& grid,bool diagonal = false){
    for(auto& gr:grid){
        for(auto& t:gr){
            g->addNode(Graph::Node(&t));
        }
    }
    int col = grid.size();
    int row = grid[0].size();
    auto size = row * col;
    for(int i=0;i<size;++i){
        // down
        if((i%row)!=(row-1)){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+1),0));
        }
        // right
        if((i+row)<size){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+row),0));
        }
        // diagonal
        if(diagonal){
            // right-down : left-top
            if((i+row+1)<size){
                g->addUndirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+row+1),0));
            }
            // left-down : right-top
            if((i+row-1)<size){
                g->addUndirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+row-1),0));
            }
        }
        // left
        if((i-row)>=0){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i-row),0));
        }
        // up
        if((i-1)>=0 && ((i-1)%row)!=(row-1)){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i-1),0));
        }
    }
}

int main(){
    bool Continue = true;
    std::vector<sf::Texture> texs;
        {
            sf::Texture t;
            t.loadFromFile("data/grass.jpg");
            texs.push_back(t);
            sf::Texture t2;
            t2.loadFromFile("data/water.jpg");
            texs.push_back(t2);
            sf::Texture t3;
            t3.loadFromFile("data/bot.png");
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
        auto m = genGrid(texs,row,col);
        Graph g;
        createNodesFromGrid(&g,m);

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
                                                    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                                                    start = std::chrono::high_resolution_clock::now();
                                                        auto path = g.bfs(g.getNode(origen),g.getNode(dest));
                                                    end = std::chrono::high_resolution_clock::now();
                                                    std::chrono::duration<long double> elapsed_seconds = end-start;
                                                    std::time_t end_time = std::chrono::high_resolution_clock::to_time_t(end);

                                                    std::cout << " BFS: -> finished computation at " << std::ctime(&end_time)
                                                              << "elapsed time: " << elapsed_seconds.count() << "s\n";
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
                                                    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                                                    start = std::chrono::high_resolution_clock::now();
                                                        auto path = g.gbfs(g.getNode(origen),g.getNode(dest));
                                                    end = std::chrono::high_resolution_clock::now();
                                                    std::chrono::duration<long double> elapsed_seconds = end-start;
                                                    std::time_t end_time = std::chrono::high_resolution_clock::to_time_t(end);

                                                    std::cout << " GBFS: -> finished computation at " << std::ctime(&end_time)
                                                              << "elapsed time: " << elapsed_seconds.count() << "s\n";
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
                                                    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                                                    start = std::chrono::high_resolution_clock::now();
                                                        auto path = g.dfs(g.getNode(origen),g.getNode(dest));
                                                    end = std::chrono::high_resolution_clock::now();
                                                    std::chrono::duration<long double> elapsed_seconds = end-start;
                                                    std::time_t end_time = std::chrono::high_resolution_clock::to_time_t(end);

                                                    std::cout << " DFS: -> finished computation at " << std::ctime(&end_time)
                                                              << "elapsed time: " << elapsed_seconds.count() << "s\n";
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
                        if(event.key.code == sf::Keyboard::M){
                            if(origen){
                                auto t1 = std::thread([&](){
                                                    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                                                    start = std::chrono::high_resolution_clock::now();
                                                        g.genMaze(g.getNode(origen),randomness,sf::Sprite(texs[1]),binomial);
                                                    end = std::chrono::high_resolution_clock::now();
                                                    std::chrono::duration<long double> elapsed_seconds = end-start;
                                                    std::time_t end_time = std::chrono::high_resolution_clock::to_time_t(end);

                                                    std::cout << "1 GenMaze: -> finished computation at " << std::ctime(&end_time)
                                                              << "elapsed time: " << elapsed_seconds.count() << "s\n";
                                                      });
                                                      t1.detach();
                            }
                            if(dest){
                                auto t2 = std::thread([&](){
                                                    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                                                    start = std::chrono::high_resolution_clock::now();
                                                        g.genMaze(g.getNode(dest),randomness,sf::Sprite(texs[1]),binomial);
                                                    end = std::chrono::high_resolution_clock::now();
                                                    std::chrono::duration<long double> elapsed_seconds = end-start;
                                                    std::time_t end_time = std::chrono::high_resolution_clock::to_time_t(end);

                                                    std::cout << "2 GenMaze: -> finished computation at " << std::ctime(&end_time)
                                                              << "elapsed time: " << elapsed_seconds.count() << "s\n";
                                                      });
                                                      t2.detach();
                            }
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
                            v.move(0,-100);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Up){
                            auto v = App.getView();
                            v.move(0,100);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Right){
                            auto v = App.getView();
                            v.move(-100,0);
                            App.setView(v);
                        }
                        if(event.key.code == sf::Keyboard::Left){
                            auto v = App.getView();
                            v.move(100,0);
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
            App.clear();
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
        std::string answer = "";
        std::cin >> answer;
        Continue = (answer == "y");
    }
    return 0;
}
