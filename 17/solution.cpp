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

enum Direction {
    NORTH=0,
    SOUTH=1,
    WEST=2,
    EAST=3
};

struct Node {
    int x, y, d, c;
    Direction dir;
    bool defined = false;
};

int field[CAPCITY * CAPCITY];
int width, height;
bool visited[99999999];
int ptoa(int x, int y) {
    return x + y * width;
}


std::string fill_to(int v, int l) {
    std::string out = std::to_string(v);
    while (out.length() < l) {
        out = "0" + out;
    }
    return out;
}

int hash(Node* node) {
    //return stoi("1" + fill_to_3(node->x)+ fill_to_3(node->y)+std::to_string(node->d)+std::to_string(node->dir));
    return stoi(fill_to(node->x, 3)+ fill_to(node->y, 3)+std::to_string(node->d % 10)+std::to_string(node->dir));
}
std::vector<Node> q;

bool contains(Node* node) {
    return visited[hash(node)];
}
int next_possibles_x[3];
int next_possibles_y[3];
int next_d[3];
Direction next_dir[3];
Node next_nodes[3];
int size = 0;
void reset_possibles() {
    size = 0;
}

bool is_in_field(Node* node) {
    return node->x >= 0 && node->x < width && node->y >= 0 && node->y < height;
}

void fill_possibles(Node* node) {
    if (node->d != 3) {
        next_nodes[size].x = node->x;
        next_nodes[size].y = node->y;
        next_nodes[size].d = node->d + 1;
        next_nodes[size].c = node->c;
        next_nodes[size].dir = node->dir;

        switch (node->dir)
        {
            case NORTH: next_nodes[size].y -= 1;; break;
            case SOUTH: next_nodes[size].y += 1; break;
            case EAST: next_nodes[size].x += 1; break;
            case WEST: next_nodes[size].x += -1; break;
        }
        if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
            next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
            size++;
        }
    }

    next_nodes[size].x = node->x;
    next_nodes[size].y = node->y;
    next_nodes[size].d = 1;
    next_nodes[size].c = node->c;
    next_nodes[size].dir = node->dir;
    if (node->dir == NORTH || node->dir == SOUTH) {
        next_nodes[size].x -= 1;
        next_nodes[size].dir = WEST;
    } else {
        next_nodes[size].y -= 1;
        next_nodes[size].dir = NORTH;
    }
    if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
        next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
        size++;
    }

    next_nodes[size].x = node->x;
    next_nodes[size].y = node->y;
    next_nodes[size].d = 1;
    next_nodes[size].c = node->c;
    next_nodes[size].dir = node->dir;
    if (node->dir == NORTH || node->dir == SOUTH) {
        next_nodes[size].x += 1;
        next_nodes[size].dir = EAST;
    } else {
        next_nodes[size].y += 1;
        next_nodes[size].dir = SOUTH;
    }
    if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
        next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
        size++;
    }
}

