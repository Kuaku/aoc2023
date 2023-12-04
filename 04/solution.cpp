#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif
#define LINE_CAP 512

struct Card {
    std::vector<int> numbers;
    std::vector<int> winning_numbers;
    int count = 1;
};

Card cards[LINE_CAP];
int size = 0;
size_t number_size;
int number;

int solution1() {
    int sum = 0;
    int points = 0;
    for (int i = 0; i < size; ++i) {
        Card card = cards[i];
        points = 0;

        for (int j = 0; j < card.numbers.size(); ++j) {
            for (int x = 0; x < card.winning_numbers.size(); ++x) {
                if (card.numbers.at(j) == card.winning_numbers.at(x)) {
                    points = points == 0 ? 1 : points * 2;
                    break;
                }
            }
        }

        sum += points;
    }
    return sum;
}

int solution2() {
    int sum = 0;
    
    int points = 0;
    for (int i = 0; i < size; ++i) {
        Card card = cards[i];
        points = 0;
        sum += card.count;

        for (int j = 0; j < card.numbers.size(); ++j) {
            for (int x = 0; x < card.winning_numbers.size(); ++x) {
                if (card.numbers.at(j) == card.winning_numbers.at(x)) {
                    points++;
                    break;
                }
            }
        }
        int x = 1; 
        while (i + x < size && x <= points) {
            cards[i + x].count += card.count;
            x++;
        }
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        Card card;
        line = line.substr(5);
        stoi(line, &number_size);
        line = line.substr(number_size + 2);

        do {
            number = stoi(line, &number_size);
            card.numbers.push_back(number);
            line = line.substr(number_size + 1);
        } while(line.at(0) != '|');

        line = line.substr(2);

        do {
            number = stoi(line, &number_size);
            card.winning_numbers.push_back(number);
            line = line.substr(number_size);
        } while(line.length() != 0);
        cards[size] = card;
        size++;
    }
    file.close();

    /*for (int i = 0; i < size; ++i) {
        Card card = cards[i];
        std::cout << "Card " << (i + 1) << ":";
        for (int j = 0; j < card.numbers.size(); ++j) {
            std::cout << " " << card.numbers.at(j);
        }
        std::cout << " |";
        for (int j = 0; j < card.winning_numbers.size(); ++j) {
            std::cout << " " << card.winning_numbers.at(j);
        }
        std::cout << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
