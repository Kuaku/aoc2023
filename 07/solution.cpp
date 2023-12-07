#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif
#define CAPACITY 1024

struct Hand {
    std::string hand;
    int score;  
};
Hand hands[CAPACITY];
Hand sorted_hands[CAPACITY];
int size = 0;

struct CharCount {
    char c;
    int count;
};

CharCount counts[5];
int char_size;

char char_scores[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
char char_scores2[13] = {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'};
int calcScore(Hand hand) {
    char_size = 0;
    for (int i = 0; i < hand.hand.length(); ++i) {
        char c = hand.hand[i];
        
        bool found = false;
        
        for (int j = 0; j < char_size; ++j) {
            if (counts[j].c == c) {
                counts[j].count++;
                found = true;
                break;
            }
        }
        if (!found) {
            counts[char_size].c = c;
            counts[char_size].count = 1;
            char_size++;
        }
    }

    int highest_count = 0, highest_index = 0; 
    for (int i = 0; i < char_size; ++i) {
        if (highest_count < counts[i].count) {
            highest_count = counts[i].count;
            highest_index = i;
        }
    }
    if (highest_count == 1) {
        return 1;
    }

    if (highest_count == 2) {
        for (int i = 0; i < char_size; i++) {
            if (i == highest_index) continue;
            if (counts[i].count == 2) {
                return 3;
            }
        }
        return 2;
    }

    if (highest_count == 3) {
        for (int i = 0; i < char_size; ++i) {
            if (counts[i].count == 2) {
                return 5;
            }
        }
        return 4;
    }

    return highest_count + 2;
}

int calcScore2(Hand hand) {
    int joker = 0;
    char_size = 0;
    for (int i = 0; i < hand.hand.length(); ++i) {
        char c = hand.hand[i];
        
        if (c == 'J') {
            joker++;
            continue;
        }

        bool found = false;
        
        for (int j = 0; j < char_size; ++j) {
            if (counts[j].c == c) {
                counts[j].count++;
                found = true;
                break;
            }
        }
        if (!found) {
            counts[char_size].c = c;
            counts[char_size].count = 1;
            char_size++;
        }
    }

    int highest_count = 0, highest_index = 0; 
    for (int i = 0; i < char_size; ++i) {
        if (highest_count < counts[i].count) {
            highest_count = counts[i].count;
            highest_index = i;
        }
    }
    if (highest_count == 0) {
        return 7;
    }

    if (highest_count == 1) {
        switch (joker) {
            case 0: return 1;
            case 1: return 2;
            case 2: return 4;
            case 3: return 6;
            case 4: return 7;
        }
    }

    if (highest_count == 2) {
        for (int i = 0; i < char_size; i++) {
            if (i == highest_index) continue;
            if (counts[i].count == 2) {
                if (joker == 1) {
                    return 5;
                } else {
                    return 3;
                }
            }
        }
        switch (joker) {
            case 1: return 4;
            case 2: return 6;
            case 3: return 7;
        }
        return 2;
    }

    if (highest_count == 3) {
        for (int i = 0; i < char_size; ++i) {
            if (counts[i].count == 2) {
                return 5;
            }
        }
        switch (joker) {
            case 1: return 6;
            case 2: return 7;
        }
        return 4;
    }

    return highest_count + 2 + joker;
}

int compareHands(Hand hand1, Hand hand2) {
    int score1 = calcScore(hand1);
    int score2 = calcScore(hand2);
    if (score1 < score2) {
        return -1;
    }
    if (score1 > score2) {
        return 1;
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 13; ++j) {
            if (hand1.hand[i] == char_scores[j]) {
                score1 = j;
            }
            if (hand2.hand[i] == char_scores[j]) {
                score2 = j;
            }
        }   
        if (score1 < score2) {
            return -1;
        }   
        if (score1 > score2) {
            return 1;
        }
    }

    return 0;
}

int compareHands2(Hand hand1, Hand hand2) {
    int score1 = calcScore2(hand1);
    int score2 = calcScore2(hand2);
    if (score1 < score2) {
        return -1;
    }
    if (score1 > score2) {
        return 1;
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 13; ++j) {
            if (hand1.hand[i] == char_scores2[j]) {
                score1 = j;
            }
            if (hand2.hand[i] == char_scores2[j]) {
                score2 = j;
            }
        }   
        if (score1 < score2) {
            return -1;
        }   
        if (score1 > score2) {
            return 1;
        }
    }

    return 0;
}

void sort(Hand hands[]) {
    int i, j;
    bool swapped;
    for (i = 0; i < size - 1; ++i) {
        swapped = false;
        for (j = 0; j < size - i - 1; ++j) {
            if (compareHands(hands[j], hands[j + 1]) == 1) {
                std::swap(hands[j], hands[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void sort2(Hand hands[]) {
    int i, j;
    bool swapped;
    for (i = 0; i < size - 1; ++i) {
        swapped = false;
        for (j = 0; j < size - i - 1; ++j) {
            if (compareHands2(hands[j], hands[j + 1]) == 1) {
                std::swap(hands[j], hands[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

int solution1() {
    int sum = 0; 
    for (int i = 0; i < size; ++i) {
        sorted_hands[i] = hands[i];
    }

    sort(sorted_hands);

    for (int i = 0; i < size; ++i) {
        sum += sorted_hands[i].score * (i + 1);
    }

    for (int i = 0; i < size; ++i) {
        std::cout << (i + 1) << "\t Hand(hand: " << sorted_hands[i].hand << ", score: " << sorted_hands[i].score << ")\t| " << calcScore(sorted_hands[i]) << std::endl;
    }
    return sum;
}

int solution2() {
    int sum = 0; 
    for (int i = 0; i < size; ++i) {
        sorted_hands[i] = hands[i];
    }

    sort2(sorted_hands);

    for (int i = 0; i < size; ++i) {
        sum += sorted_hands[i].score * (i + 1);
    }

    for (int i = 0; i < size; ++i) {
        std::cout << (i + 1) << "\t Hand(hand: " << sorted_hands[i].hand << ", score: " << sorted_hands[i].score << ")\t| " << calcScore2(sorted_hands[i]) << std::endl;
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;

    int number;
    int i;
    while (getline(file, line)) {
        i = 0;
        Hand hand;
        hand.hand = "";
        while (i < line.length() && line[i] != ' ') {
            hand.hand += line[i];
            i++;
        }
        line = line.substr(i);
        hand.score = stoi(line);
        hands[size] = hand;
        size++;
    }
    file.close();

    for (int i = 0; i < size; ++i) {
        std::cout << "Hand(hand: " << hands[i].hand << ", score: " << hands[i].score << ")" << std::endl;
    }
    std::cout << "--------------" << std::endl;
    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