void fill_possibles2(Node* node) {
    if (node->d >= 4) {
        if (node->d != 10) {
            next_nodes[size].x = node->x;
            next_nodes[size].y = node->y;
            next_nodes[size].d = node->d + 1;
            next_nodes[size].c = node->c;
            next_nodes[size].dir = node->dir;

            switch (node->dir)
            {
                case NORTH: next_nodes[size].y -= 1;; break;
                case SOUTH: next_nodes[size].y += 1; break;
                case EAST: next_nodes[size].x += 1; break;
                case WEST: next_nodes[size].x += -1; break;
            }
            if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
                next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
                size++;
            }
        }
        next_nodes[size].x = node->x;
        next_nodes[size].y = node->y;
        next_nodes[size].d = 1;
        next_nodes[size].c = node->c;
        next_nodes[size].dir = node->dir;
        if (node->dir == NORTH || node->dir == SOUTH) {
            next_nodes[size].x -= 1;
            next_nodes[size].dir = WEST;
        } else {
            next_nodes[size].y -= 1;
            next_nodes[size].dir = NORTH;
        }
        if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
            next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
            size++;
        }

        next_nodes[size].x = node->x;
        next_nodes[size].y = node->y;
        next_nodes[size].d = 1;
        next_nodes[size].c = node->c;
        next_nodes[size].dir = node->dir;
        if (node->dir == NORTH || node->dir == SOUTH) {
            next_nodes[size].x += 1;
            next_nodes[size].dir = EAST;
        } else {
            next_nodes[size].y += 1;
            next_nodes[size].dir = SOUTH;
        }
        if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
            next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
            size++;
        }
    } else {
        next_nodes[size].x = node->x;
        next_nodes[size].y = node->y;
        next_nodes[size].d = node->d + 3;
        next_nodes[size].c = node->c;
        next_nodes[size].dir = node->dir;
        int x_c = 0;
        int y_c = 0;
        switch (node->dir)
        {
            case NORTH: y_c = -1;; break;
            case SOUTH: y_c = 1; break;
            case EAST: x_c = 1; break;
            case WEST: x_c = -1; break;
        }
        for (int i = 0; i < 3; i++) {
            next_nodes[size].x += x_c;
            next_nodes[size].y += y_c;
             if (!is_in_field(&next_nodes[size])) {
                return;
            }
            next_nodes[size].c += field[ptoa(next_nodes[size].x, next_nodes[size].y)];
        }
        if (is_in_field(&next_nodes[size]) && !contains(&next_nodes[size])) {
            size++;
        }

    }
}

void print_field() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool found = false;
            for (int i = 0; i < q.size(); ++i) {
                if (q[i].x == x && q[i].y == y) {
                    found = true;
                    break;
                }
            }
            if (found) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int solution1() {
    for (int i = 0; i < 99999999; ++i) {
        visited[i] = false;
    }
    q.push_back({1, 0, 1, field[ptoa(1, 0)], EAST});
    q.push_back({0, 1, 1, field[ptoa(0, 1)], SOUTH});
    visited[hash(&q[0])] = true;
    visited[hash(&q[1])] = true;
    while (q.size() != 0) {
        int x, y;
        int smallest = INT_MAX;
        Node smallest_n;
        for (int i = q.size() - 1; i >= 0 ; --i) {
            reset_possibles();
            fill_possibles(&q[i]);
            if (size <= 1) {
                q.erase(q.begin() + i);
            }
            for (int j = 0; j < size; ++j) {
               if (next_nodes[j].c < smallest) {
                    smallest = next_nodes[j].c;
                    smallest_n = next_nodes[j];
               } 
            }
        }
        q.push_back(smallest_n);    
        visited[hash(&smallest_n)] = true;
        if (smallest_n.x == width - 1 && smallest_n.y == height - 1) {
            return smallest_n.c;
        }   
    }
    return 0;
}

int solution2() {
    for (int i = 0; i < 99999999; ++i) {
        visited[i] = false;
    }
    q.push_back({1, 0, 1, field[ptoa(1, 0)], EAST});
    q.push_back({0, 1, 1, field[ptoa(0, 1)], SOUTH});
    visited[hash(&q[0])] = true;
    visited[hash(&q[1])] = true;
    while (q.size() != 0) {
        int x, y;
        int smallest = INT_MAX;
        Node smallest_n;
        for (int i = q.size() - 1; i >= 0 ; --i) {
            reset_possibles();
            fill_possibles2(&q[i]);
            if (size <= 1) {
                q.erase(q.begin() + i);
            }
            for (int j = 0; j < size; ++j) {
               if (next_nodes[j].c < smallest) {
                    smallest = next_nodes[j].c;
                    smallest_n = next_nodes[j];
               } 
            }
        }
        q.push_back(smallest_n);    
        visited[hash(&smallest_n)] = true;
        if (smallest_n.x == width - 1 && smallest_n.y == height - 1 && smallest_n.d >= 4) {
            return smallest_n.c;
        }   
    }
    return 0;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        width = line.length();
        for(int i = 0; i < width; ++i) {
            field[ptoa(i, height)] = int(line[i]) - int('0');
        }
        height++;
    }
    file.close();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << field[ptoa(x, y)];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
