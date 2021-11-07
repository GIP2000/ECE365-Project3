#ifndef _GRAPH_H
#define _GRAPH_H

#include <list>
#include <tuple>
#include <string>
#include <climits>
#include "./hash.h"

class graph {
    
    protected:  
        // Class describing the each vertex
        class vertex{
            friend class graph; 
            private:
                std::string name {""}; 
                // edge which is a tuple with the first value the pointer to a vertex
                // the second value is the cost of the edge
                std::list<std::tuple<vertex*,int>> edges; 
            public:
                vertex() = default; 
                vertex(const std::string& name); 
                std::list<std::tuple<vertex*,int>>::iterator begin(){ return edges.begin();}
                std::list<std::tuple<vertex*,int>>::iterator end(){ return edges.end();}
                std::string getName(){return name;}
        }; 
        // list of vertecies 
        std::list<vertex> verts; 
    private: 
        // hashtable containing all the vertecies and their pointer inside the list 
        hashTable hashmap; 

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


        // Prints the vertecies and its edges
        // Fromat: 
        //          StartVName1: (endVName1,cost1) (endVName2,cost2) ...
        //          StartVName2: (endVName1,cost1) (endVName2,cost2) ...
        void print(); 

        // returns true if a vertex with the name exists
        bool contains(const std::string& node); 

        std::list<vertex>::iterator begin() {return verts.begin();}
        std::list<vertex>::iterator end() {return verts.end();}


        class Pair{
            public:
                Pair(vertex* self = nullptr, vertex* path = nullptr);
                vertex* self; 
                vertex* path; 
        };
};

#endif