#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif
#define FIELD_CAP 1000

char field[FIELD_CAP * FIELD_CAP];
int width = 0, height = 0;

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isSymbol(char c) {
    return !isNumber(c) && c != '.';
}

bool isInRange(int i, int j) {
    return i >= 0 && i < height && j >= 0 && j < width;
}

int solution1() {
    int sum = 0;
    for (int i = 0; i < height; i++) {
        int j = 0;
        while (j < width) {
            char c  = field[i * height + j];
            if (!isNumber(c)) {
                j++;
                continue;
            }

            int length = 1;
            while (j + length < width && isNumber(field[i * height + j + length])) {
                length++;
            }
            
            bool found = false;
            if ((isInRange(i, j - 1) && isSymbol(field[i * height + j - 1])) || 
                (isInRange(i, j + length) && isSymbol(field[i * height + j + length]))) 
            {
                found = true;
            }
            if (!found && i - 1 >= 0) {
                for (int x = -1; x <= length; x++) {
                    if (isInRange(i - 1, j + x) && isSymbol(field[(i - 1) * height + j + x])) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found && i + 1 < height) {
                for (int x = -1; x <= length; x++) {
                    if (isInRange(i + 1, j + x) && isSymbol(field[(i + 1) * height + j + x])) {
                        found = true;
                        break;
                    }
                }
            }
            if (found) {
                int number = atoi(field + (i * height) + j);
                sum += number;
            }
            j += length;
        }
    }
    return sum;
}

int getGearRation(int i, int j) {
    //Find adjusent numbers
    //Left 
    std::vector<int> numbers;
    int length = 0;
    if (isInRange(i, j - 1) && isNumber(field[i * height + j - 1])) {
        length = 1;
        while (isInRange(i, j - length - 1) && isNumber(field[i * height + j - length - 1])) {
            length++;
        }
        numbers.push_back(atoi(field + i * height + j - length));
    }
    //Right
    if (isInRange(i, j + 1) && isNumber(field[i * height + j + 1])) {
        length = 1;
        while (isInRange(i, j + length + 1) && isNumber(field[i * height + j + length + 1])) {
            length++;
        }
        numbers.push_back(atoi(field + i * height + j + 1));
    }
    //Top
    if (i - 1 >= 0) {
        if (isNumber(field[(i - 1) * height + j])) {
            length = 0;
            while (isInRange(i - 1, j - length - 1) && isNumber(field[(i - 1) * height + j - length - 1])) {
                length++;
            }
            numbers.push_back(atoi(field + (i - 1) * height + j - length));
        } else {
            if (isInRange(i - 1, j + 1) && isNumber(field[(i - 1) * height + j + 1])) {
                numbers.push_back(atoi(field + (i - 1) * height + j + 1));
            }
            if (isInRange(i - 1, j - 1) && isNumber(field[(i - 1) * height + j - 1])) {
                length = (i - 1) * height + j -1;
                while (isInRange(length / height, length % width) && isNumber(field[length - 1])) {
                    length--;
                }
                numbers.push_back(atoi(field + length));
            }
        }
    }

    //Bottom
    if (i + 1 >= 0) {
        if (isNumber(field[(i + 1) * height + j])) {
            length = 0;
            while (isInRange(i + 1, j - length - 1) && isNumber(field[(i + 1) * height + j - length - 1])) {
                length++;
            }
            numbers.push_back(atoi(field + (i + 1) * height + j - length));
        } else {
            if (isInRange(i + 1, j + 1) && isNumber(field[(i + 1) * height + j + 1])) {
                numbers.push_back(atoi(field + (i + 1) * height + j + 1));
            }
            if (isInRange(i + 1, j + 1) && isNumber(field[(i + 1) * height + j - 1])) {
                length = (i + 1) * height + j -1;
                while (isInRange(length / height, length % width) && isNumber(field[length - 1])) {
                    length--;
                }
                numbers.push_back(atoi(field + length));
            }
        }
    }
    if (numbers.size() == 2) {
        return numbers.at(0) * numbers.at(1);
    }

    return 0;
}

int solution2() {
    int sum = 0;
    for (int i = 0; i < height * width; ++i) {
        if (field[i] == '*') {
            sum += getGearRation(i / height, i % width);
        }
    }       
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        if (width == 0) {
            width = line.size();
        }
        for (int i = 0; i < line.size(); i++) {
            char c = line.at(i);
            field[height * line.size() + i] = c;
        }
        height++;
    }
    file.close();
    
    /*for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << field[i * height + j];
        }
        std::cout << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
