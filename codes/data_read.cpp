#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
using namespace std;

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


int main()
{
    string data_path = "../data/test_data.txt";
    cout << data_path << endl;
    ifstream read_ifs;
    read_ifs.open(data_path, ios::in);
    if (!read_ifs.is_open()) {
        cout << "target file path :" << data_path << endl;
        cout << "data file open failed !" << endl;
        return 1;
    }
    string buf;
    int number_records = 0;
    while (getline(read_ifs, buf)) {
        ++number_records;
    }
    cout << number_records << endl;
    vector<Record> records(number_records, Record(0,0,0));
    read_ifs.clear();
    read_ifs.seekg(0, ios::beg);
    int cell = 0;
    uint32_t  count = 0;
    while ((read_ifs >> cell) && read_ifs.ignore()) {
        int loc = count % 3;
        int idx = count / 3;
        if (loc == 0) records[idx].source_id = cell;
        else if (loc == 1) records[idx].target_id = cell;
        else if (loc == 2) records[idx].account = cell;
        count++;
    }
    read_ifs.close();
    // prepare the data
    
    
    unordered_set<uint32_t> all_ids;
    unordered_map<uint32_t, uint32_t> word_book; // {original_code, idx}
    for (int i = 0; i < number_records; ++i) {
        all_ids.insert(records[i].source_id);
        all_ids.insert(records[i].target_id);
    }
    int idx = 0;
    for (auto iter = all_ids.begin(); iter != all_ids.end(); ++iter) {
        if (word_book.find(*iter) == word_book.end())
            word_book[*iter] = idx++;
    }
    
    // unordered_map<uint32_t,uint32_t> codebook; // {idx, original_code}
    // for (auto iter = word_book.begin(); iter != word_book.end(); ++iter) {
    //     codebook[iter->second] = iter->first;
    // }
    // int valid_ids_num = codebook.size();
    // codebook and wordbook
    
    // unordered_map<int, vector<Record>> director; // {source_id:[sid,tid,amount]...}
    
    // for (int i = 0; i < number_records; ++i) {
    //     Record rec = records[i];
    //     if (director.find(rec.source_id) == director.end()) {
    //         director[rec.source_id] = vector<Record>{rec};
    //     }
    //     else 
    //         director[rec.source_id].emplace_back(rec);
    // }

    // // dfs 
    // for (int each_node = 0; each_node < valid_ids_num; ++each_node) {
    //     cout << codebook[each_node] << endl;
    //     int curr_node = codebook[each_node];
    //     vector<Record> direction = director[curr_node];
    //     stack<pair<int,int>> progstack;
    //     for (auto &v:direction) {
    //         progstack.push({curr_node, v.target_id});
    //     }

    //     while (!progstack.empty()) {
            
    //     }


    // }


    return 0;
}



















