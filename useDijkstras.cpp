#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>
#include "./heap.h"
#include "./graph.h"
#include <chrono>

using namespace std;

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


// Returns the start node as a string if the start node is not in the vertex list then retry
string getStartNode(graph& g){
    cout << "Enter name of starting vertex: "; 
    string startNode; 
    cin >> startNode; 

    if(!g.contains(startNode)) {
        cout << "Invalid vertex name: " << startNode << ". Please insert a valid vertex name" << "\n"; 
        return getStartNode(g); 
    }
    return startNode;


}


// Performs Dijkstra's algorithm on the graph starting from start vertex
// Returns a hashTable of node info objects with the distances and paths associated with each node
hashTable Dijkstra(graph &g, const string& start){
    // initialize the heap and hashtable 
    heap dheap(g.getSize());
    hashTable ktable(g.getSize()); 
    for(auto& v: g){
        graph::Pair* pair = new graph::Pair(&v); 
        dheap.insert(v.getName(),v.getName() == start ? 0 : INT_MAX,pair);
    }

    string vName;
    int dv;
    graph::Pair * pair; 
    while(dheap.deleteMin(&vName,&dv,(void**)&pair) != 1){
        // Add to the known set 
        NodeInfo* ni = new NodeInfo(vName,pair->path == nullptr ? vName : pair->path->getName(),dv); 
        ktable.insert(vName,ni); 

        // for every edge v->w
        for(auto& e: *(pair->self)){
            int cvw = get<1>(e);
            auto* w = get<0>(e); 
            bool found;  
            int dw = dheap.getKey(w->getName(),&found);
            if(!found){
                NodeInfo* ni = (NodeInfo*)ktable.getPointer(w->getName(),&found); 
                if(!found) {
                    cerr << "Unreachable\n";
                    exit(-1); 
                }
                dw = ni->weight; 
            }

            // if new distance is smaller than current distance 
            if(dv + cvw < dw){
                dheap.setKey(w->getName(),dv + cvw);
                graph::Pair* wp = (graph::Pair*)dheap.getPointer(w->getName());
                wp->path = pair->self;  
            }
        }
        delete pair; 
    }
    return ktable;
}    


// Reads the input file and generates a graph (of type graphWithDijkstras) with the given vertecies and edges
// file content will be a new line sep file of values tokenized by: 
// {startnode} {endNode} {weight}
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

            ss >> start >> end >> weight;

            g.insert(start,end,weight);
            
        }
    }
    inputfile.close();
    return g; 
}


// Reccursive function that creates the string for the path from Vcurrent -> Vdestination
// start is the name of the starting vertex (where Dijkstra's ran in refrence to). doesn't change in reccursive routine
// current is the name of the current vertex in the path. This changes based on how far we transversed the path
// kh is a hashtable that represents the results of Dijkstra's algorthim its value is a graphWithDijkstras::NodeInfo*
string getPath(const string& start, const string& current, hashTable& kh){
    bool found; 
    NodeInfo* ni = (NodeInfo*)kh.getPointer(current,&found); 
    if(!found){
        cerr << "Unreachable \n";
        exit(-1);
    }
    string pname = ni->path == "" ? start : ni->path;
    
    if(current == start) return "[" + start;
    return getPath(start,pname,kh) + ", " + current; 
}


// Writes the output file containing the min distance between vertecies from a start vertex
// format is {vName}: {weight} [{vName, Va, ...,Vstart}]
// takes a string filename to write to
// a string startNode which is the start vertex
// a hashtable kh which is the hashtable outputed by Dijkstra's algorthim
void writeToOutput(const string& filename, const string& startNode, graph& g, hashTable& kh){
    ofstream outstream(filename);
    for(auto& v: g){
        bool found; 
        NodeInfo* ni = (NodeInfo*)kh.getPointer(v.getName(),&found); 
        if(!found){
            cerr << "Unreachable \n";
            exit(-1);
        }
        int distance = ni->weight; 
        if(distance < 0 || distance == INT_MAX){
            outstream << v.getName() << ": " << "NO PATH" << "\n";
            continue;
        }
        string fs = getPath(startNode,v.getName(),kh) + "]"; 
        outstream << v.getName() << ": " << distance << " " << fs << "\n"; 
    }
    outstream.close(); 
}

int main(){

    // make graph
    cout << "Enter name of graph file: "; 
    string graphFile;
    cin >> graphFile;
    graph g = generateGraph(graphFile);
    string startNode = getStartNode(g); 

    // Apply Dijkstra's algorithm
    auto start = chrono::_V2::steady_clock::now();
    // hashTable ktable = g.Dijkstra(startNode); 
    hashTable ktable = Dijkstra(g,startNode);
    auto end = chrono::_V2::steady_clock::now();
    chrono::duration<double> duration = end-start; 
    cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << duration.count() << endl;

    // Write to output file
    cout << "Enter name of output file: "; 
    string outfile; 
    cin >> outfile;
    writeToOutput(outfile,startNode,g,ktable); 

    return 0; 
}