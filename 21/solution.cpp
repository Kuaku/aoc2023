#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <numeric>
#include <chrono>

#if 0
    #define FILE_PATH "example.txt"
    #define STEPS 10
#else
    #define FILE_PATH "input.txt"
    #define STEPS 64
#endif
#define CAPCITY 512
struct Position {
    int x, y;
};

std::string fill_to(int v, int l) {
    std::string out = std::to_string(v);
    while (out.length() < l) {
        out = "0" + out;
    }
    return out;
}
enum Enviroment {
    FREE = 0,
    ROCK = 1
};
Enviroment field[CAPCITY * CAPCITY];
int start_x = 0, start_y = 0;
int height = 0, width = 0;
bool visited[CAPCITY * CAPCITY];
bool swap[CAPCITY * CAPCITY];

#define PADDING 10
Enviroment field2[CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1)];
int start_x2 = 0, start_y2 = 0;
int height2 = 0, width2 = 0;
int total_height2 = 0, total_width2 = 0;
int visited_tracker[CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1)];
bool visited2[CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1)];
bool swap2[CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1)];
int cell_sizes[(2 * PADDING + 1) * (PADDING * 2 + 1) * CAPCITY];

int p(int x, int y) {
    return x + y * width;
}

int p2(int x, int y) {
    return x + y * total_width2;
}

Position dirs[4] = {{0, 1},{0, -1},{1, 0},{-1, 0}};

bool is_in_field(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool is_in_field2(int x, int y) {
    return x >= 0 && x < total_width2 && y >= 0 && y < total_height2;
}

void mark_neighbours(int x, int y) {
    for (int i = 0; i < 4; ++i) {
        if (is_in_field(x + dirs[i].x, y + dirs[i].y) && field[p(x + dirs[i].x, y + dirs[i].y)] == FREE) {
            swap[p(x + dirs[i].x, y + dirs[i].y)] = true;
        }
    }
}

void mark_neighbours2(int x, int y, int steps) {
    for (int i = 0; i < 4; ++i) {
        if (is_in_field2(x + dirs[i].x, y + dirs[i].y) && field2[p2(x + dirs[i].x, y + dirs[i].y)] == FREE) {
            swap2[p2(x + dirs[i].x, y + dirs[i].y)] = true;
            if (visited_tracker[p2(x + dirs[i].x, y + dirs[i].y)] == -1) {
                visited_tracker[p2(x + dirs[i].x, y + dirs[i].y)] = steps;
            }
        }
    }
}

void swap_arrays() {
    std::memcpy(visited, swap, width * height);
    std::memset(swap, 0, width * height);
}

void swap_arrays2() {
    std::memcpy(visited2, swap2, CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1));
    std::memset(swap2, 0, CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1));
}

long long solution1() {
    visited[p(start_x, start_y)] = true;
    for (int i = 0; i < STEPS; ++i) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (visited[p(x, y)]) {
                    mark_neighbours(x, y);
                }
            }
        }
        swap_arrays();
    }
    long long sum = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (visited[p(x, y)]) {
                sum++;
            }
        }
    }
    return sum;
}

long long solution2() {
    long long sum = 0;
    int f0 = -1;
    int f1 = -1;
    int f2 = -1;
    visited2[p2(start_x2, start_y2)] = true;
    for (int i = 0; i < 1000; ++i) {
        for (int y = 0; y < total_height2; ++y) {
            for (int x = 0; x < total_width2; ++x) {
                if (visited2[p2(x, y)]) {
                    mark_neighbours2(x, y, i);
                }
            }
        }
        swap_arrays2();
        sum = 0;
        for (int y = 0; y < total_height2; ++y) {
            for (int x = 0; x < total_width2; ++x) {
                if (visited_tracker[p2(x, y)] % 2 == i % 2) {
                    sum++;
                }
            }
        }
        int small_sum = 0;
        if ((i + 1) == start_x) {
            f0 = sum;
        }
        if ((i + 1) == start_x + width) {
            f1 = sum;
        }
        if ((i + 1) == start_x + width * 2) {
            f2 = sum;
            break;
        }
    }
    long long c = f0;
    long long a = (f2 - 2 * f1 + f0) / 2;
    long long b = f1 - f0 - a;
    std::cout << f0 << " " << f1 << " " << f2 << std::endl;
    std::cout << "a: " << a << ",b: " << b << ",c: " << c << std::endl;
    long long x = (26501365 - start_x) / width;
    return a * x * x + b * x + c;
}


int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        width = line.length();
        width2 = line.length();
        total_width2 = width2 * PADDING * 2 + width2;
        for (int i = 0; i < width; ++i) {
            if (line[i] == '.') {
                field[p(i, height)] = FREE;
            } else if (line[i] == 'S') {
                field[p(i, height)] = FREE;
                start_x = i;
                start_y = height;
                start_x2 = width * PADDING + i;
            } else {
                field[p(i, height)] = ROCK;
            }
        }
        height2++;
        height++;
    }
    start_y2 = height2 * PADDING + start_y;
    total_height2 = height2 * PADDING * 2 + height2;
    file.close();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int i = 0; i < PADDING * 2 + 1; ++i) {
                for (int j = 0; j < PADDING * 2 + 1; ++j) {
                    field2[p2((j * width) + x, (i * height) + y)] = field[p(x, y)];
                }
            }
        }
    }
    std::memset(visited_tracker, -1, (sizeof(int)) * CAPCITY * CAPCITY * (2 * PADDING + 1) * (2 * PADDING + 1));

    /*for (int y = 0; y < total_height2; ++y) {
        for (int x = 0; x < total_width2; ++x) {
            if (start_x2 == x && start_y2 == y) {
                std::cout << "S";
            } else if (field2[p2(x, y)] == FREE) {
                std::cout << ".";
            } else {
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
