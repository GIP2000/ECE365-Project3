#include "./graph.h"
#include "./heap.h"
#include "./hash.h"
#include <string>
#include <tuple>

#include <iostream> // for debugging

using namespace std;

graph::vertex::vertex(const string& name){
    this->name = name; 
}
// graph::Pair::Pair(vertex* self, vertex* path){
//     this->self = self; 
//     this->path = path; 
// }

bool graph::contains(const std::string& node){
    return this->hashmap.contains(node);
}

int graph::insert(const string& node, const string& endNode, int weight) {
    bool nodeExist, endNodeExist;
    vertex* startVertex = (vertex*)this->hashmap.getPointer(node,&nodeExist); 
    vertex* endVertex = (vertex*)this->hashmap.getPointer(endNode,&endNodeExist);
    // add start node if it doesn't exit
    if(!nodeExist) {
        this->verts.push_back(vertex(node));
        this->hashmap.insert(node,&(this->verts.back()));
        startVertex = &(this->verts.back()); 
    }

    // add end node if it doesn't exit 
    if(!endNodeExist) {
        this->verts.push_back(vertex(endNode));
        this->hashmap.insert(endNode,&(this->verts.back()));
        endVertex = &(this->verts.back()); 
    }
    
    // set the edge 
    tuple<vertex*,int> edge = {endVertex,weight};
    startVertex->edges.push_back(edge);
    return 0; 
}


int graph::getSize(){
    return this->verts.size(); 
}


void graph::print(){
    for(auto& v : this->verts){
        cout << v.name << ": ";
        for(auto& e : v){
            cout << "(" << get<0>(e)->name << "," << get<1>(e) << ") ";
        }
        cout << "\n";
    }
}
