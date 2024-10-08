#ifndef MERGEFILES_H
#define MERGEFILES_H

#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// A time node contains name, time , whole line
// name and time are used to compare the order
// the line is used to write out
struct tnode {
    string name;
    string time;
    string line;
};

/*
 return true if node1 < node2
 first compare time stemps,
 if two time stemps are equal
 then compare two asset names.
*/
bool tnode_less(tnode tnode1, tnode tnode2);

/*
 Go throw all files and find smollest time stemps
 as a beginning of time.
*/
string min_time(vector<ifstream *> files);

/*
 For a given milliseconds range
 return the time stepms of current time + milliseconds.
*/
string next_time(string curret_T, int delta_T);

// create tnodes
tnode create_node(string aline, string name);

/*
 tnode is an entry of data from a file.
 This fuction will create sorted data entries
 in vector format.
 Inputs arguments:
    ifiles: vector of all input file streams
    asset_names: vector of asset names, corresponds to input files.
    t_end: time stemsp (string), read from ifstream untill
           the time stepms reach the t_end.
*/
vector<tnode> create_tnodes(vector<ifstream *> ifiles,
                            vector<string> asset_names, string t_end);

#endif