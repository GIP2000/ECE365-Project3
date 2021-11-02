#ifndef _GRAPH_H
#define _GRAPH_H

#include <list>
#include <tuple>
#include <string>
#include "./hash.h"

class graph {
     
    hashTable hashmap; 
    public:
        class vertex{
            public:
                std::string name {""}; 
                std::list<std::tuple<vertex*,int>> edges; 
                vertex() = default; 
                vertex(const std::string& name); 
        }; 
        std::list<vertex> verts; 
        graph() = default; 

        // Inserts node into the graph with an edge
        // if node exists just adds edge to existing node
        // if endNode doesn't exist it will first add the endNode 
        // return 0 on success 
        int insert(const std::string& node, const std::string& endNode, int weight); 

        // gets the vertex with the given name from the hashtable
        // returns nullptr if not found
        vertex* getItem(const std::string& node); 

        std::list<vertex> getList(); 

        int getSize();

        void print(); 


    
};


#endif