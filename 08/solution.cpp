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
#define CAPACITY 1024
struct Node {
    std::string ident;
    int l = -1, r = -1;
};

struct Way {
    int position;
    int steps = 0;
    bool finished = false;
};

std::string instruction;
Node network[CAPACITY];
int network_size = 0;

int findNode(std::string ident) {
    for (int i = 0; i < network_size; ++i) {
        if (network[i].ident == ident) 
            return i;
    }
    return -1;
}

int addOrFindNode(std::string ident) {
    int index = findNode(ident);
    if (index == -1) {
        index = network_size;
        network[index].ident = ident;
        network_size++;
    }
    return index;
}

void parseNode(std::string line) {
    std::string ident = line.substr(0, 3);
    line = line.substr(7);
    int index = addOrFindNode(ident);
    
    ident = line.substr(0, 3);
    int index2 = addOrFindNode(ident);
    line = line.substr(5);
    network[index].l = index2;

    ident = line.substr(0, 3);
    index2 = addOrFindNode(ident);
    network[index].r = index2;
}

int solution1() {
    int steps = 0;
    int position = -1; 
    int goal = -1;
    for (int i = 0; i < network_size; ++i) {
        if (network[i].ident == "AAA") {
            position = i;
        }
        if (network[i].ident == "ZZZ") {
            goal = i;
        }
        if (goal != -1 && position != -1) {
            break;
        }
    }

    while (position != goal) {
        if (instruction[steps % instruction.length()] == 'L') {
            position = network[position].l;
        } else {
            position = network[position].r;
        }
        steps++;
    }
    return steps;
}
long long gcd(long long a, long long b) {
    if (b==0) return a;
    return gcd(b, a%b);
}

long long lcm(long long a, long long b) {
    return a*b/gcd(a, b);
}

bool allFinished(std::vector<Way> ways) {
    for (int i = 0; i < ways.size(); ++i) {
        if (!ways[i].finished) {
            return false;
        }
    }
    return true;
}

long long solution2() {
    int steps = 0;
    int position_count = 0;
    std::vector<Way> ways;
    //Find start points
    for (int i = 0; i < network_size; ++i) {
        if (network[i].ident[2] == 'A') {
            Way way;
            way.position = i;
            ways.push_back(way);
        }
    }

    //Find first finish
    while (!allFinished(ways)) {
        for (int i = 0; i < ways.size(); ++i) {
            if (!ways[i].finished) {
                ways[i].position = instruction[steps % instruction.length()] == 'L' ? network[ways[i].position].l : network[ways[i].position].r;
                if (network[ways[i].position].ident[2] == 'Z') {
                    ways[i].finished = true;
                    ways[i].steps = steps + 1;
                }
            }
        }
        steps++;
    }

    //Find lcm
    long long temp = ways[0].steps;
    for (int i = 1; i < ways.size(); i++) {
        temp = lcm(temp, ways[i].steps);
    }

    return temp;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    getline(file, line);
    instruction = line;
    getline(file, line);
    while (getline(file, line)) {
        parseNode(line);
    }
    file.close();

    /*for (int i = 0; i < network_size; ++i) {
        std::cout << "Node(ident: " << network[i].ident << ", l: " << network[i].l << ", r:" << network[i].r << ")" << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
