// Soha Hassoun
// Tufts University
// Code for COMP 150 GA to count cycles of a strongly connected graph.

// implements Johnson's algo.
// Donald B. Johnson: Finding All the Elementary Circuits of a Directed Graph.
// * SIAM Journal on Computing. Volumne 4, Nr. 1 (1975), pp. 77-84.<br><br>

// This is the portion of the code that assumed you already found a strongly
// connected component!!
// Do not try to use this code on a not storngly-connected component

#ifndef SIMPLE_CYCLE
#define SIMPLE_CYCLE

#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string.h>
#include <algorithm>
using namespace std;

const int maxlen = 8;

bool findCycles(int v, int s, vector<vector<int> >& adjList,  vector<bool>& blocked,  deque<int>& stackLike,  vector<vector<int> >&  B, vector<vector<int> >& cycles) ;

void unblock(int  node, vector<bool>&  blocked, vector<vector<int> >&  B);


void readAdjData (string s, vector<vector<int> >* & aList );

void displayVecOfVec (vector<vector<int> >&  cycles); 

int compute_complex_cycle(vector<vector<int>> *adjList, vector<vector<int>> &cycles){
    // blocked 
    vector<bool> blocked (adjList->size(), false);
    // stack- but use a deque because better than stack
    deque<int>  stackLike;
    // B_Fruitless is the book keeping needed to avoid fruitless searches.  It is
    // referred to as B in Johnson's original algorithm
    // initialize B
    vector<vector<int> >  B_Fruitless;

    for (int i=0; i < adjList->size(); i++ ) {
        vector<int>* k = new vector<int>;
        B_Fruitless.push_back(*k);
    }

    // loop to start new search from each node i
    for (int i=0; i< adjList->size()  ; i++ ) {
        // clear all book keeping
        for (int j =0; j< adjList->size(); j++) {
            blocked[j] = false;
            B_Fruitless[j].clear();
        }
        findCycles(i, i, *adjList,  blocked, stackLike, B_Fruitless, cycles) ;
    }
    return 0;
}


int compute_simple_cycle(vector<vector<int>> *adjList, vector<vector<int>> &cycles) {
    int numnode = adjList->size();
    for (int i = 0; i < numnode; ++i){
        // cout << (*adjList)[i].size() << endl;
        if ((*adjList)[i].size() > 1) return -1;
    }
    cycles.resize(1);
    cycles[0].push_back(0);
    for (int i = 1; i < numnode; ++i) {
        cycles[0].push_back((*adjList)[cycles[0][i-1]][0]);  
    }
    return 0;
}


int get_simple_cycle(vector<vector<int>> *adjList, 
                     vector<vector<int>> &cycles,
                     unordered_map<int,int> &codebook) {
    if (!adjList) return -1;
    // verify data is read correctly by printing size of adjList for each node
    // cout << "Adj List:\n";
    // displayVecOfVec (*adjList);
    
    if (adjList->size() < maxlen){
        int ret = compute_simple_cycle(adjList, cycles);
        if (ret == -1) {
            compute_complex_cycle(adjList, cycles);
        }
    } // the scc is small enough, just judge it whether the single cycle
    else {
        compute_complex_cycle(adjList, cycles);   
    }
    //TODO if the adjList too large, we need to seperate the SCC into 7-neighbor
    int n = cycles.size();
    for (int i = 0; i < n; ++i) {
        int m = cycles[i].size();
        for (int j = 0; j < m; ++j) {
            cycles[i][j] = codebook[cycles[i][j]];
        }
    }
    // cout << "Cycles:\n";
    // displayVecOfVec(cycles); 
    return 0;

}


