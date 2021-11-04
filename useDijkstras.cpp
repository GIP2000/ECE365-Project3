#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>
#include "./heap.h"
#include "./graph.h"
#include <chrono>

using namespace std;

// Reads the input file and generates a graph (of type graphWithDijkstras) with the given vertecies and edges
// file content will be a new line sep file of values tokenized by: 
// {startnode} {endNode} {weight}
graphWithDijkstras generateGraph(const string& filename){
    ifstream inputfile(filename); 
    graphWithDijkstras g; 
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


// Reccursive function that creates the string for the path from Vcurrent -> Vdestination
// start is the name of the starting vertex (where Dijkstra's ran in refrence to). doesn't change in reccursive routine
// current is the name of the current vertex in the path. This changes based on how far we transversed the path
// kh is a hashtable that represents the results of Dijkstra's algorthim its value is a graphWithDijkstras::NodeInfo*
string getPath(const string& start, const string& current, hashTable& kh){
    bool found; 
    graphWithDijkstras::NodeInfo* dh = (graphWithDijkstras::NodeInfo*)kh.getPointer(current,&found); 
    if(!found){
        cerr << "Unreachable \n";
        exit(-1);
    }
    string pname = dh->path == "" ? start : dh->path;
    
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
    for(auto& v: g.getNames()){
        bool found; 
        graphWithDijkstras::NodeInfo* dhp = (graphWithDijkstras::NodeInfo*)kh.getPointer(v,&found); 
        if(!found){
            cerr << "Unreachable \n";
            exit(-1);
        }
        int distance = dhp->weight; 
        if(distance < 0 || distance == INT_MAX){
            outstream << v << ": " << "NO PATH" << "\n";
            continue;
        }
        string fs = getPath(startNode,v,kh) + "]"; 
        outstream << v << ": " << distance << " " << fs << "\n"; 
    }
    outstream.close(); 
}

int main(){

    // make graph
    cout << "Enter name of graph file: "; 
    string graphFile;
    cin >> graphFile;
    graphWithDijkstras g = generateGraph(graphFile);
    cout << "Enter name of starting vertex: "; 
    string startNode; 
    cin >> startNode; 

    // Apply Dijkstra's algorithm
    auto start = chrono::_V2::steady_clock::now();
    hashTable ktable = g.Dijkstra(startNode); 
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