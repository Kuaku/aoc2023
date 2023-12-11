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
char input[CAPCITY * CAPCITY];
int i_width = 0, i_height = 0;
char row_expanded[CAPCITY * CAPCITY];
int r_width = 0, r_height = 0;
char all_expanded[CAPCITY * CAPCITY];
int a_width = 0, a_height = 0;

typedef struct
{
    int x, y;
} Point;

int solution1() {
    r_width = i_width;
    for (int i = 0; i < i_height; ++i) {
        bool isEmpty = true;
        for (int j = 0; j < i_width; ++j) {
            if (input[i * i_width + j] != '.') {
                isEmpty = false;
            }
            row_expanded[r_height * r_width + j] = input[i * i_width + j];
        }
        r_height++;
        if (isEmpty) {
            for (int j = 0; j < i_width; ++j) {
                if (input[i * i_width + j] != '.') {
                    isEmpty = false;
                }
                row_expanded[r_height * r_width + j] = input[i * i_width + j];
            }
            r_height++;
        }
    }
    int count = 0;
    for (int x = 0; x < r_width; ++x) {
        bool isEmpty = true;
        for (int y = 0; y < r_height; ++y) {
            if (row_expanded[x + y * r_width] != '.') {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) {
            count++;
        }
    }
    a_height = r_height;
    a_width = r_width + count;
    int x2 = 0;
    for (int x = 0; x < r_width; ++x) {
        bool isEmpty = true;
        for (int y = 0; y < r_height; ++y) {
            if (row_expanded[x + y * r_width] != '.') {
                isEmpty = false;
            }
            all_expanded[x2 + y * a_width] = row_expanded[x + y * r_width];
        }
        x2++;
        if (isEmpty) {
            for (int y = 0; y < r_height; ++y) {
                all_expanded[x2 + y * a_width] = row_expanded[x + y * r_width];
            }
            x2++;
        }
    }
    for (int i = 0; i < a_height; ++i) {
        for (int j = 0; j < a_width; ++j) {
            std::cout << all_expanded[i * a_width + j];
        }
        std::cout << std::endl;
    }



    std::vector<Point> points;
    for (int i = 0; i < a_height; ++i) {
        for (int j = 0; j < a_width; ++j) {
            if (all_expanded[i * a_width + j] == '#') {
                points.push_back({j, i});
            }
        }
    }


    int sum = 0;
    for (int i = 0; i < points.size(); ++i) {
        Point p1 = points[i];
        for (int j = i + 1; j < points.size(); ++j) {
            Point p2 = points[j];
            int distance = std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
            sum += distance;
        }
    }

    return sum;
}

std::vector<int> x_expansions;
std::vector<int> y_expansions; 

int count_expansions(Point p1, Point p2) {
    int x1 = std::min(p1.x, p2.x);
    int x2 = p1.x == x1 ? p2.x : p1.x;
    int y1 = std::min(p1.y, p2.y);
    int y2 = p1.y == y1 ? p2.y : p1.y;

    int sum = 0;

    for (int i = 0; i < x_expansions.size(); ++i) {
        int x = x_expansions[i];
        if (x > x1 && x < x2) {
            sum++;
        }
    }

    for (int i = 0; i < y_expansions.size(); ++i) {
        int y = y_expansions[i];
        if (y > y1 && y < y2) {
            sum++;
        }
    }

    return sum;
}

long long solution2() {
    
    for (int x = 0; x < i_width; ++x) {
        bool isEmpty = true;
        for (int y = 0; y < i_height; ++y) {
            if (input[x + y * i_width] != '.') {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) {
            x_expansions.push_back(x);
        }
    }

    
    for (int y = 0; y < i_height; ++y) {
        bool isEmpty = true;
        for (int x = 0; x < i_width; ++x) {
            if (input[x + y * i_width] != '.') {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) {
            y_expansions.push_back(y);
        }
    }

    std::vector<Point> points;

    for (int x = 0; x < i_width; ++x) {
        for (int y = 0; y < i_height; ++y) {
            if (input[x + y * i_width] == '#') {
                points.push_back({x, y});
            }     
        }
    }

    
    long long sum = 0;
    for (int i = 0; i < points.size(); ++i) {
        Point p1 = points[i];
        for (int j = i + 1; j < points.size(); ++j) {
            Point p2 = points[j];
            long long distance = std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
            int expansions = count_expansions(p1, p2);
            //std::cout << expansions << std::endl; 
            sum += distance + expansions * 999999;
        }
    }

    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        i_width = line.size();
        for (int i = 0; i < line.size(); ++i) {
            input[i_height * i_width + i] = line[i];
        }
        i_height++;
    }
    file.close();

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
