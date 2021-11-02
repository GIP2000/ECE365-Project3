#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>
#include "./heap.h"
#include "./graph.h"

using namespace std;

class DijkstraHelper {
    public:
        DijkstraHelper(graph::vertex* self = nullptr,graph::vertex* path = nullptr){
            this -> self = self;
            this -> path = path;
        }
        graph::vertex* self = {nullptr};
        graph::vertex* path = {nullptr};
};


graph generateGraph(const string& filename){
    ifstream inputfile(filename); 
    graph g; 
    if(inputfile.is_open()){
        string line; 
        while(getline(inputfile, line)){
            string word;
            int counter = 0; 
            stringstream ss(line);

            string start; 
            string end;
            int weight; 

            while(getline(ss,word, ' ')){
                switch(counter){
                    case 0:
                        start = word; 
                        break;
                    case 1:
                        end = word;
                        break;
                    case 2:
                        weight = stoi(word);
                        break;
                    default:
                        cerr << "Unreachable \n"; 
                }
                counter++; 
            }
            g.insert(start,end,weight);
            
        }
    }
    inputfile.close();
    return g; 
}

heap Dijkstra(graph& g, const string& start){
    heap dheap(g.getSize());
    heap kheap(g.getSize());
    for(auto& v: g.verts){
        DijkstraHelper* dh = new DijkstraHelper(&v);
        dheap.insert(v.name,v.name == start ? 0 : INT_MAX,dh);
    }
    string name;
    int dv;
    DijkstraHelper* dhp;
    while(dheap.deleteMin(&name,&dv,(void**)&dhp) != 1){
        kheap.insert(name,dv,dhp);
        for(auto e: dhp->self->edges){
            int cvw = get<1>(e);
            graph::vertex* w = get<0>(e); 
            bool found;  
            int dw = dheap.getKey(w->name,&found);
            if(!found){
                dw = kheap.getKey(w->name,&found);
                if(!found) {
                    cerr << "Unreachable\n";
                    exit(-1); 
                }
            }
            if(dv + cvw < dw){
                dheap.setKey(w->name,dv + cvw);
                DijkstraHelper * updatedDHP = new DijkstraHelper(w,dhp->self); 
                dheap.setPointer(w->name,updatedDHP); 
            }
        }

    }
    return kheap; 
    
    
}

string getPath(const string& start, const string& current, heap& kh){
    bool found; 
    DijkstraHelper* dh = (DijkstraHelper*)kh.getPointer(current,&found); 
    if(!found){
        cerr << "Unreachable \n";
        exit(-1);
    }
    string pname = dh->path == nullptr ? start : dh->path->name;
    
    if(current == start) return "[" + start;
    return getPath(start,pname,kh) + ", " + current; 

    
    
}

void writeToOutput(const string& filename, const string& startNode, graph& g, heap& kh){
    ofstream outstream(filename);
    for(auto& v: g.verts){
        bool found; 
        int distance = kh.getKey(v.name,&found);
        if(!found){
            cerr << "Unreachable \n";
            exit(-1);
        }

        string fs = getPath(startNode,v.name,kh) + "]"; 
        outstream << v.name << ": " << distance << " " << fs << "\n"; 
    }
    outstream.close(); 
}


int main(){
    cout << "Enter name of graph file: "; 
    string graphFile;
    cin >> graphFile;
    graph g = generateGraph(graphFile);
    cout << "Enter name of starting vertex: "; 
    string startNode; 
    cin >> startNode; 
    heap kheap = Dijkstra(g,startNode); 
    cout << "Enter name of output file: "; 
    string outfile; 
    cin >> outfile;
    writeToOutput(outfile,startNode,g,kheap); 

    return 0; 
}