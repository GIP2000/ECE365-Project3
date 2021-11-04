#ifndef _GRAPH_H
#define _GRAPH_H

#include <list>
#include <tuple>
#include <string>
#include <climits>
#include "./hash.h"

class graph {
    private: 
        // hashtable containing all the vertecies and their pointer inside the list 
        hashTable hashmap; 
    protected:  
        // Class describing the each vertex
        class vertex{
            public:
                std::string name {""}; 

                // edge which is a tuple with the first value the pointer to a vertex
                // the second value is the cost of the edge
                std::list<std::tuple<vertex*,int>> edges; 
                vertex() = default; 
                vertex(const std::string& name); 
        }; 
        // list of vertecies 
        std::list<vertex> verts; 
        

    public:
        graph() = default; 

        // Inserts node into the graph with an edge
        // if node exists just adds edge to existing node
        // if endNode doesn't exist it will first add the endNode 
        // return 0 on success 
        int insert(const std::string& node, const std::string& endNode, int weight); 

        // returns the size of the list
        int getSize();
        
        // returns a list of strings containing the names of the vertecies
        std::list<std::string> getNames(); 


        // Prints the vertecies and its edges
        // Fromat: 
        //          StartVName1: (endVName1,cost1) (endVName2,cost2) ...
        //          StartVName2: (endVName1,cost1) (endVName2,cost2) ...
        void print(); 


    
};

// An extention of the graph class that implements Dijkstra's Algorthim 
class graphWithDijkstras : public graph {
    public: 
        graphWithDijkstras() : graph() {}
        
        // Calculates the min distance between the start node (string) and every node in the graph using Dijkstra's Algorthim
        // Returns a hashTable with the key being the vertex name and the pv a NodeInfo*
        hashTable Dijkstra(const std::string& start); 

        // A helper class containing all the information crutial to Dijkstra's algorthim
        class NodeInfo {
            public:
                NodeInfo(const std::string& self, const std::string& path, int weight){
                    this -> self = self;
                    this -> path = path;
                    this -> weight = weight; 
                }
                std::string self = {""}; 
                std::string path = {""}; 
                int weight = {INT_MAX}; 
        };
        
    private:

        // A helper class used to calculate the min distance internally 
        class DijkstraHelper {
            public:
                DijkstraHelper(graph::vertex* self = nullptr,graph::vertex* path = nullptr){
                    this -> self = self;
                    this -> path = path;
                }
                
                graph::vertex* self = {nullptr};
                graph::vertex* path = {nullptr};
        };

    
}; 
#endif