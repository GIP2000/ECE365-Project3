#ifndef _GRAPH_H
#define _GRAPH_H

#include <list>
#include <tuple> 

class graph {
    public:
        graph();
        //return 0 on success 
        int insert(std::string node, std::string path, int weight); 

    private: 
        class graphItem{
            public:
                std::string name {""}; 
                std::list<std::tuple<std::string,int>> edges; 
                graphItem() = default; 
        }; 
        std::list<graphItem> vertecies; 
};


#endif