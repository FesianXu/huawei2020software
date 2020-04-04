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
#include "./johnsons_circle.hpp"
#include "./scc.hpp"
#include <chrono>
using namespace std; 

// // Driver program to test above functions 
int main() 
{ 
    auto start = std::chrono::steady_clock::now();
    auto start_t = std::chrono::steady_clock::now();
    string data_path = "../data/test_data.txt";
    Graph g(data_path);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start; // std::micro 表示以微秒为时间单位
    std::cout<< "reading time: "  << elapsed.count() << "us" << std::endl;

    start = std::chrono::steady_clock::now();
    g.get_SCCs(); 
    // g.printSCCs();

    end = std::chrono::steady_clock::now();
    elapsed = end - start; // std::micro 表示以微秒为时间单位
    std::cout<< "scc time: "  << elapsed.count() << "us" << std::endl;

    start = std::chrono::steady_clock::now();
    
    int number_scc = g.get_number_scc();
    
    // for (int i = number_scc-1; i >= 0; --i) {
    for (int i = 0; i < number_scc; ++i) {
        unordered_map<int,int> codebook;
        vector<vector<int>> *ret = g.get_adjList(i, 2, 7,codebook);
        if (ret != nullptr) {
            vector<vector<int>> cycle;
            get_simple_cycle(ret, cycle, codebook);
        }
    }
    cout << "number of scc" << " " ;
    cout << number_scc << endl;
    
    end = std::chrono::steady_clock::now();
    elapsed = end - start; // std::micro 表示以微秒为时间单位
    std::cout<< "cycle time: "  << elapsed.count() << "us" << std::endl;

    auto end_t = std::chrono::steady_clock::now();
    elapsed = end_t - start_t; // std::micro 表示以微秒为时间单位
    std::cout<< "Total time: "  << elapsed.count() << "us" << std::endl;

    return 0; 
} 