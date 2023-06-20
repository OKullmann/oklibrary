#include <string>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> readFileToVector(const std::string& filename) {
    std::ifstream source;
    source.open(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(source, line)) {
        lines.push_back(line);
    }
    return lines;
}

void displayVector(const std::vector<std::string> v) {
    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
}

int skipCommentLines(const std::vector<std::string> v) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i][0] != 'c') {
            return i;
        }
    }

    return -1;
}

std::string readFileReturnFirstLineMatrix(const std::string& filename) {
    std::ifstream source;
    source.open(filename);
    std::string line;
    while (std::getline(source, line)) {
        if (line[0] == 'p') {
            break;
        }
    }
    std::string firstLine = std::getline(source, line);
    return firstLine;
}

int main(int argc,  char **argv) {
    std::string filename(argv[1]);
    // std::vector<std::string> lines = readFileToVector(filename);
    // displayVector(lines);
    // int problemLine = skipCommentLines(lines);
    std::string firstLine = readFileReturnFirstLineMatrix(filename)
    if (firstLine[0] == 'a') {
        return 0;
    }
    
    std::cout << "No global variable found.";
}