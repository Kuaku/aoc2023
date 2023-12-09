#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <numeric>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif
std::vector<std::vector<int>> numbers;

bool allZeros(std::vector<int> line) {
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] != 0) {
            return false;
        }
    }
    return true;
}

int solution1() {
    int sum = 0;
    
    for (int i = 0; i < numbers.size(); ++i) {
        std::vector<std::vector<int>> lines;
        lines.push_back(numbers[i]);

        while (!allZeros(lines[lines.size() - 1])) {
            std::vector<int> line;
            for (int j = 1; j < lines[lines.size() - 1].size(); ++j) {
                line.push_back(lines[lines.size() - 1][j] - lines[lines.size() - 1][j - 1]);
            }
            lines.push_back(line);
        }
        lines[lines.size() - 1].push_back(0);
        for (int j = lines.size() - 2; j >= 0; --j) {
            lines[j].push_back(lines[j + 1][lines[j + 1].size() - 1] + lines[j][lines[j].size() - 1]);
        }
        sum += lines[0][lines[0].size() - 1];
    }

    return sum;
}

long long solution2() {
    int sum = 0;
    
    for (int i = 0; i < numbers.size(); ++i) {
        std::vector<std::vector<int>> lines;
        lines.push_back(numbers[i]);

        while (!allZeros(lines[lines.size() - 1])) {
            std::vector<int> line;
            for (int j = 1; j < lines[lines.size() - 1].size(); ++j) {
                line.push_back(lines[lines.size() - 1][j] - lines[lines.size() - 1][j - 1]);
            }
            lines.push_back(line);
        }
        lines[lines.size() - 1].push_back(0);
        for (int j = lines.size() - 2; j >= 0; --j) {
            lines[j].push_back(lines[j][0] - lines[j + 1][lines[j + 1].size() - 1]);
        }
        sum += lines[0][lines[0].size() - 1];
    }

    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    int number;
    size_t number_size;
    while (getline(file, line)) {
        std::vector<int> number_line;
        while (line.length() != 0) {
            number = stoi(line, &number_size);
            line = line.substr(number_size);
            number_line.push_back(number);
        }
        numbers.push_back(number_line);
    }
    file.close();

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
