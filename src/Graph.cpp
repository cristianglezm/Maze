#include "Graph.hpp"
#include <iostream>
#include "parallel.hpp"
#include <thread>

    Graph::Node::Node(Tile* t)
    : tile(t)
    , visited(false)
    , index(0)
    , parent(-1){}
    bool Graph::Node::operator==(const Node& rhs) const{
        return tile == rhs.tile;
    }
    bool Graph::Node::operator!=(const Node& rhs) const{
        return tile != rhs.tile;
    }
    Graph::Edge::Edge()
    : weight(0)
    , prev(0)
    , next(0){}
    Graph::Edge::Edge(const std::size_t& p,const std::size_t& n,const int& w)
    : weight(w)
    , prev(p)
    , next(n){}
    bool Graph::Edge::operator==(const Edge& rhs) const{
        return (weight == rhs.weight
                && prev == rhs.prev
                && next == rhs.next);
    }
    bool Graph::Edge::operator!=(const Edge& rhs) const{
        return (weight != rhs.weight
                || prev != rhs.prev
                || next != rhs.next);
    }
    bool Graph::Edge::operator<=(const Edge& rhs) const{
        return (weight<=rhs.weight);
    }
    Graph::Graph()
    : nodes()
    , edges(){}
    void Graph::addNode(Node n){
        n.index = nodes.size();
        nodes.emplace_back(n);
        edges.emplace_back(std::vector<Edge>());
    }
    Graph::Node& Graph::getNode(Tile* t){
        return *std::find_if(nodes.begin(),nodes.end(),[&](Node& n){
                            return (t == n.tile);
                         });
    }
    void Graph::removeNode(const Node& n){
        /// @todo Fix node.index
        isolate(n);
        //edges.erase(edges.begin() + n.index);
        //nodes.erase(nodes.begin() + n.index);
    }
    void Graph::addUndirectedEdge(const Edge& e){
        edges[e.prev].emplace_back(e);
        Edge tmp(e.next,e.prev,e.weight);
        edges[tmp.prev].emplace_back(tmp);
    }
    void Graph::addDirectedEdge(const Edge& e){
        edges[e.prev].emplace_back(e);
    }
    void Graph::removeDirectedEdge(const Edge& e){
        edges[e.prev].erase(std::remove(edges[e.prev].begin(),edges[e.prev].end(),e),edges[e.prev].end());
    }
    void Graph::removeUndirectedEdge(const Edge& e){
        edges[e.prev].erase(std::remove(edges[e.prev].begin(),edges[e.prev].end(),e),edges[e.prev].end());
        Edge tmp(e.next,e.prev,e.weight);
        edges[tmp.prev].erase(std::remove(edges[tmp.prev].begin(),edges[tmp.prev].end(),tmp),edges[tmp.prev].end());
    }
    void Graph::removeAllEdges(const Node& n){
        edges[n.index].clear();
    }
    void Graph::isolate(const Node& n){
        edges[n.index].clear();
    //#define PARALLEL_ISOLATE
    #ifdef PARALLEL_ISOLATE
        int index = n.index;
        auto removeEdges = [index](std::vector<Edge>& vec){
            vec.erase(std::remove_if(std::begin(vec),std::end(vec),
                [index](const Edge& e){
                    return (index == e.next);
                }), std::end(vec));
        };
        parallel::for_each(edges.begin(),edges.end(),removeEdges);
    #else
        for(auto& vec:edges){
            vec.erase(std::remove_if(std::begin(vec),std::end(vec),
                [&n](const Edge& e){
                    return (n.index == e.next);
                }), std::end(vec));
        }
    #endif
    }
    bool Graph::areConnected(const Node& n1,const Node& n2){
        bool n1Connected = std::find_if(std::begin(edges[n1.index]),std::end(edges[n1.index]),
                                        [&](const Edge& e){
                                            return (n1.index == e.prev && n2.index == e.next);
                                        }) != edges[n1.index].end();
        bool n2Connected = std::find_if(std::begin(edges[n2.index]),std::end(edges[n2.index]),
                                        [&](const Edge& e){
                                            return (n2.index == e.prev && n1.index == e.next);
                                        }) != edges[n2.index].end();
        return (n1Connected && n2Connected);
    }
    void Graph::genMaze(Node& origin,float randomness,sf::Sprite spr,float binomial){
        // growing Tree algorithm
        std::vector<Node*> cells;
        reset();
        std::random_device rd;
        std::mt19937 rEngine(rd());
        cells.emplace_back(&origin);
        std::bernoulli_distribution bdRandomness(randomness);
        std::bernoulli_distribution bdBinomial(binomial);
        while(!cells.empty()){
            Node* cell = nullptr;
            auto sizeCells = cells.size();
            if(bdRandomness(rEngine)){
                cell = cells[random<int>(1,sizeCells,rEngine) - 1];
            }else{
                cell = cells.back();
            }
            std::vector<Node*> notVisitedCells;
            if(!allNeighboursVisited(cell,notVisitedCells)){
                int index = 0;
                auto NotVisitedCellsSize = notVisitedCells.size();
                if(bdBinomial(rEngine)){
                    #ifndef ANDROID // there's a bug in android(c++_shared) binomial_distribution that freezes the thread.
                        index = random<int,std::binomial_distribution<int>>(1,NotVisitedCellsSize,rEngine) - 1;
                    #else
                        index = random<int>(1,NotVisitedCellsSize,rEngine) - 1;
                    #endif
                }else{
                    index = random<int>(1,NotVisitedCellsSize,rEngine) - 1;
                }
                auto& wall = *notVisitedCells[index];
                wall.visited = true;
                cells.emplace_back(&wall);
                notVisitedCells.erase(notVisitedCells.begin() + index);
                if(!notVisitedCells.empty()){
                    --NotVisitedCellsSize;
                    index = random<int>(1,NotVisitedCellsSize,rEngine) - 1;
                    auto& newCell = *notVisitedCells[index];
                    newCell.tile->setSprite(spr);
                    newCell.visited = true;
                    isolate(newCell);
                }
            }else{
                cells.erase(std::remove(std::begin(cells), std::end(cells), cell), std::end(cells));
            }
        }
    }
    bool Graph::allNeighboursVisited(Node* n,std::vector<Node*>& notVisited){
        auto visited = 0u;
        for(auto& e:edges[n->index]){
            if(nodes[e.next].visited){
                visited +=1;
            }else{
                notVisited.emplace_back(&nodes[e.next]);
            }
        }
        return (visited == edges[n->index].size());
    }
    std::vector<Graph::Node*> Graph::dfs(Node& s,Node& d){
        std::stack<Node*> stack;
        reset();
        std::vector<Node*> path;
        if(s == d){
            path.emplace_back(&s);
            return path;
        }
        stack.push(&s);
        while(!stack.empty()){
            auto& v = *stack.top();
            #ifdef SLOW_SEARCH
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            #endif
            stack.pop();
            if(!v.visited){
                v.tile->setColor(sf::Color::Green);
                v.visited = true;
                // look at the edges of current node.
                for(auto& e: edges[v.index]){
                    if(nodes[e.next].visited)
                        continue;
                    if(nodes[e.next].parent == -1){
                        nodes[e.next].parent = e.prev;
                    }
                    if(nodes[e.next] == d){
                        nodes[e.next].tile->setColor(sf::Color::Red);
                        Node* curr = &nodes[e.next];
                        // build the path
                        while(curr != &s && curr != nullptr){
                            #ifdef SLOW_SEARCH
                                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            #endif
                            path.emplace_back(curr);
                            if(curr->parent != -1){
                                auto index = curr->parent;
                                curr->parent = -1;
                                curr = &nodes[index];
                            }else{
                                curr = nullptr;
                            }
                            curr->tile->setColor(sf::Color::Magenta);
                        }
                        std::reverse(path.begin(),path.end());
                        return path;
                    }
                    stack.push(&nodes[e.next]);
                }
            }
        }
        return path;
    }
    std::vector<Graph::Node*> Graph::bfs(Node& s,Node& d){
        std::queue<Node*> queue;
        reset();
        std::vector<Node*> path;
        if(s == d){
            path.emplace_back(&s);
            return path;
        }
        queue.push(&s);
        while(!queue.empty()){
            #ifdef SLOW_SEARCH
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            #endif
            auto& v = *queue.front();
            queue.pop();
            if(!v.visited){
                v.tile->setColor(sf::Color::Green);
                v.visited = true;
                // look at the edges of current node.
                for(auto& e: edges[v.index]){
                    if(nodes[e.next].visited)
                        continue;
                    if(nodes[e.next].parent == -1){
                        nodes[e.next].parent = e.prev;
                    }
                    if(nodes[e.next] == d){
                        nodes[e.next].tile->setColor(sf::Color::Red);
                        Node* curr = &nodes[e.next];
                        // build the path
                        while(curr != &s && curr != nullptr){
                            #ifdef SLOW_SEARCH
                                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            #endif
                            path.emplace_back(curr);
                            if(curr->parent != -1){
                                auto index = curr->parent;
                                curr->parent = -1;
                                curr = &nodes[index];
                            }else{
                                curr = nullptr;
                            }
                            curr->tile->setColor(sf::Color::Magenta);
                        }
                        std::reverse(path.begin(),path.end());
                        return path;
                    }
                    queue.push(&nodes[e.next]);
                }
            }
        }
        return path;
    }
    std::vector<Graph::Node*> Graph::gbfs(Node& s,Node& d){
    #ifdef MANHATTAN
        auto eval = [&](Node* a,Node* b){
            auto sumA = 10 *(std::abs(a->tile->getPosition().x-d.tile->getPosition().x)
                           + std::abs(a->tile->getPosition().y-d.tile->getPosition().y));
            auto sumB = 10 *(std::abs(b->tile->getPosition().x-d.tile->getPosition().x)
                           + std::abs(b->tile->getPosition().y-d.tile->getPosition().y));
            return (sumA > sumB);
        };
    #elif defined CHEBYSHEV
        auto eval = [&](Node* a,Node* b){
            auto maxA = std::max(a->tile->getPosition().x - d.tile->getPosition().x,
                                    a->tile->getPosition().y - d.tile->getPosition().y);
            auto maxB = std::max(b->tile->getPosition().x - d.tile->getPosition().x,
                                    b->tile->getPosition().y - d.tile->getPosition().y);
            return (maxA > maxB);
        };
    #else // EUCLIDEAN
        auto eval = [&](Node* a,Node* b){
            auto hypotA = std::hypot(a->tile->getPosition().x - d.tile->getPosition().x,
                                        a->tile->getPosition().y - d.tile->getPosition().y);
            auto hypotB = std::hypot(b->tile->getPosition().x - d.tile->getPosition().x,
                                        b->tile->getPosition().y - d.tile->getPosition().y);
            return (hypotA > hypotB);
        };
    #endif
        std::priority_queue<Node*,std::deque<Node*>,decltype(eval)> queue(eval);
        reset();
        std::vector<Node*> path;
        if(s == d){
            path.emplace_back(&s);
            return path;
        }
        queue.push(&s);
        while(!queue.empty()){
            #ifdef SLOW_SEARCH
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            #endif
            auto& v = *queue.top();
            queue.pop();
            if(!v.visited){
                v.tile->setColor(sf::Color::Green);
                v.visited = true;
                // look at the edges of current node.
                for(auto& e: edges[v.index]){
                    if(nodes[e.next].visited)
                        continue;
                    if(nodes[e.next].parent == -1){
                        nodes[e.next].parent = e.prev;
                    }
                    if(nodes[e.next] == d){
                        nodes[e.next].tile->setColor(sf::Color::Red);
                        Node* curr = &nodes[e.next];
                        // build the path
                        while(curr != &s && curr != nullptr){
                            #ifdef SLOW_SEARCH
                                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            #endif
                            path.emplace_back(curr);
                            if(curr->parent != -1){
                                auto index = curr->parent;
                                curr->parent = -1;
                                curr = &nodes[index];
                            }else{
                                curr = nullptr;
                            }
                            curr->tile->setColor(sf::Color::Magenta);
                        }
                        std::reverse(path.begin(),path.end());
                        return path;
                    }
                    queue.push(&nodes[e.next]);
                }
            }
        }
        return path;
    }
    std::vector<Graph::Node*> Graph::aStar(Node& s,Node& d){
    #ifdef MANHATTAN
        auto eval = [&](Node* a,Node* b){
            auto sumA = 10 *(std::abs(a->tile->getPosition().x - d.tile->getPosition().x)
                           + std::abs(a->tile->getPosition().y - d.tile->getPosition().y));
            auto sumB = 10 *(std::abs(b->tile->getPosition().x - d.tile->getPosition().x)
                           + std::abs(b->tile->getPosition().y - d.tile->getPosition().y));
            return (sumA > sumB);
        };
    #elif defined CHEBYSHEV
        auto eval = [&](Node* a,Node* b){
            auto maxA = std::max(a->tile->getPosition().x - d.tile->getPosition().x,
                                    a->tile->getPosition().y - d.tile->getPosition().y);
            auto maxB = std::max(b->tile->getPosition().x - d.tile->getPosition().x,
                                    b->tile->getPosition().y - d.tile->getPosition().y);
            return (maxA > maxB);
        };
    #else // EUCLIDEAN
        auto eval = [&](Node* a,Node* b){
            auto hypotA = std::hypot(a->tile->getPosition().x - d.tile->getPosition().x,
                                        a->tile->getPosition().y - d.tile->getPosition().y);
            auto hypotB = std::hypot(b->tile->getPosition().x - d.tile->getPosition().x,
                                        b->tile->getPosition().y - d.tile->getPosition().y);
            return (hypotA > hypotB);
        };
    #endif
        std::priority_queue<Node*,std::deque<Node*>,decltype(eval)> queue(eval);
        reset();
        std::vector<Node*> path;
        if(s == d){
            path.emplace_back(&s);
            return path;
        }
        queue.push(&s);
        auto evalEdge = [](Edge* e1,Edge* e2){
            auto w1 = std::abs(e1->weight);
            auto w2 = std::abs(e2->weight);
            return (w1 >= w2);
        };
        while(!queue.empty()){
            #ifdef SLOW_SEARCH
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            #endif
            auto& v = *queue.top();
            queue.pop();
            if(!v.visited){
                v.tile->setColor(sf::Color::Green);
                v.visited = true;
                std::priority_queue<Edge*,std::deque<Edge*>,decltype(evalEdge)> edgeQueue(evalEdge);
                // look at the edges of current node.
                for(auto& e: edges[v.index]){
                    if(nodes[e.next].visited)
                        continue;
                    if(nodes[e.next].parent == -1){
                        nodes[e.next].parent = e.prev;
                    }
                    if(nodes[e.next] == d){
                        nodes[e.next].tile->setColor(sf::Color::Red);
                        Node* curr = &nodes[e.next];
                        // build the path
                        while(curr != &s && curr != nullptr){
                            #ifdef SLOW_SEARCH
                                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            #endif
                            path.emplace_back(curr);
                            if(curr->parent != -1){
                                auto index = curr->parent;
                                curr->parent = -1;
                                curr = &nodes[index];
                            }else{
                                curr = nullptr;
                            }
                            curr->tile->setColor(sf::Color::Magenta);
                        }
                        std::reverse(std::begin(path),std::end(path));
                        return path;
                    }
                    edgeQueue.push(&e);
                }
                while(!edgeQueue.empty()){
                    auto& e = *edgeQueue.top();
                    edgeQueue.pop();
                    queue.push(&nodes[e.next]);
                }
            }
        }
        return path;
    }
    Graph::~Graph(){
        nodes.clear();
        edges.clear();
    }
    void Graph::reset(){
        for(auto& n: nodes){
            n.visited = false;
            n.tile->setColor(sf::Color::White);
            n.parent = -1;
        }
    }

