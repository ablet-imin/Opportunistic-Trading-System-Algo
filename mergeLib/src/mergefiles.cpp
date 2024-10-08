#include "mergefiles/mergefiles.h"
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool tnode_less(tnode tnode1, tnode tnode2) {
    if (tnode1.time < tnode2.time) {
        return true;
    } else if (tnode1.time > tnode2.time) {
        return false;
    } else { // equal
        if (tnode1.name < tnode2.name) {
            return true; // node 1 < node 2
        }
        return false;
    }
}

string min_time(vector<ifstream *> files) {
    int file_len = files.size();
    if (file_len < 1)
        return "";

    string min_time = "";
    string current_time = "";
    ifstream *ifs;
    int pos;
    for (int i = 0; i < file_len; i++) {

        ifstream *ifs = files[i];

        /*
         Read the first entry from each file
         and select the min time steps
        */
        pos = ifs->tellg();
        getline(*ifs, current_time, ',');
        ifs->seekg(pos, ifs->beg);

        if (min_time.empty()) {
            min_time = current_time;
            continue;
        }
        if (current_time < min_time) {
            // update min time
            min_time = current_time;
        }
    }
    return min_time;
}

string next_time(string curret_T, int delta_T) {

    // From time stemps to milliseconds
    tm c_time = {};
    istringstream sst(curret_T);
    sst >> get_time(&c_time, "%Y-%m-%d %H:%M:%S");

    long long msecond = 0;
    sst.ignore(1);  // ignore .
    sst >> msecond; // digites after .

    auto tp = chrono::system_clock::from_time_t(mktime(&c_time));
    long long total_msecond =
        chrono::duration_cast<chrono::milliseconds>(tp.time_since_epoch())
            .count();
    total_msecond += msecond + delta_T; // end time in millisecond.

    // From milliseconds to string
    chrono::milliseconds duration(total_msecond);
    chrono::time_point<std::chrono::system_clock> timePoint(duration);

    // Convert to tt (seconds since epoch)
    time_t tt = chrono::system_clock::to_time_t(timePoint);

    // Create a tm struct from tt
    tm *tm = localtime(&tt);

    // Format the string
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);

    // Append milliseconds
    string result(buffer);
    result += ".";
    result += std::to_string(total_msecond % 1000);

    return result;
}

// create tnodes
tnode create_node(string aline, string name) {
    tnode tn;
    tn.name = name;
    tn.line = aline;
    stringstream sline(aline);
    getline(sline, tn.time, ',');

    return tn;
}
vector<tnode> create_tnodes(vector<ifstream *> ifiles,
                            vector<string> asset_names, string t_end) {
    vector<tnode> ordered_tnodes;
    int file_len = ifiles.size();
    ifstream *ifs;
    for (int i = 0; i < file_len; i++) {
        string line;
        ifs = ifiles[i];

        vector<tnode> tmp_nodes;
        int pos = 0;

        while (!ifs->eof()) {
            // Store the current position in the input stream
            pos = ifs->tellg();
            // if(ifs->eof()) break;

            if (!getline(*ifs, line))
                break;
            /* assume data is clean
               so we don't have to check for
               every entries.
             */
            // if (line.empty()) continue;

            // skip the ... in test files
            // otherwise this line can be removed
            // to speed up
            if (!isdigit(line[0]))
                continue;

            tnode new_node = create_node(line, asset_names[i]);

            if (new_node.time > t_end) { // read untill reach the end_time
                // Go back to the previous position
                ifs->seekg(pos, ifs->beg);

                break; // stop reading and go to next file
            }

            tmp_nodes.push_back(new_node);
            // How insert new node to the list?
        }

        // add to the main list in time ordered way
        if (ordered_tnodes.empty()) {
            ordered_tnodes = vector<tnode>(tmp_nodes);
            continue;
        }

        int ibegin = 0; // find a starting place for sorting
        while (ibegin < ordered_tnodes.size()) {
            if (tnode_less(ordered_tnodes[ibegin], tmp_nodes[0])) {
                ibegin++;
            } else
                break;
        }
        // extend whole list.
        for (int i = 0; i < tmp_nodes.size(); i++) {
            ordered_tnodes.push_back(tmp_nodes[i]);
        }

        // sort the last part
        sort(ordered_tnodes.begin() + ibegin, ordered_tnodes.end(), tnode_less);
    }
    return ordered_tnodes;
}
