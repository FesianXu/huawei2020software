// C++ Implementation of Kosaraju's algorithm to print all SCCs 
#include <iostream> 
#include <list> 
#include <stack> 
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
using namespace std; 


// vector<vector<int>> scc; // store the strong-connected component
class Record {
public:
    uint32_t source_id,target_id;
    uint32_t account;
public:
    Record(uint32_t source_id, uint32_t target_id, uint32_t account){
        source_id = source_id;
        target_id = target_id;
        account = account;
    }
};
  
class Graph 
{ 
    int V;    // No. of vertices 
    list<int> *adj;    // An array of adjacency lists 
    vector<vector<int>> scc; // store the strong-connected component
    vector<Record> *records;
    int number_scc;
    
    // Fills Stack with vertices (in increasing order of finishing 
    // times). The top element of stack has the maximum finishing  
    // time 
    void fillOrder(int v, bool visited[], stack<int> &Stack); 
  
    // A recursive function to print DFS starting from v 
    void DFSUtil(int v, bool visited[], vector<int> &vecs); 

    
public: 
    Graph(int V);
    Graph(string data_path); 
    void addEdge(int v, int w); 
  
    // The main function that finds and prints strongly connected 
    // components 
    void get_SCCs(); 
  
    // Function that returns reverse (or transpose) of this graph 
    Graph getTranspose(); 

    void printSCCs();
}; 
  
Graph::Graph(int V) 
{ 
    this->V = V; 
    this->number_scc = 0;
    adj = new list<int>[V]; 
} 

Graph::Graph(string data_path) {
    // read data from txt file
    cout << "Open txt file = " << data_path << endl;
    cout << "processing..." << endl;
    this->number_scc = 0;
    ifstream read_ifs;
    read_ifs.open(data_path, ios::in);
    if (!read_ifs.is_open()) {
        cout << "target file path :" << data_path << endl;
        cout << "data file open failed !" << endl;
        return ;
    }
    string buf;
    int number_records = 0;
    while (getline(read_ifs, buf)) {
        ++number_records;
    }
    cout <<  "The number of the records = " << number_records << endl;
    
    this->records = new vector<Record>(number_records, Record(0,0,0));
    read_ifs.clear();
    read_ifs.seekg(0, ios::beg);
    int cell = 0;
    uint32_t  count = 0;
    while ((read_ifs >> cell) && read_ifs.ignore()) {
        int loc = count % 3;
        int idx = count / 3;
        if (loc == 0) (*records)[idx].source_id = cell;
        else if (loc == 1) (*records)[idx].target_id = cell;
        else if (loc == 2) (*records)[idx].account = cell;
        count++;
    }
    read_ifs.close();

    unordered_set<uint32_t> all_ids;
    unordered_map<uint32_t, uint32_t> word_book; // {original_code, idx}
    for (int i = 0; i < number_records; ++i) {
        all_ids.insert((*records)[i].source_id);
        all_ids.insert((*records)[i].target_id);
    }
    int idx = 0;
    for (auto iter = all_ids.begin(); iter != all_ids.end(); ++iter) {
        if (word_book.find(*iter) == word_book.end())
            word_book[*iter] = idx++;
    }

    unordered_map<uint32_t,uint32_t> codebook; // {idx, original_code}
    for (auto iter = word_book.begin(); iter != word_book.end(); ++iter) {
        codebook[iter->second] = iter->first;
    }
    // codebook and wordbook
    
    this->V = codebook.size();
    adj = new list<int>[this->V]; 
    for (Record &rec:*records) {
        this->addEdge(word_book[rec.source_id], word_book[rec.target_id]);
    }
    cout << "Finished build the graph " << endl;
}
  
// A recursive function to print DFS starting from v 
void Graph::DFSUtil(int v, bool visited[], vector<int> &vecs) 
{ 
    // Mark the current node as visited and print it 
    visited[v] = true; 
    vecs.push_back(v);
    // cout << v << " ";
    // belonged to the same SCC
    // Recur for all the vertices adjacent to this vertex 
    list<int>::iterator i; 
    for (i = adj[v].begin(); i != adj[v].end(); ++i) 
        if (!visited[*i]) 
            DFSUtil(*i, visited, vecs); 
} 
  
Graph Graph::getTranspose() 
{ 
    Graph g(V); 
    for (int v = 0; v < V; v++) 
    { 
        // Recur for all the vertices adjacent to this vertex 
        list<int>::iterator i; 
        for(i = adj[v].begin(); i != adj[v].end(); ++i) 
        { 
            g.adj[*i].push_back(v); 
        } 
    } 
    return g; 
} 
  
void Graph::addEdge(int v, int w) 
{ 
    adj[v].push_back(w); // Add w to v’s list. 
} 
  
void Graph::fillOrder(int v, bool visited[], stack<int> &Stack) 
{ 
    // Mark the current node as visited and print it 
    visited[v] = true; 
  
    // Recur for all the vertices adjacent to this vertex 
    list<int>::iterator i; 
    for(i = adj[v].begin(); i != adj[v].end(); ++i) 
        if(!visited[*i]) 
            fillOrder(*i, visited, Stack); 
    
    // All vertices reachable from v are processed by now, push v  
    Stack.push(v); 
} 
  
// The main function that finds and prints all strongly connected  
// components 
void Graph::get_SCCs() 
{ 
    stack<int> Stack; 
  
    // Mark all the vertices as not visited (For first DFS) 
    bool *visited = new bool[V]; 
    for(int i = 0; i < V; i++) 
        visited[i] = false; 
    
    // Fill vertices in stack according to their finishing times 
    for(int i = 0; i < V; i++) 
        if(visited[i] == false) {
            fillOrder(i, visited, Stack); 
        }
    
    // Create a reversed graph 
    Graph gr = getTranspose(); 
    
    // Mark all the vertices as not visited (For second DFS) 
    for(int i = 0; i < V; i++) 
        visited[i] = false; 
  
    // Now process all vertices in order defined by Stack 
    while (Stack.empty() == false) 
    { 
        // Pop a vertex from stack 
        int v = Stack.top(); 
        Stack.pop(); 
  
        // Print Strongly connected component of the popped vertex 
        if (visited[v] == false) 
        {   
            vector<int> vecs;
            gr.DFSUtil(v, visited, vecs);
            this->scc.push_back(vecs);
            this->number_scc++;
        } 
    } 
    cout << number_scc << endl;
} 

void Graph::printSCCs() {
    for (int i = 0; i < number_scc; ++i) {
        vector<int> curr = scc[i];
        int size = curr.size();
        cout << "begin :" ;
        for (int j = 0; j < size; ++j)
            cout << curr[j] << " " ;
        cout << endl;
    }
    cout << "size of the scc = " << number_scc << endl;
}

// // Driver program to test above functions 
int main() 
{ 
    // Create a graph given in the above diagram 
    // Graph g(6); 
    // g.addEdge(0,3);
    // g.addEdge(2,0);
    // g.addEdge(0,1);
    // g.addEdge(3,2);
    // g.addEdge(3,1);
    // g.addEdge(4,0);
    // g.addEdge(5,3);
    string data_path = "../data/test_data.txt";
    Graph g(data_path);
  
    cout << "Following are strongly connected components in "
            "given graph \n"; 
    g.get_SCCs(); 
    g.printSCCs();
    return 0; 
} 