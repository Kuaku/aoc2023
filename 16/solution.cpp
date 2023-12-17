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

char field[CAPCITY * CAPCITY];
bool energized[CAPCITY * CAPCITY];
int width, height;

enum Dir {
    EAST  = 0,
    SOUTH  = 1,
    WEST  = 2,
    NORTH = 3
};

struct Position {
    int x = 0, y = 0;
};

struct Laser {
    Position start, end;
};

struct LaserStart {
    Position start;
    Dir dir;
};

#define CAPCITY_L 100000
Laser lasers[CAPCITY_L];
int size_l = 0;

int ptoa(int x, int y) {
    return x + y * width;
}

Position dtoc(Dir dir) {
    Position c = {0, 0};

    switch (dir)
    {
        case EAST: c.x = 1; break;
        case SOUTH: c.y = 1; break;
        case WEST: c.x = -1; break;
        case NORTH: c.y = -1; break;
    }    

    return c;
}

bool contains_laser(Position start, Position end) {
    for (int i = 0; i < size_l; ++i) {
        if (lasers[i].start.x == start.x && lasers[i].start.y == start.y 
        && lasers[i].end.x == end.x && lasers[i].end.y == end.y) {
            return true;
        }
    }
    return false;
}

bool add_laser(Position start, Position end) {
    //std::cout << "Laser(start: (x:" << start.x << " ,y:"<< start.y <<" ), end: (x:"<< end.x <<" ,y:" << end.y <<" ))" << std::endl;
    if (!contains_laser(start, end)) {
        lasers[size_l] = {start, end};
        size_l++;
        return true;
    }
    return false;
}

void calc_laser(LaserStart start) {
    Position c = dtoc(start.dir);
    int x = start.start.x;
    int y = start.start.y;
    while (true) {
        x += c.x;
        y += c.y;

        if (x < 0 || x >= width || y < 0 || y >= height) {
            x -= c.x;
            y -= c.y;
            add_laser(start.start, {x, y});
            return;
        }

        if (field[ptoa(x, y)] == '/') {
            if (add_laser(start.start, {x, y})) {
                switch (start.dir)
                {
                    case EAST: calc_laser({{x, y}, NORTH}); break;
                    case SOUTH: calc_laser({{x, y}, WEST}); break;
                    case WEST: calc_laser({{x, y}, SOUTH}); break;
                    case NORTH: calc_laser({{x, y}, EAST}); break;
                }
            }
            return;
        } else if (field[ptoa(x, y)] == '\\') {
            if (add_laser(start.start, {x, y})) {
                switch (start.dir)
                {
                    case EAST: calc_laser({{x, y}, SOUTH}); break;
                    case SOUTH: calc_laser({{x, y}, EAST}); break;
                    case WEST: calc_laser({{x, y}, NORTH}); break;
                    case NORTH: calc_laser({{x, y}, WEST}); break;
                }
            }
            return;
        } else if (field[ptoa(x, y)] == '|' && (start.dir == EAST || start.dir == WEST)) {
            if (add_laser(start.start, {x, y})) {
                calc_laser({{x, y}, NORTH});
                calc_laser({{x, y}, SOUTH});
            }
            return;
        } else if (field[ptoa(x, y)] == '-' && (start.dir == NORTH || start.dir == SOUTH)) {
            if (add_laser(start.start, {x, y})) {
                calc_laser({{x, y}, EAST});
                calc_laser({{x, y}, WEST});
            }
            return;
        }
    }
}

int energize() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            energized[ptoa(x, y)] = false;
        }
    }
    
    for (int i = 0; i < size_l; ++i) {
        Position c = {lasers[i].end.x - lasers[i].start.x, lasers[i].end.y - lasers[i].start.y};
        if (c.x > 0) {
            c.x = 1;
        } else if (c.x < 0) {
            c.x = -1;
        }
        if (c.y > 0) {
            c.y = 1;
        } else if (c.y < 0) {
            c.y = -1;
        }
        int x = lasers[i].start.x;
        int y = lasers[i].start.y;
        do {
            energized[ptoa(x, y)] = true;
            x += c.x;
            y += c.y;
        } while (x != lasers[i].end.x || y != lasers[i].end.y);
        energized[ptoa(x, y)] = true;
    }
    int sum = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (energized[ptoa(x, y)]) {
                sum++;
            } else {
            }
        }
    }
    return sum;
}

int solution1() {
    if (field[0] == '\\' || field[0] == '|') {
        calc_laser({{0, 0}, SOUTH});
    } else {
        calc_laser({{0, 0}, EAST});
    }
    return energize();
}

int solution2() {
    int largest = 0;
    int res = 0;
    for (int x = 0; x < width; ++x) {
        size_l = 0;
        if (field[ptoa(x, 0)] == '-') {
            calc_laser({{x, 0}, EAST});
            calc_laser({{x, 0}, WEST});
        } else if (field[ptoa(x, 0)] == '\\') {
            calc_laser({{x, 0}, EAST});
        } else if (field[ptoa(x, 0)] == '/') {
            calc_laser({{x, 0}, WEST});
        } else {
            calc_laser({{x, 0}, SOUTH});
        }
        res = energize();
        if (res > largest) {
            largest = res;
        }

        size_l = 0;
        if (field[ptoa(x, height-1)] == '-') {
            calc_laser({{x, height-1}, EAST});
            calc_laser({{x, height-1}, WEST});
        } else if (field[ptoa(x, height-1)] == '\\') {
            calc_laser({{x, height-1}, WEST});
        } else if (field[ptoa(x, height-1)] == '/') {
            calc_laser({{x, height-1}, EAST});
        } else {
            calc_laser({{x, height-1}, NORTH});
        }
        res = energize();
        if (res > largest) {
            largest = res;
        }
    }
    
    for (int y = 0; y < height; ++y) {
        size_l = 0;
        if (field[ptoa(0, y)] == '|') {
            calc_laser({{0, y}, NORTH});
            calc_laser({{0, y}, SOUTH});
        } else if (field[ptoa(0, y)] == '\\') {
            calc_laser({{0, y}, SOUTH});
        } else if (field[ptoa(0, y)] == '/') {
            calc_laser({{0, y}, NORTH});
        } else {
            calc_laser({{0, y}, EAST});
        }
        res = energize();
        if (res > largest) {
            largest = res;
        }

        size_l = 0;
        if (field[ptoa(width-1, y)] == '|') {
            calc_laser({{width-1, y}, NORTH});
            calc_laser({{width-1, y}, SOUTH});
        } else if (field[ptoa(width-1, y)] == '\\') {
            calc_laser({{width-1, y}, NORTH});
        } else if (field[ptoa(width-1, y)] == '/') {
            calc_laser({{width-1, y}, SOUTH});
        } else {
            calc_laser({{width-1, y}, WEST});
        }
        res = energize();
        if (res > largest) {
            largest = res;
        }
    }
    return largest;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        width = line.length();
        for(int i = 0; i < width; ++i) {
            field[ptoa(i, height)] = line[i];
            energized[ptoa(i, height)] = false;
        }
        height++;
    }
    file.close();

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
