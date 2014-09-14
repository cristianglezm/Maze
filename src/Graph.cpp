#include "Graph.hpp"
#include <iostream>
//#define SLOW_SEARCH
#ifdef SLOW_SEARCH
    #include <thread>
#endif

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
    Graph::Graph()
    : nodes()
    , edges(){}
    void Graph::addNode(Node n){
        n.index = nodes.size();
        nodes.push_back(n);
        edges.push_back(std::vector<Edge>());
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
        edges[e.prev].push_back(e);
        Edge tmp(e.next,e.prev,e.weight);
        edges[tmp.prev].push_back(tmp);
    }
    void Graph::addDirectedEdge(const Edge& e){
        edges[e.prev].push_back(e);
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
        for(auto& vec: edges){
            vec.erase(std::remove_if(vec.begin(),vec.end(),
                                     [&](const Edge& e){ return (e.next == n.index);}),
                                     vec.end());
        }
    }
    void Graph::genMaze(Node& origin,float randomness,sf::Sprite spr,float binomial){
        // growing Tree algorithm
        std::vector<Node*> cells;
        reset();
        std::random_device rd;
        std::default_random_engine rEngine(rd());
        cells.push_back(&origin);
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
                int num = 0;
                if(bdBinomial(rEngine)){
                    num = random<int,std::binomial_distribution<int>>(1,notVisitedCells.size(),rEngine) - 1;
                }else{
                    num = random<int>(1,notVisitedCells.size(),rEngine) - 1;
                }
                Node* wall = notVisitedCells[num];
                notVisitedCells.erase(notVisitedCells.begin() + num);
                wall->visited = true;
                cells.push_back(wall);
                if(!notVisitedCells.empty()){
                    num = random<int>(1,notVisitedCells.size(),rEngine) - 1;
                    auto newCell = notVisitedCells[num];
                    isolate(*newCell);
                    newCell->tile->setSprite(spr);
                    newCell->visited = true;
                }
            }else{
                cells.erase(std::remove(cells.begin(),cells.end(),cell),cells.end());
            }
        }
    }
    bool Graph::allNeighboursVisited(Node* n,std::vector<Node*>& notVisited){
        unsigned int visited = 0;
        for(auto& e:edges[n->index]){
            if(nodes[e.next].visited){
                visited +=1;
            }else{
                notVisited.push_back(&nodes[e.next]);
            }
        }
        return (visited == edges[n->index].size());
    }
    std::vector<Graph::Node*> Graph::dfs(Node& s,Node& d){
        std::stack<Node*> stack;
        reset();
        std::vector<Node*> path;
        if(s == d){
            path.push_back(&s);
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
                            path.push_back(curr);
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
            path.push_back(&s);
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
                            path.push_back(curr);
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
        auto eval = [&](Node* a,Node* b){
            auto sumA = 10 *(std::abs(a->tile->getPosition().x-d.tile->getPosition().x)
                           + std::abs(a->tile->getPosition().y-d.tile->getPosition().y));
            auto sumB = 10 *(std::abs(b->tile->getPosition().x-d.tile->getPosition().x)
                           + std::abs(b->tile->getPosition().y-d.tile->getPosition().y));
            return (sumA > sumB);
        };
        std::priority_queue<Node*,std::deque<Node*>,decltype(eval)> queue(eval);
        reset();
        std::vector<Node*> path;
        if(s == d){
            path.push_back(&s);
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
                            path.push_back(curr);
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
    void Graph::reset(){
        for(auto& n: nodes){
            n.visited = false;
            n.tile->setColor(sf::Color::White);
            n.parent = -1;
        }
    }
