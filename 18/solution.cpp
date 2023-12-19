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
#define CAPCITY 2048

struct Position {
    long long x, y;
};

struct Line {
    Position start, end;
    bool convex = false;
    bool vert = false;
};

Line edges[CAPCITY];
int size;
int width, height;
Position verticies[CAPCITY];
int verts = 0;

void move_out_of_negative() {
    int least_y = 0;
    int least_x = 0;
    for (int i = 0; i < size; ++i) {
        if (edges[i].start.x < least_x) {
            least_x = edges[i].start.x;
        }
        if (edges[i].end.x < least_x) {
            least_x = edges[i].end.x;
        }
        if (edges[i].start.y < least_y) {
            least_y = edges[i].start.y;
        }
        if (edges[i].end.y < least_y) {
            least_y = edges[i].end.y;
        }
    }
    for (int i = 0; i < size; ++i) {
        if (least_x < 0) {
            edges[i].start.x += abs(least_x);
            edges[i].end.x += abs(least_x);
        }
        if (least_y < 0) {
            edges[i].start.y += abs(least_y);
            edges[i].end.y += abs(least_y);
        }
    }
}

void calc_size() {
    for (int i = 0; i < size; ++i) {
        if (edges[i].start.x > width) {
            width = edges[i].start.x;
        }
        if (edges[i].end.x > width) {
            width = edges[i].end.x;
        }
        if (edges[i].start.y > height) {
            height = edges[i].start.y;
        }
        if (edges[i].end.y > height) {
            height = edges[i].end.y;
        }
    }
    width++;
    height++;
}

int is_in_line(int x, int y) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    for (int i = 0; i < size; ++i) {
        x1 = std::min(edges[i].start.x, edges[i].end.x);
        x2 = x1 != edges[i].start.x ? edges[i].start.x : edges[i].end.x;
        y1 = std::min(edges[i].start.y, edges[i].end.y);
        y2 = y1 != edges[i].start.y ? edges[i].start.y : edges[i].end.y;
        if (x1 <= x && x2 >= x && y1 <= y && y2 >= y) {
            return i;
        }
    }
    return -1;
}

int is_in_vert_line(int x, int y) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    for (int i = 0; i < size; ++i) {
        if (!edges[i].vert) {
            continue;
        }
        x1 = std::min(edges[i].start.x, edges[i].end.x);
        x2 = x1 != edges[i].start.x ? edges[i].start.x : edges[i].end.x;
        y1 = std::min(edges[i].start.y, edges[i].end.y);
        y2 = y1 != edges[i].start.y ? edges[i].start.y : edges[i].end.y;
        if (x1 <= x && x2 >= x && y1 <= y && y2 >= y) {
            return i;
        }
    }
    return -1;
}

int is_in_hor_line(int x, int y) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    for (int i = 0; i < size; ++i) {
        if (edges[i].vert) {
            continue;
        }
        x1 = std::min(edges[i].start.x, edges[i].end.x);
        x2 = x1 != edges[i].start.x ? edges[i].start.x : edges[i].end.x;
        y1 = std::min(edges[i].start.y, edges[i].end.y);
        y2 = y1 != edges[i].start.y ? edges[i].start.y : edges[i].end.y;
        if (x1 <= x && x2 >= x && y1 <= y && y2 >= y) {
            return i;
        }
    }
    return -1;
}

bool is_convex_vert(int index) {
    int prev = index - 1 < 0 ? size - 1 : index - 1;
    int next = (index + 1) % size;
    bool next_smaller = edges[next].end.x < edges[index].end.x;
    bool prev_smaller = edges[prev].start.x < edges[index].start.x;

    return next_smaller != prev_smaller;
}

bool is_convex_hor(int index) {
    int prev = index - 1 < 0 ? size - 1 : index - 1;
    int next = (index + 1) % size;
    bool next_smaller = edges[next].end.y < edges[index].end.y;
    bool prev_smaller = edges[prev].start.y < edges[index].start.y;

    return next_smaller != prev_smaller;
}

void calc_convex() {
    for (int i = 0; i < size; ++i) {
        if (edges[i].start.x == edges[i].end.x) {
            edges[i].convex = is_convex_vert(i);
            edges[i].vert = true;
        } else {
            edges[i].convex = is_convex_hor(i);
        }
    }
}

