#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif
#define LINE_CAP 1024

struct Pull {
    int blue;
    int green;
    int red;
};

struct Game
{
    std::vector<Pull> pulls;
};
Game games[LINE_CAP];
int size = 0; 

void parsePull(std::string *line, Pull *pull) {
    size_t number_size;
    do {
        int number = stoi(*line, &number_size);
        *line = line->substr(number_size + 1);
        if (line->length() >= 4 && line->substr(0, 4) == "blue")  {
            pull->blue = number;
            *line = line->substr(4);
        } else if (line->length() >= 5 && line->substr(0, 5) == "green") {
            pull->green = number;
            *line = line->substr(5);
        } else if (line->length() >= 3 && line->substr(0, 3) == "red") {
            pull->red = number;
            *line = line->substr(3);
        }
        if (line->length() == 0) {
            break;
        }
        if (line->at(0) == ';') {
            *line = line->substr(2);
            break;
        } 
        *line = line->substr(2);
    } while(true);
}

bool isValidPull(Pull pull) {
    return pull.red <= 12 && pull.green <= 13 && pull.blue <= 14;
}

int solution1() {
    int sum = 0; 

    for (int i = 0; i < size; i++) {
        Game game = games[i];
        bool valid = true;
        for (int j = 0; j < game.pulls.size(); j++) {
            if (!isValidPull(game.pulls.at(j))) {
                valid = false;
                break;
            }
        }
        if (valid) {
            sum += i + 1;
        }
    } 

    return sum;
}

int solution2() {
    int sum = 0;

    for (int i = 0; i < size; i++) {
        Game game = games[i];
        int green = 0, red = 0, blue = 0;
        for (int j = 0; j < game.pulls.size(); j++) {
            Pull pull = game.pulls.at(j);
            if (pull.green > green) {
                green = pull.green;
            }
            if (pull.red > red) {
                red = pull.red;
            }
            if (pull.blue > blue) {
                blue = pull.blue;
            }
        }
        sum += green * red * blue;
    } 

    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        line = line.substr(5);
        size_t number_size = 0;
        stoi(line, &number_size);
        line = line.substr(number_size + 2);
        Game game;
        do {
            Pull pull = {0, 0, 0};
            parsePull(&line, &pull);
            game.pulls.push_back(pull);
        } while(line.length() > 0);
        games[size] = game;
        size++;
    }

    
    /*for (int i = 0; i < size; i++) {
        std::cout << "Game: " << i << std::endl;
        for (int j = 0; j < games[i].pulls.size(); j++) {
            Pull pull = games[i].pulls.at(j);
            std::cout << "Pull{blue: " << pull.blue << ", green: " << pull.green << ", red: " << pull.red << "}" << std::endl; 
        }
    }*/
    

    file.close();
    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
