#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <math.h> 

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif

struct Race {
    int record;
    int time;
};

std::vector<Race> races;



int solution1() {
    int sum = 1;
    for (int i = 0; i < races.size(); ++i) {
        Race race = races[i];
        double record = race.record;
        double time = race.time;
        double s = sqrt((time * time)/4.0 - record);

        int x1 = time / 2 - s;
        int x2 = ceil(time / 2 + s - 1);
        sum *= x2 - x1;
    }
    return sum;
}

int solution2() {
    std::string totalTimeString =  "";
    std::string totalRecordString =  "";
    for (int i = 0; i < races.size(); ++i) {
        totalTimeString += std::to_string(races[i].time);
        totalRecordString += std::to_string(races[i].record);
    }
    long double time = strtold(totalTimeString.c_str(), nullptr);
    long double record = strtold(totalRecordString.c_str(), nullptr);
    long double s = sqrt((time * time)/4.0 - record);

    long x1 = time / 2 - s;
    long x2 = ceil(time / 2 + s - 1);

    return x2 - x1;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    size_t number_size;
    getline(file, line);
    line = line.substr(5);
    while (line.length() > 0) {
        Race race;
        race.time = stoi(line, &number_size);
        line = line.substr(number_size);
        races.push_back(race);
    }
    getline(file, line);
    line = line.substr(9);
    int i = 0;
    while (line.length() > 0) {
        Race race;
        races[i].record = stoi(line, &number_size);
        line = line.substr(number_size);
        i++;
    }
    file.close();

    /*for (int i = 0; i < races.size(); i++) {
        std::cout << "Race(time: " << races[i].time << ", record: " << races[i].record << ")" << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
