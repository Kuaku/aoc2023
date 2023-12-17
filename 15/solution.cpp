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

#define CAPCITY 10000
#define LENS_CAP 200
#define BOX_COUNT 256

struct Lens {
    std::string label;
    int value;
};

struct Box {
    Lens lenses[LENS_CAP];
    int lens_count = 0;
};

Box boxes[BOX_COUNT];

std::string instructions[CAPCITY];
int size = 0;

int calc_hash(std::string instruction) {
    int hash = 0;
    for (int i = 0; i < instruction.length(); ++i) {
        hash += int(instruction[i]);
        hash *= 17;
        hash %= 256;
    }

    return hash;
}

int solution1() {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += calc_hash(instructions[i]);
    }
    return sum;
}

void remove_from_box(int index, std::string label) {
    for (int i = 0; i < boxes[index].lens_count; ++i) {
        if (boxes[index].lenses[i].label == label) {
            for (int j = i; j < boxes[index].lens_count - 1; ++j) {
                boxes[index].lenses[j].label = boxes[index].lenses[j + 1].label;
                boxes[index].lenses[j].value = boxes[index].lenses[j + 1].value; 
            }
            boxes[index].lens_count--;

            return;
        }
    }
}

void change_or_add_to_box(int index, std::string label, int value) {
for (int i = 0; i < boxes[index].lens_count; ++i) {
        if (boxes[index].lenses[i].label == label) {
            boxes[index].lenses[i].value = value;
            return;
        }
    }
    boxes[index].lenses[boxes[index].lens_count].label = label;
    boxes[index].lenses[boxes[index].lens_count].value = value;
    boxes[index].lens_count++;
}

int solution2() {
    for (int i = 0; i < size; ++i) {
        int j = 0;
        while (instructions[i].length() > j && instructions[i][j] >= 'a' && instructions[i][j] <= 'z') {
            j++;
        }
        std::string label = instructions[i].substr(0, j);
        int index = calc_hash(label);
        if (instructions[i][j] == '-') {
            remove_from_box(index, label);
            continue;
        }
        int value = stoi(instructions[i].substr(j + 1));
        change_or_add_to_box(index, label, value);
    }
    int sum = 0;
    for (int i = 0; i < BOX_COUNT; ++i) {
        for (int j = 0; j < boxes[i].lens_count; ++j) {
            sum += (i + 1) * (j + 1) * boxes[i].lenses[j].value;
        }
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    getline(file, line);
    int start = 0;
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == ',') {
            instructions[size] = line.substr(start, i - start);
            size++; 
            start = i + 1;
        }
    }
    instructions[size] = line.substr(start, line.length() - start);
    size++; 
    file.close();

    /*std::cout << "Instructions: ";
    for (int i = 0; i < size; ++i) {
        std::cout << instructions[i] << " ";
    }
    std::cout << std::endl;*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
