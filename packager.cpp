//
// Created by meiyixuan on 2021-12-14.
//
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "src/core/Utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    // check argument count
    if (argc != 2) {
        cout << "Need to specify the number of partial files" << endl;
        return 0;
    }
    int file_count = parse_int(argv[1], strlen(argv[1]));

    // open output
    ofstream output;
    output.open("1.ppm", ios::out | ios::trunc);

    // parse partial files
    for (int idx = 0; idx < file_count; ++idx) {
        // open input
        ifstream input;
        string input_file_name = to_string(idx) + ".partial";
        input.open(input_file_name.c_str());

        // parse
        string line;
        while(getline(input,line)){
            output << line << "\n";
        }

        // clean up
        input.close();
    }
    output.close();
}
