#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <stack>
#include <random>
#include <queue>
#include "Tile.hpp"
/**
 * @brief Graph class.
 * @author Cristian Glez <Cristian.glez.m@gmail.com>
 */
class Graph{
    public:
        /**
         * @brief Node
         */
        struct Node{
            Tile* tile;
            bool visited;
            unsigned int index;
            int parent;
            Node(Tile* t);
            bool operator==(const Node& rhs) const;
            bool operator!=(const Node& rhs) const;
        };
        /**
         * @brief Edge connects 1 node to another
         */
        struct Edge{
            int weight;
            std::size_t prev;
            std::size_t next;
            Edge();
            Edge(const std::size_t& p,const std::size_t& n,const int& w);
            bool operator==(const Edge& rhs) const;
            bool operator!=(const Edge& rhs) const;
        };
        /**
         * @brief Constructor
         */
        explicit Graph();
        /**
         * @brief adds a node to the graph
         * @param n Graph::Node
         */
        void addNode(Node n);
        /**
         * @brief getter for a node by tile
         * @param t Tile*
         * @return Node &
         */
        Node& getNode(Tile* t);
        /**
         * @brief does the same as Graph::isolate
         * @param n const Graph::Node &
         */
        void removeNode(const Node& n);
        /**
         * @brief adds 2 edges one connecting prev to next and another connecting next to prev
         * @param e const Graph::Edge &
         */
        void addUndirectedEdge(const Edge& e);
        /**
         * @brief Adds one edge connecting p to n
         * @param e const Graph::Edge &
         */
        void addDirectedEdge(const Edge& e);
        /**
         * @brief removes the edge connecting from prev to next
         */
        void removeDirectedEdge(const Edge& e);
        /**
         * @brief Removes two edges one connecting prev to next and another connecting next to prev.
         * @param e const Graph::Edge &
         */
        void removeUndirectedEdge(const Edge& e);
        /**
         * @brief Removes all edges from this node.
         * @param n const Graph::Node &
         */
        void removeAllEdges(const Node& n);
        /**
         * @brief Removes all the edges connecting from and to this node.
         * @param n const Graph::Node &
         */
        void isolate(const Node& n);
        /**
         * @brief Checks if a node has neighbors that are not visited.
         * @param n Graph::Node *
         * @param notVisited std::vector<Node*> nodes that are not visited.
         * @return bool true if all have been visited, false otherwise.
         */
        bool allNeighboursVisited(Node* n,std::vector<Node*>& notVisited);
        /**
         * @brief Generates a Maze.
         * @param origin Node & node to start from.
         * @param randomness float way of choosing the next node to process(0.0 = linial,1.0 = random)
         * @param spr sf::Sprite image for the walls
         * @param binomial float way of choosing the neighbor( 0.0 = uniform, 1.0 = binomial)
         */
        void genMaze(Node& origin,float randomness = 0.25f,sf::Sprite spr = sf::Sprite(),float binomial = 0.5f);
        /**
         * @brief Deep-First Search
         * @param s Graph::Node & src
         * @param d Graph::Node & dest
         * @return std::vector<Node*> path
         */
        std::vector<Node*> dfs(Node& s,Node& d);
        /**
         * @brief Breadth-First Search
         * @param s Graph::Node & src
         * @param d Graph::Node & dest
         * @return std::vector<Node*> path
         */
        std::vector<Node*> bfs(Node& s,Node& d);
        /**
         * @brief Greedy-Best-First Search
         * @param s Graph::Node & src
         * @param d Graph::Node & dest
         * @return std::vector<Node*> path
         */
        std::vector<Node*> gbfs(Node& s,Node& d);
        ~Graph();
    private:
        /**
         * @brief Resets parents, visited and the color of tile
         */
        void reset();
    private:
    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> edges;
};

template<typename T,
         typename Distribution = std::uniform_int_distribution<T>,
         typename Engine = std::default_random_engine(std::random_device())>
T random(T min,T max,Engine rEngine){
    Distribution dice(min,max);
    return dice(rEngine);
}

#endif // GRAPH_HPP