std::vector<std::vector<Tile>> createGrid(std::vector<sf::Texture>& textures, const unsigned& row, const unsigned& col, bool isometric){
    auto m = std::vector<std::vector<Tile>>(col);
    auto type = Tile::Type::GRASS;
    sf::Sprite s;
    float x = 0,y = 0;
    for(auto i=0u;i<col;++i){
        m[i] = std::vector<Tile>(row,Tile(sf::Vector2f(0,0),type,s,isometric));
        auto& borders = m[i][0].getBounds();
        for(auto j=0u;j<row;++j){
            s = sf::Sprite(textures[type]);
            s.setScale(0.5f,0.5f);
            if(isometric){
                x = j * borders.x / 2;
                y = i * borders.y / 2;
            }else{
                x = j * borders.x;
                y = i * borders.y;
            }
            m[i][j] = Tile(sf::Vector2f(x,y),type,s,isometric);
        }
    }
    return m;
}

void createGraph(Graph* g,std::vector<std::vector<Tile>>& grid,bool diagonal){
    for(auto& gr:grid){
        for(auto& t:gr){
            g->addNode(Graph::Node(&t));
        }
    }
    int col = grid.size();
    int row = grid[0].size();
    auto size = row * col;
    static std::random_device rd;
    static std::default_random_engine rEngine(rd());
    for(int i=0;i<size;++i){
        // down
        if((i%row)!=(row-1)){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+1),random(0,size,rEngine)));
        }
        // right
        if((i+row)<size){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+row),random(0,size,rEngine)));
        }
        // diagonal
        if(diagonal){
            // right-down : left-top
            if((i+row+1)<size){
                g->addUndirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+row+1),random(0,size,rEngine)));
            }
            // left-down : right-top
            if((i+row-1)<size){
                g->addUndirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i+row-1),random(0,size,rEngine)));
            }
        }
        // left
        if((i-row)>=0){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i-row),random(0,size,rEngine)));
        }
        // up
        if((i-1)>=0 && ((i-1)%row)!=(row-1)){
            g->addDirectedEdge(Graph::Edge(std::size_t(i),std::size_t(i-1),random(0,size,rEngine)));
        }
    }
}
