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
    outfile << cycles_size << endl;
    for (auto &v:records) {
        for (auto &vv:v)
            outfile << vv << " ";
        outfile << endl;
    }
    return 0;
} 




#endif