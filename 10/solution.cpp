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
enum Pipes {
    Ground = '.',
    Vertical = '|',
    Horizontal = '-',
    NorthEast = 'L',
    NorthWest = 'J',
    SouthWest = '7',
    SouthEast = 'F',
    Start = 'S'
};

enum Dir {
    Top,
    Bottom,
    Left,
    Right,
    None
};

struct Position {
    int x, y;
};

struct Line {
    Position pos1, pos2;
    bool convex;
};

char field[CAPCITY * CAPCITY];
bool isTrack[CAPCITY * CAPCITY];
int width = 0, height = 0;

std::vector<Position> positions;

bool hasTopConnection(int x, int y) {
    return y >= 1 && (field[(y - 1) * width + x] == Vertical || field[(y - 1) * width + x] == SouthEast || field[(y - 1) * width + x] == SouthWest);     
}

bool hasBottomConnection(int x, int y) {
    return y + 1 < height && (field[(y + 1) * width + x] == Vertical || field[(y + 1) * width + x] == NorthEast || field[(y + 1) * width + x] == NorthWest);     
}

bool hasRightConnection(int x, int y) {
    return x + 1 < width && (field[y * width + x + 1] == Horizontal || field[y * width + x + 1] == SouthWest || field[y * width + x + 1] == NorthWest);
}

bool hasLeftConnection(int x, int y) {
    return x - 1 < width && (field[y * width + x - 1] == Horizontal || field[y * width + x - 1] == SouthEast || field[y * width + x - 1] == NorthEast);
}

int solution1() {
    int sx = -1, sy = -1;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (field[i * width + j] == Start) {
                sx = j;
                sy = i;
                break;
            }
            if (sx != -1) break;
        }
    }
    int x = sx, y = sy;
    Dir dir = None;

    bool top = hasTopConnection(sx, sy);
    bool bottom = hasBottomConnection(sx, sy);
    bool left = hasLeftConnection(sx, sy);
    bool right = hasRightConnection(sx, sy);
    if (top) {
        y -= 1;
        dir = Top;
    } else if (bottom) {
        y += 1;
        dir = Bottom;
    } else if (left) {
        x -= 1;
        dir = Left;
    } else {
        x += 1;
        dir = Right;
    }

    int steps = 1;
    do {
        char c = field[x + y * width];
        if (dir == Top) {
            if (c == SouthEast) {
                dir = Right;
            } else if (c == SouthWest) {
                dir = Left;
            }
        } else if (dir == Bottom) {
            if (c == NorthEast) {
                dir = Right;
            } else if (c == NorthWest) {
                dir = Left;
            }
        } else if (dir == Left) {
            if (c == SouthEast) {
                dir = Bottom;
            } else if (c == NorthEast) {
                dir = Top;
            }
        } else if (dir == Right) {
            if (c == SouthWest) {
                dir = Bottom;
            } else if (c == NorthWest) {
                dir = Top;
            }
        }

        switch (dir)
        {
            case Top: y -= 1; break;
            case Bottom: y += 1; break;
            case Left: x -= 1; break;
            case Right: x += 1; break;
        }

        steps++;
    } while (x != sx || y != sy);
    return steps / 2;
}

void markAsTrack(int x, int y) {
    Position position;
    position.x = x;
    position.y = y;
    positions.push_back(position);
    isTrack[x + y * width] = true;
}
bool isInArray(Position position) {
    return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
}

Position directions[4] = {
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1}
};
std::vector<Line> lines;


bool isInLine(int x, int y, Line line) {
    int x1 = std::min(line.pos1.x, line.pos2.x);
    int x2 = x1 != line.pos1.x ? line.pos1.x : line.pos2.x;
    int y1 = std::min(line.pos1.y, line.pos2.y);
    int y2 = y1 != line.pos1.y ? line.pos1.y : line.pos2.y;

    return x1 <= x && x2 >= x && y1 <= y && y2 >= y;
}

int hasCollinearHit(int x, int y, bool horizontal) {
    for (int i = 0; i < lines.size(); ++i) {
        Line line = lines[i];
        if ((horizontal && line.pos1.x == line.pos2.x) || (!horizontal && line.pos1.y == line.pos2.y)) {
            continue;
        }
        if (isInLine(x, y, line)) {
            return i;
        }
    }
    return -1;
}

int hasNonCollinearHit(int x, int y, bool horizontal) {
    for (int i = 0; i < lines.size(); ++i) {
        Line line = lines[i];
        if ((horizontal && line.pos1.x != line.pos2.x) || (!horizontal && line.pos1.y != line.pos2.y)) {
            continue;
        }
        if (isInLine(x, y, line)) {
            return i;
        }
    }
    return -1;
}

bool isInTrack(int x, int y) {
    int inside = 0;
    for (int i = 0; i < 4; ++i) {
        bool isInside = false;
        int hits = 0;
        Position direction = directions[i];
        Position position = {x, y};
        while (isInArray(position)) {
            int lineIndex = hasCollinearHit(position.x, position.y, direction.y == 0);
            if (lineIndex != -1) {
                Line line = lines[lineIndex];
                if (!line.convex) {
                    hits++;
                    isInside = !isInside;
                }

                if (direction.y == 0) {
                    if (position.x == line.pos1.x) {
                        position.x = line.pos2.x;
                    } else {
                        position.x = line.pos1.x;
                    }
                } else {
                    if (position.y == line.pos1.y) {
                        position.y = line.pos2.y;
                    } else {
                        position.y = line.pos1.y;
                    }
                }
                position.x += direction.x;
                position.y += direction.y;
                continue;
            }
            lineIndex = hasNonCollinearHit(position.x, position.y, direction.y == 0);
            if (lineIndex != -1) {
                hits++;
                isInside = !isInside;
            }

            position.x += direction.x;
            position.y += direction.y;
        }
        if (!isInside) {
            return false;
        }
    }
    //std::cout << "Position(x: " << x << ",y: " << y << ")" << std::endl;
    return true;
}


