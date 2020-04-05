#ifndef MERGE_CYCLES_HPP
#define MERGE_CYCLES_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
using namespace std;

int get_merge_cycles(vector<vector<int>> &cycle, vector<vector<int>> &merged_cycles) {
    int ncycle = cycle.size();
    vector<int> merged;
    bool flag = false;
    for (int i = 0; i < ncycle; ++i) {
        vector<int> curr = cycle[i];
        flag = false;
        int currsize = curr.size();
        int minloc = std::distance(curr.begin(), std::min_element(curr.begin(), curr.end()));
        merged.resize(currsize);
        for (int j = 0; j < currsize; ++j) {
            merged[j] = curr[(j+minloc) % currsize];
        }
        for (auto &v:merged_cycles)
            if (merged == v) {
                flag = true;
                break;
            }
        if (flag) continue;
        merged_cycles.push_back(merged);
    }
    return 0;
}


#endif