std::string fill_to(int v, int l) {
    std::string out = std::to_string(v);
    while (out.length() < l) {
        out = "0" + out;
    }
    return out;
}
void print_field() {
    for (int y = 0; y < height; ++y) {
        bool inside = false;
        int last_hit = -1;
        std::cout << fill_to(y, 3) << ": ";
        for (int x = 0; x < width; ++x) {
            int hit = is_in_vert_line(x, y);
            if (hit == -1) {
                hit = is_in_hor_line(x, y);
            }
            if (hit != -1) {
                std::cout << '#';
                if (hit != last_hit && (edges[hit].vert || edges[hit].convex)) {
                    inside = !inside;
                }
                last_hit = hit;
            } else if (inside) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int calc_fields() {
    int sum = 0;
    for (int y = 0; y < height; ++y) {
        bool inside = false;
        int last_hit = -1;
        for (int x = 0; x < width; ++x) {
            int hit = is_in_vert_line(x, y);
            if (hit == -1) {
                hit = is_in_hor_line(x, y);
            }
            if (hit != -1) {
                sum++;
                if (hit != last_hit && (edges[hit].vert || edges[hit].convex)) {
                    inside = !inside;
                }
                last_hit = hit;
            } else if (inside) {
                sum++;
            }
        }
    }
    return sum;
}

int solution1() {
    return calc_fields();
}

long long solution2() {
    long long sum = 0;
    for (int i = 0; i < verts ; ++i) {
        sum += (verticies[i].x * verticies[(i+1)%verts].y - verticies[i].y * verticies[(i+1)%verts].x);
        std::cout << sum << std::endl;
        //std::cout << (verticies[i].x * verticies[(i+1)%verts].y) << " - " << verticies[i].y * verticies[(i+1)%verts].x << " + ";
        //verticies[i].y * (verticies[i-1 < 0 ? verts - 1 : i-1].x - verticies[(i+1)%verts].x);
    }
    sum = sum/2;
    int ext = 0;
    for (int i = 0; i < verts; ++i) {
        ext += abs(verticies[i].x - verticies[(i+1)%verts].x) + abs(verticies[i].y - verticies[(i+1)%verts].y); 
    }
    //std::cout << ext/2 << std::endl;
    return sum + ext/2 + 1;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    Position last = {0, 0};
    size_t number_length;
    verticies[0] = {0, 0};
    verts++;
    while (getline(file, line)) {
        char dir = line[0];
        line = line.substr(2);
        int length1 = stoi(line, &number_length);
        Position next = {last.x, last.y};
        switch (dir)
        {
            case 'R': next.x += length1; break;
            case 'L': next.x -= length1; break;
            case 'U': next.y -= length1; break;
            case 'D': next.y += length1; break;
        }
        last = next;
        size++;
        line = line.substr(number_length + 3);
        int length2 = stoi(line.substr(0, 5), nullptr, 16);
        std::cout << length2 << std::endl;
        verticies[verts] = {verticies[verts-1].x, verticies[verts-1].y};
        std::cout << line[5] << std::endl;
        switch (line[5])
        {
            case '0': verticies[verts].x += length2; break;
            case '1': verticies[verts].y += length2; break;
            case '2': verticies[verts].x -= length2; break;
            case '3': verticies[verts].y -= length2; break;
        }
        if (verticies[verts].x == 0 && verticies[verts].y == 0) {
            continue;
        }
        verts++;
    }
    file.close();
    move_out_of_negative();
    calc_size();
    calc_convex();
    for (int i = 0; i < verts; ++i) {
        std::cout << "Vert(x: "<<verticies[i].x<<",y: "<<verticies[i].y<<")" << std::endl;
    }
    /*for (int i = 0; i < size; ++i) {
        std::cout << "Line(start: (x: "<<edges[i].start.x<<",y: "<<edges[i].start.y<<"), end: (x: "<<edges[i].end.x<<",y: "<<edges[i].end.y<<"), convex: "<<edges[i].convex<<")" << std::endl;
    }*/
    //print_field();

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
