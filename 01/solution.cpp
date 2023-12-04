#include <iostream>
#include <fstream>
#include <string>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif
#define LINE_CAP 1024

std::string lines[LINE_CAP];
std::string numbers[9] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};
int size = 0;

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

int findNumber1(std::string line) {
    int forward = -1; 
    int backward = -1;
    for (int i = 0; i < line.length(); i++) {
        if (forward == -1 && isNumber(line[i])) {
            forward = line[i] - '0';
        }
        if (backward == -1 && isNumber(line[line.length() - 1 - i])) {
            backward = line[line.length() - 1 - i] - '0';
        }
    }
    return forward * 10 + backward;
}

int solution1() {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += findNumber1(lines[i]);
    }
    return sum;
}

int findNumber2(std::string line) {
    int forward = -1; 
    int backward = -1;
    int value_forward = -1;
    int value_backward = -1;
    for (int i = 0; i < line.length(); i++) {
        if (forward == -1 && isNumber(line[i])) {
            forward = i;
            value_forward = line[i] - '0';
        }
        if (backward == -1 && isNumber(line[line.length() - 1 - i])) {
            backward = line.length() - 1 - i;
            value_backward = line[line.length() - 1 - i] - '0';
        }
    }
    if (forward == -1) {
        forward = line.length();
    }
    for (int i = 0; i < 9; i++) {
        int start = 0;
        while (line.find(numbers[i], start) != std::string::npos) {
            int pos = line.find(numbers[i], start);
            if (pos < forward) {
                forward = pos;
                value_forward = i + 1;
            }
            if (pos > backward) {
                backward = pos;
                value_backward = i + 1;
            }
            start = pos + numbers[i].length();
        }
    }
    return value_forward * 10 + value_backward;
}

int solution2() {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += findNumber2(lines[i]);
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        lines[size] = line;
        size++;
    }
    file.close();
    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
