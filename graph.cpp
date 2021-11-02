#include "./graph.h"
#include <string>
#include <tuple>

#include <iostream> // for debugging

using namespace std;

graph::vertex::vertex(const string& name){
    this->name = name; 
}

int graph::insert(const string& node, const string& endNode, int weight) {
    bool nodeExist, endNodeExist;
    vertex* startVertex = (vertex*)this->hashmap.getPointer(node,&nodeExist); 
    vertex* endVertex = (vertex*)this->hashmap.getPointer(endNode,&endNodeExist);
    if(!nodeExist) {
        this->verts.push_back(vertex(node));
        this->hashmap.insert(node,&(this->verts.back()));
        startVertex = &(this->verts.back()); 
    }
    if(!endNodeExist) {
        this->verts.push_back(vertex(endNode));
        this->hashmap.insert(endNode,&(this->verts.back()));
        endVertex = &(this->verts.back()); 
    }
    tuple<vertex*,int> edge = {endVertex,weight};
    startVertex->edges.push_back(edge);
    return 0; 
}

graph::vertex* graph::getItem(const string& node){
    return (vertex*)this->hashmap.getPointer(node); 
}

list<graph::vertex> graph::getList(){
    return this->verts; 
}


int graph::getSize(){
    return this->verts.size(); 
}

void graph::print(){
    for(auto& v : this->verts){
        cout << v.name << ": ";
        for(auto& e : v.edges){
            cout << "(" << get<0>(e)->name << "," << get<1>(e) << ") ";
        }
        cout << "\n";
    }
}