#pragma once

#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

string fileContent(const char *path, bool verbose) {
    std::ifstream file(path);

    if (!file.is_open()) {
        if (verbose) {
            cout << "Failed to open the file: " << path << endl;
        }
        exit(3);
    }

    string startFileContent;
    string line;

    while (std::getline(file, line)) {
        startFileContent += line;
        startFileContent += '\n';
    }

    file.close();

    return startFileContent;
}