bool findCycles(int v, int s, 
                vector<vector<int> > &adjList,
                vector<bool> &blocked,  
                deque<int> &stackLike,  
                vector<vector<int> > &B_Fruitless,
                vector<vector<int> > &cycles) {

    bool f = false;
    stackLike.push_front(v);  // insert like a stack:  so at the front
    blocked[v] = true;
  
    //   cout << "BLOCKED list from visiting node" << v << ":::\t" ;
    //   for (int i =0; i<adjList.size() ;i++) {
    //     cout << blocked[i] << "\t";
    //   }
    //   cout << endl;


    // explore all neighbours -- recursively 
    for (int i = 0; i < adjList[v].size(); i++) {
        int w =  adjList[v][i];
               
    // found cycle through ANY of my neighbours w.
        if (w == s) {
            // cout << "----- FOUND cycle with s=" << s <<  "and v=" << v << " and Size=" << stackLike.size() << ": " ;
            vector<int>* cycle = new vector<int>;
            for (int j = 0; j < stackLike.size(); j++) {
                cycle->push_back( stackLike.at (stackLike.size() - j  - 1) );
                // cout << stackLike.at(stackLike.size() - j -1) << " ";
            }
            cycles.push_back(*cycle);
            // cout << endl;
            f = true;
        } else if (!blocked[w]) {
            if (findCycles(w, s, adjList, blocked, stackLike, B_Fruitless, cycles)) {
                f = true;
            }
        }
    } // for


    if (f) {
        // cout << "F is true! just found a cycle, and unblock starting with " << v << endl; 
        unblock(v, blocked, B_Fruitless);
    } else {
    // cout << "--no cycles found for " << v << endl;

    // go through all my neighbors w.
    //  v is pushed on B_Fruitless[w].
    // looking at B_Fruitless[w] = [v1, v2, ..] , i know i can get from v1 to w, and v2 to w, etc. 
    // later, whenever i block w, i recursively unblock v1, and v2, and v.. 

        for (int i = 0; i < adjList[v].size(); i++) {
            int w = adjList[v][i];
            // mark B_Fruitless[w] to point to v.  This says that going from v to w lead to an unfruitful search.
            // later when w is found to particiate in a cycle, i'd better get rid of this false assupmtion about
            // w not leading to fruitful cycles.
            vector<int>::iterator it;
            it = find(B_Fruitless[w].begin(), B_Fruitless[w].end(), v);
            if (it == B_Fruitless[w].end()) {
                // cout << "Pushing v=" << v << "on B_Fruitless list for w=" << w << "\n";
                B_Fruitless[w].push_back(v);
            }
        }
    }
     // find v and remove it from stack
    deque<int>::iterator it;

    it = find(stackLike.begin(), stackLike.end(), v);
    if (it != stackLike.end() ) {
        stackLike.erase(it);
    } 
    return f;
} // bool  findCycles




    // Unblocks recursivly all blocked nodes, starting with a given node.
void unblock(int  node, vector<bool> & blocked, vector<vector<int> > & B_Fruitless)
{
    // state of B_Fruitless
    //   cout << "state of B_Fruitless\n";
        // displayVecOfVec (B_Fruitless);

    //   cout << "Unblocking " << node << endl; 
    blocked[node] = false;
    while (B_Fruitless[node].size() > 0) {
        int w = B_Fruitless[node][0];
        B_Fruitless[node].erase(find (B_Fruitless[node].begin(), B_Fruitless[node].end(), w) ) ;
        if (blocked[w]) {
            unblock(w, blocked, B_Fruitless);
        }
    }
}



// IO util

void readAdjData (string s, vector<vector<int> >*  & aList ) {

  ifstream file(s.c_str());
  char buff[100];

  file.getline(buff, 100, '\n');
  int numberOfNodes = atoi (buff);
  
  aList = new vector<vector<int> > [numberOfNodes];

  char* pch;
  while(!file.getline(buff, 100).eof()) {
    // buff has an adjacency list, seperated by tabs.

    vector<int>* currentAdjList  = new vector<int>;

    pch = strtok (buff, " \t");
    while (pch != NULL) {
    //      cout << "pushd " << pch << "\t";
      currentAdjList->push_back(atoi (pch));
      pch = strtok (NULL, " \t");
    }
    //    cout << "new line and size = " << currentAdjList->size() << "\n";
    aList->push_back(*currentAdjList);
  }
}


void displayVecOfVec (vector<vector<int> > &A) {
  vector< vector<int> >::iterator it;  
  int count = 0;
  for (it = A.begin(); 
       it != A.end(); 
       it++, count++) {

    vector<int> currentCycle = *it;
    cout << "[item " << count << " of size " << currentCycle.size() << "]\t";

    vector<int>::iterator nodeIter;

    for (nodeIter = currentCycle.begin(); nodeIter != currentCycle.end() ; nodeIter++) {
      cout << *nodeIter << "\t";
    }
    cout << "\n";
  }
  cout << "\n\n";
}

#endif