long long solution2() {
    int sx = -1, sy = -1;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (field[i * width + j] == Start) {
                sx = j;
                sy = i;
                break;
            }
            if (sx != -1) break;
        }
    }
    int x = sx, y = sy;
    markAsTrack(sx, sy);
    Dir dir = None;
    Position lastCorner = {sx, sy};
    bool top = hasTopConnection(sx, sy);
    bool bottom = hasBottomConnection(sx, sy);
    bool left = hasLeftConnection(sx, sy);
    bool right = hasRightConnection(sx, sy);

    if (top && bottom) {
        field[sx + sy * width] = Vertical;
    } else if (left && right) {
        field[sx + sy * width] = Horizontal;
    } else if (top && left) {
        field[sx + sy * width] = NorthEast;
    } else if (top && left) {
        field[sx + sy * width] = NorthWest;
    } else if (bottom && right) {
        field[sx + sy * width] = SouthEast;
    } else {
        field[sx + sy * width] = SouthWest;
    }

    if (top) {
        y -= 1;
        dir = Top;
    } else if (bottom) {
        y += 1;
        dir = Bottom;
    } else if (left) {
        x -= 1;
        dir = Left;
    } else {
        x += 1;
        dir = Right;
    }
    Dir temp;
    do {
        markAsTrack(x, y);
        temp = dir;
        char c = field[x + y * width];
        if (dir == Top) {
            if (c == SouthEast) {
                dir = Right;
            } else if (c == SouthWest) {
                dir = Left;
            }
        } else if (dir == Bottom) {
            if (c == NorthEast) {
                dir = Right;
            } else if (c == NorthWest) {
                dir = Left;
            }
        } else if (dir == Left) {
            if (c == SouthEast) {
                dir = Bottom;
            } else if (c == NorthEast) {
                dir = Top;
            }
        } else if (dir == Right) {
            if (c == SouthWest) {
                dir = Bottom;
            } else if (c == NorthWest) {
                dir = Top;
            }
        }


        if (temp != dir) {
            bool convex = false;
            char c1 = field[x + y * width],
                c2 = field[lastCorner.x + lastCorner.y * width];
            if (c1 == NorthEast && c2 == SouthEast) {
                convex = true;
            } else if (c1 == NorthEast && c2 == NorthWest) {
                convex = true;
            } else if (c1 == NorthWest && c2 == SouthWest) {
                convex = true;
            } else if (c1 == NorthWest && c2 == NorthEast) {
                convex = true;
            } else if (c1 == SouthWest && c2 == NorthWest) {
                convex = true;
            } else if (c1 == SouthWest && c2 == SouthEast) {
                convex = true;
            } else if (c1 == SouthEast && c2 == NorthEast) {
                convex = true;
            } else if (c1 == SouthEast && c2 == SouthWest) {
                convex = true;
            }
            lines.push_back({lastCorner, {x, y}, convex});
            lastCorner = {x, y};
        }

        switch (dir)
        {
            case Top: y -= 1; break;
            case Bottom: y += 1; break;
            case Left: x -= 1; break;
            case Right: x += 1; break;
        }


    } while (x != sx || y != sy);
    
    bool convex = false;
    char c1 = field[sx + sy * width],
        c2 = field[lastCorner.x + lastCorner.y * width];
    if (c1 == NorthEast && c2 == SouthEast) {
        convex = true;
    } else if (c1 == NorthEast && c2 == NorthWest) {
        convex = true;
    } else if (c1 == NorthWest && c2 == SouthWest) {
        convex = true;
    } else if (c1 == NorthWest && c2 == NorthEast) {
        convex = true;
    } else if (c1 == SouthWest && c2 == NorthWest) {
        convex = true;
    } else if (c1 == SouthWest && c2 == SouthEast) {
        convex = true;
    } else if (c1 == SouthEast && c2 == NorthEast) {
        convex = true;
    } else if (c1 == SouthEast && c2 == SouthWest) {
        convex = true;
    }
    lines.push_back({lastCorner, {sx, sy}, convex});
    /*for (int i = 0; i < positions.size(); ++i) {
        std::cout << "Position(x: " << positions[i].x << ",y: " << positions[i].y << ")" << std::endl;
    }*/

    /*for (int i = 0; i < lines.size(); ++i) {
        std::cout << "Line(pos1: Position(x: " << lines[i].pos1.x << ", y: " << lines[i].pos1.y << "), pos2: Position(x: " << lines[i].pos2.x << ", y: " << lines[i].pos2.y << "), convex: " << lines[i].convex << ")" << std::endl;
    }*/

    int sum = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (isTrack[i * width + j]) {
                continue;
            }

            if (isInTrack(j, i)) {
                sum++;
            }
        }
    }

    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        width = line.length();
        for (int i = 0; i < width; ++i) {
            field[height * width + i] = line[i];
        }
        height++;
    }
    file.close();

    /*for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << field[i * width + j];
        }
        std::cout << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    auto started = std::chrono::high_resolution_clock::now();
    std::cout << "Solution 2: " << solution2() << std::endl;
    auto done = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();
    return 0;
}
