#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector> 
#include <string> 
#include <fstream> 
#include <iostream>

using namespace std;

int write_records(string path, vector<vector<int>> &records) {
    ofstream outfile(path, ios::out);
    int cycles_size = records.size();
    outfile << cycles_size << "\r\n";
    for (auto &v:records) {
        int n = v.size();
        for (int i = 0; i < n-1; ++i)
            outfile << v[i] << ",";
        outfile << v[n-1];
        outfile << "\r\n" ;
    }
    return 0;
} 




#endif