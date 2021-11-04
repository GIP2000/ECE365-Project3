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

list<string> graph::getNames(){
    list<string> lst; 
    for(auto& v: this->verts){
        lst.push_back(v.name);
    }
    return lst; 
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

hashTable graphWithDijkstras::Dijkstra(const string& start){

    // initialize the heap and hashtable 
    heap dheap(this->getSize());
    hashTable ktable(this->getSize()); 
    for(auto& v: this->verts){
        DijkstraHelper* dh = new DijkstraHelper(&v);
        dheap.insert(v.name,v.name == start ? 0 : INT_MAX,dh);
    }
    
    string vName;
    int dv;
    DijkstraHelper* dhp;
    
    // while there are still unkown values 
    while(dheap.deleteMin(&vName,&dv,(void**)&dhp) != 1){
        
        // Add to the known set 
        NodeInfo* ni = new NodeInfo(vName,dhp->path == nullptr ? vName : dhp->path->name,dv); 
        ktable.insert(vName,ni); 

        // for every edge v->w
        for(auto e: dhp->self->edges){
            int cvw = get<1>(e);
            graph::vertex* w = get<0>(e); 
            bool found;  
            int dw = dheap.getKey(w->name,&found);
            if(!found){
                NodeInfo* tdhp = (NodeInfo*)ktable.getPointer(w->name,&found); 
                if(!found) {
                    cerr << "Unreachable\n";
                    exit(-1); 
                }
                dw = tdhp->weight; 
            }

            // if new distance is smaller than current distance 
            if(dv + cvw < dw){
                dheap.setKey(w->name,dv + cvw);
                DijkstraHelper * updatedDHP = new DijkstraHelper(w,dhp->self);
                if(found) delete (DijkstraHelper *)dheap.getPointer(w->name);  
                dheap.setPointer(w->name,updatedDHP); 
            }
        }
        delete dhp; 

    }
    return ktable; 
}