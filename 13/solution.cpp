#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <numeric>
#include <chrono>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif

#define CAPCITY 512

std::vector<std::vector<std::string>> input;

int findVerticalMirror(int t, int ignore = -1) {
    int left, right;
    for (int y = 0; y < input[t].size() - 1; ++y) {
        if (y == ignore) {
            continue;
        }
        left = y;
        right = y+1;
        bool isMirror = true;
        while (left >= 0 && right < input[t].size()) {
            for (int x = 0; x < input[t][left].length(); ++x) {
                if (input[t][left][x] != input[t][right][x]) {
                    isMirror = false;
                    break;
                }
            }
            if (!isMirror) {
                break;
            }
            left--;
            right++;
        }
        if (isMirror) {
            return y;
        }
    }

    return -1;
}

int findHorizontalMirror(int t, int ignore = -1) {
    int left, right;
    for (int x = 0; x < input[t][0].length() - 1; ++x) {
        if (x == ignore) {
            continue;
        }
        left = x;
        right = x + 1;
        bool isMirror = true;
        while (left >= 0 && right < input[t][0].length()) {
            for (int y = 0; y < input[t].size(); ++y) {
                if (input[t][y][left] != input[t][y][right]) {
                    isMirror = false;
                    break;
                }
            }
            if (!isMirror) {
                break;
            }
            left--;
            right++;
        }
        if (isMirror) {
            return x;
        }
    }

    return -1;
}

int solution1() {
    int sum = 0;
    for (int t = 0; t < input.size(); ++t) {
        int vert = findVerticalMirror(t);
        if (vert != -1) {
            sum += (vert + 1) * 100;
        }
        int hor = findHorizontalMirror(t);
        if (hor != -1) {
            sum += hor + 1;
        }
    }
    return sum;
}

int solution2() {
    int sum = 0;
    for (int t = 0; t < input.size(); ++t) {
        int org_value;
        bool org_vert;

        int temp = findVerticalMirror(t);
        if (temp != -1) {
            org_vert = true;
            org_value = temp;
        } else {
            temp = findHorizontalMirror(t);
            org_vert = false;
            org_value = temp;
        }

        int smallest = INT_MAX;
        bool isVert = false;
        for (int y = 0; y < input[t].size(); ++y) {
            for (int x = 0; x < input[t][y].length(); ++x) {
                if (input[t][y][x] == '.') {
                    input[t][y][x] = '#';
                    int vert = findVerticalMirror(t, org_vert ? org_value : -1);
                    if (vert != -1 && smallest > vert) {
                        smallest = vert;;
                        isVert = true;
                    } 
                    int hor = findHorizontalMirror(t, !org_vert ? org_value : -1);
                    if (hor != -1 && smallest > hor) {
                        smallest = hor;
                        isVert = false;
                    }
                    input[t][y][x] = '.';
                }
            }
        }
        if (isVert) {
            sum += (smallest + 1) * 100;
        } else {
            sum += smallest + 1;
        }
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    std::vector<std::string> temp;
    while (getline(file, line)) {
        if (line == "") {
            input.push_back(temp);
            temp.clear();
        } else {
            temp.push_back(line);
        }
    }
    input.push_back(temp);
    file.close();

    /*for (int t = 0; t < input.size(); ++t) {
        for (int i = 0; i < input[t].size(); ++i) {
            for (int j = 0; j < input[t][i].length(); ++j) {
                std::cout << input[t][i][j];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }*/

    /*for (int i = 0; i < i_height; ++i) {
        for (int j = 0; j < i_width; ++j) {
            std::cout << input[i * i_width + j];
        }
        std::cout << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
