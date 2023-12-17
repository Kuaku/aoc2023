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

#define CAPCITY 200
#define CACHE_CAPACITY 200


char field[CAPCITY * CAPCITY];
int height = 0, width = 0;

char working_field[CAPCITY * CAPCITY];
char field_cache[CACHE_CAPACITY * CAPCITY * CAPCITY];
int cycle_cache[CACHE_CAPACITY];
int cycle_index = 0;
int cacheSize = 0;


int positionIn1DArray(int x, int y) {
    return x + y * width;
}

void putInCache() {
    if (cacheSize == CACHE_CAPACITY) {
        return;
    }
    std::memcpy(field_cache + cacheSize * (width * height), working_field, width * height);
    cycle_cache[cacheSize] = cycle_index;
    cacheSize++;
}

void restoreCache(int index) {
    std::memcpy(working_field, field_cache + index * (width * height), width * height);
}

int findInCache() {
    for (int i = 0; i < cacheSize; ++i) {
        if (std::memcmp(field_cache + i * (height * width), working_field, height * width) == 0) {
            return i;
        }
    }
    return -1;
}

void reset_working_field() {    
    std::memcpy(working_field, field, width * height);
}

void move_north() {
    bool change;
    do {
        change = false;
        for (int y = 1; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (working_field[positionIn1DArray(x, y)] == 'O' && working_field[positionIn1DArray(x, y - 1)] == '.') {
                    working_field[positionIn1DArray(x, y - 1)] = 'O';
                    working_field[positionIn1DArray(x, y)] = '.';
                    change = true;
                }
            }
        }
    } while (change);
}
void move_south() {
    bool change;
    do {
        change = false;
        for (int y = height - 2; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                if (working_field[positionIn1DArray(x, y)] == 'O' && working_field[positionIn1DArray(x, y + 1)] == '.') {
                    working_field[positionIn1DArray(x, y + 1)] = 'O';
                    working_field[positionIn1DArray(x, y)] = '.';
                    change = true;
                }
            }
        }
    } while (change);
}

void move_west() {
    bool change;
    do {
        change = false;
        for (int x = 1; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (working_field[positionIn1DArray(x, y)] == 'O' && working_field[positionIn1DArray(x - 1, y)] == '.') {
                    working_field[positionIn1DArray(x - 1, y)] = 'O';
                    working_field[positionIn1DArray(x, y)] = '.';
                    change = true;
                }
            }
        }
    } while (change);
}

void move_east() {
    bool change;
    do {
        change = false;
        for (int x = width - 2; x >= 0; --x) {
            for (int y = 0; y < height; ++y) {
                if (working_field[positionIn1DArray(x, y)] == 'O' && working_field[positionIn1DArray(x + 1, y)] == '.') {
                    working_field[positionIn1DArray(x + 1, y)] = 'O';
                    working_field[positionIn1DArray(x, y)] = '.';
                    change = true;
                }
            }
        }
    } while (change);
}

void cycle() {
    move_north();
    move_west();
    move_south();
    move_east();
}

int calculate_weight() {
    int sum = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (working_field[positionIn1DArray(x, y)] == 'O') {
                sum += height - y;
            }
        }
    }
    return sum;
}

int solution1() {
    move_north();
    return calculate_weight();
}

int solution2() {
    int cacheIndex = -1; 

    for (int i = 0; i < 1000000000; i++) {
        cacheIndex = findInCache();
        if (cacheIndex == -1) {
            putInCache();
        } else {
            break;
        }
        cycle();
        cycle_index++;
    }
    int index = (1000000000 - cycle_index) % (cycle_index - cycle_cache[cacheIndex]) + cycle_cache[cacheIndex];
    restoreCache(index);

    return calculate_weight();
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        width = line.length();
        for (int i = 0; i < line.length(); i++) {
            field[positionIn1DArray(i, height)] = line[i];
            working_field[positionIn1DArray(i, height)] = line[i];
        }
        height++;
    }
    file.close();

    /*for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << working_field[y][x];
        }
        std::cout << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    reset_working_field();
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
