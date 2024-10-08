
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "mergefiles/mergefiles.h"

using namespace std;

void help() {
    cout << "Usage:" << endl;
    cout << "  ./build/mergeApp/mergeApp -f CSCO.txt,MSFT.txt -d 5 -o "
            "output_file.txt"
         << endl;
    cout << "    -f INFILE1,INFILE2,..." << endl;
    cout << "    -d time intervals in milliseconds,  default 5," << endl;
    cout << "    -o OUTPUTFILE, default output_file.txt" << endl;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        help();
        return 0;
    }

    // input file list
    string infiles = "";
    string ofile_name = "output_file.txt";
    int delta_time = 5;
    string delta_time_ss = "";
    char opt;
    while ((opt = getopt(argc, argv, ":f:d:o:h")) != -1) {
        switch (opt) {
            case 'f':
                /* get file name list */
                infiles = optarg;
                break;

            case 'd':
                /* get file name list */
                delta_time = stoi(optarg);
                break;
            case 'o':
                /* get file name list */
                ofile_name = optarg;
                break;
            case 'h':
                help();
                return 0;

            case '?':
                cout << "unknown option: " << optopt << endl;
                return 1;
            default:
                help();
                abort();
        }
    }

    stringstream files_ss(infiles);
    vector<string> file_list;
    string file_path;

    while (getline(files_ss, file_path, ',')) {
        file_list.push_back(file_path);
    }
    sort(file_list.begin(), file_list.end());

    // ifstream
    vector<string> asset_names;
    string asset_name;
    string header_line;
    vector<ifstream *> inputFiles_stream;
    for (int i = 0; i < file_list.size(); i++) {
        asset_name = file_list[i];
        std::size_t lastslash = asset_name.find_last_of("/");
        // If a '.' is found, remove the extension
        if (lastslash != std::string::npos) {
            asset_name = asset_name.substr(0, lastslash);
        }

        std::size_t lastDot = asset_name.find_last_of(".");
        // If a '.' is found, remove the extension
        if (lastDot != std::string::npos) {
            asset_name = asset_name.substr(0, lastDot);
        }
        asset_names.push_back(asset_name);

        ifstream *inf = new ifstream(file_list[i]);
        // skip headers
        getline(*inf, header_line);
        inputFiles_stream.push_back(inf);
    }
    string T_BEGIN = min_time(inputFiles_stream);

    // wrtie to output file
    ofstream ofile(ofile_name);
    if (!ofile.is_open()) {
        cout << "Error opening file!" << endl;
    }

    ofile << "Symbol"
          << ", " << header_line << endl; // include headers

    string T_END = T_BEGIN;
    vector<tnode> sorted_nodes;
    while (true) {
        sorted_nodes.clear();
        T_END = next_time(T_END, delta_time);

        // merge files
        sorted_nodes = create_tnodes(inputFiles_stream, asset_names, T_END);
        if (sorted_nodes.empty()) {
            // close all input files.
            bool isEndOfFile = true;
            for (int i = 0; i < inputFiles_stream.size(); i++) {
                if (!inputFiles_stream.at(i)->eof()) {
                    isEndOfFile = false;
                    break;
                }
            }
            if (isEndOfFile)
                break; // no more data left
            T_END = next_time(T_END, 3 * delta_time);
        }

        // store the sorted data to output file
        for (const auto &inode : sorted_nodes) {
            ofile << inode.name << ", " << inode.line << endl;
        }
    }
    // close the output file.
    ofile.close();

    // close all input files.
    for (int i = 0; i < inputFiles_stream.size(); i++) {
        inputFiles_stream.at(i)->close();
    }

    return 0;
}
