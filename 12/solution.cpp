#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <numeric>
#include <chrono>
#include <set>
#include <tuple>
#include <thread>

#if 0
    #define FILE_PATH "example.txt"
#else
    #define FILE_PATH "input.txt"
#endif

#define CAPCITY 1024

struct Game {
    std::string field;
    std::vector<int> numbers;
};
Game games[CAPCITY];
int size = 0;

bool isCorrect(std::string field, std::vector<int> numbers) {
    std::vector<int> numb;
    int count = 0;
    for (int i = 0; i < field.length(); ++i) {
        if (field[i] == '#') {
            count++;
        } else if (count > 0) {
            numb.push_back(count);
            count = 0;
        }
    }
    if (count > 0) {
        numb.push_back(count);
    } 

    if (numb.size() != numbers.size()) {
        return false;
    }

    for (int i = 0; i < numbers.size(); ++i) {
        if (numb[i] != numbers[i]) {
            return false;
        }
    }

    return true;
}

int rec(std::string field, std::vector<int> numbers, int start) {
    for (int i = start;  i < field.length(); ++i) {
        if (field[i] == '?') {
            field[i] = '.';
            int temp = rec(field, numbers, i);
            field[i] = '#';
            return temp + rec(field, numbers, i);
        }
    }
    return isCorrect(field, numbers) ? 1 : 0;
}

int rec2(std::string field, std::vector<int> numbers, int start) {

    for (int i = start;  i < field.length(); ++i) {
        if (field[i] == '?') {
            field[i] = '.';
            int temp = rec(field, numbers, i);
            field[i] = '#';
            return temp + rec(field, numbers, i);
        }
    }
    return isCorrect(field, numbers) ? 1 : 0;
}

#define Threads 8
std::vector<std::string> fields[Threads];
std::vector<std::vector<int>> numbers_c[Threads];
std::vector<long long> res_c[Threads];
int contains(std::string field, std::vector<int> numbers, int thread) {
    for (int i = 0; i < fields[thread].size(); ++i) {
        if (fields[thread][i].compare(field) == 0) {
            bool found = true;
            if (numbers_c[i].size() != numbers.size()) {
                continue;
            }
            for (int j = 0; j < numbers.size(); ++j) {
                if (numbers[j] != numbers_c[thread][i][j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }
    }
    return -1;
}
int a = 0;
void addCache(std::string field, std::vector<int> numbers, long long res, int d, int thread) {
        if (d < 5) {
            return;
        }
        a++;
        fields[thread].push_back(field);
        std::vector<int> temp;
        for (int i = 0; i < numbers.size(); ++i) {
            temp.push_back(numbers[i]);
        }
        numbers_c[thread].push_back(temp);
        res_c[thread].push_back(res);
}

long long rec3(std::string field, std::vector<int> numbers, int d, int thread) {
    std::cout << "--------" << std::endl;
    int id = contains(field, numbers, thread);
    if (id != -1) {
        return res_c[thread][id];
    }
    
    int sum = 0;
    int count = 0;
    
    for (int i = 0; i < numbers.size(); ++i) {
        count += numbers[i];
    }
    count += numbers.size() - 1;
    if (count != -1 && count > field.length()) {
            std::cout << "2" << std::endl;
        addCache(field, numbers, 0, d, thread);
        return 0;
    }

    count = 0;
    int i = 0;
    for (i = 0; i < field.length(); ++i) {
        if (count > 0 && field[i] == '.' || field[i] == '?') {
            break;
        }
        if (field[i] == '#') {
            count++;
        }
    }

    if (field[i] == '?') {
        field[i] = '.';
        sum += rec3(field, numbers, d++, thread);
        field[i] = '#';

        
        long long res = sum + rec3(field, numbers, d++, thread);
        field[i] = '?';
            std::cout << "3" << std::endl;
        addCache(field, numbers, res, d, thread);

        return res;
    }

    if (numbers.size() == 0 && count == 0) {
            std::cout << "4" << std::endl;
        addCache(field, numbers, 1, d, thread);
        return 1;
    }
    if (numbers.size() == 0 && count != 0) {
            std::cout << "5" << std::endl;
        addCache(field, numbers, 0, d, thread);
        return 0;
    }
    if (numbers.size() != 0 && count == 0) {
            std::cout << "6" << std::endl;
        addCache(field, numbers, 0, d, thread);
        return 0;
    }

    if (count > 0) {
        if (numbers[0] == count) {
            std::vector<int> temp;
            for (int j = 1; j < numbers.size(); ++j) {
                temp.push_back(numbers[j]);
            }
            long long res = rec3(field.substr(i), temp, d++, thread);
            std::cout << "7" << std::endl;
            addCache(field, numbers, res, d, thread);
            return res;
        } else {
            std::cout << "8" << std::endl;
            addCache(field, numbers, 0, d, thread);
            return 0;
        }
    }

    addCache(field, numbers, 1, d, thread);
    return 1;
}

int solution1() {
    int sum = 0; 
    for (int i = 0; i < size; ++i) {
        sum += rec3(games[i].field, games[i].numbers, 0, 0);
    }
    return sum;
}




#define CacheLimit 10000000


struct CacheEntry {
    std::string field;
    std::vector<int> numbers;
    int s_f;
    int s_n;
    long long result;
};

struct Cache {
    CacheEntry *cache;
    int cacheSize;
};

Cache o_caches[Threads]; 
long long results[Threads];
std::thread threads[Threads];
int indexe[Threads];

void printCache(int thread) {
    std::cout << "==============================" << std::endl;
    for (int i = 0; i < o_caches[thread].cacheSize; ++i) {
        std::cout << "Cache Entry " << i << ":" << std::endl;
        std::cout << "Field: " << o_caches[thread].cache[i].field << std::endl;
        std::cout << "Numbers: ";
        for (int j = 0; j < o_caches[thread].cache[i].numbers.size(); ++j) {
            std::cout << o_caches[thread].cache[i].numbers[j] << " ";
        }
        std::cout << std::endl;
        std::cout << "s_f: " << o_caches[thread].cache[i].s_f << std::endl;
        std::cout << "s_n: " << o_caches[thread].cache[i].s_n << std::endl;
        std::cout << "Result: " << o_caches[thread].cache[i].result << std::endl;
        std::cout << std::endl;
    }
    std::cout << "==============================" << std::endl;
}

int isInCache(std::string field, std::vector<int> numbers, int s_f, int s_n, int thread) {
    
    //printCache(thread);
    for (int i = 0; i < o_caches[thread].cacheSize; ++i) {
        if (field.length() - s_f != o_caches[thread].cache[i].field.length() - o_caches[thread].cache[i].s_f) {
            continue;
        }
        if (o_caches[thread].cache[i].numbers.size() - o_caches[thread].cache[i].s_n != numbers.size() - s_n) {
            continue;
        }
        bool found = true;
        
        for (int j = 0; j < field.size() - s_f; ++j) {
            if (field[s_f + j] != o_caches[thread].cache[i].field[j + o_caches[thread].cache[i].s_f] || !found) {
                found = false;
                break;
            }
        }

        for (int j = 0; j < numbers.size() - s_n; ++j) {
            if (numbers[s_n + j] != o_caches[thread].cache[i].numbers[j + o_caches[thread].cache[i].s_n] || !found) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return -1;
}

void addToCache(std::string field, std::vector<int> numbers, int s_f, int s_n, long long  result, int thread) {
    o_caches[thread].cache[o_caches[thread].cacheSize].field = field;
    for (int i = 0; i < numbers.size(); ++i) {
        o_caches[thread].cache[o_caches[thread].cacheSize].numbers.push_back(numbers[i]);
    }
    o_caches[thread].cache[o_caches[thread].cacheSize].result = result;
    o_caches[thread].cache[o_caches[thread].cacheSize].s_f = s_f;
    o_caches[thread].cache[o_caches[thread].cacheSize].s_n = s_n;
    o_caches[thread].cacheSize++;
}
long long rec4_call(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread);
long long rec4(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread);

long long rec4_call(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread) {
    int id = isInCache(field, numbers, s_f, s_n, thread);
    if (id != -1) {
        return o_caches[thread].cache[id].result;
    }

    long long res = rec4(field, numbers, s_f, s_n, d, thread);
    addToCache(field, numbers, s_f, s_n, res, thread);
    return res;
}
long long rec4(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread) {
    //https://github.com/zivnadel/advent-of-code/blob/master/2023/python/day12.py 
    if (numbers.size() - s_n == 0) {
        for (int i = s_f; i < field.length(); ++i) {
            if (field[i] == '#') {
                return 0;
            }
        }
        return 1;
    }

    int count = 0;    
    for (int i = s_n; i < numbers.size(); ++i) {
        count += numbers[i];
    }
    if (field.length() - s_f < count) {
        return 0;
    }
    if (field[s_f] == '.') {
        return rec4_call(field, numbers, s_f + 1, s_n, d++, thread);
    }

    bool test = true;
    for (int i = s_f; i < s_f + numbers[s_n]; ++i) {
        if (field[i] == '.') {
            test = false;
            break;
        }
    }
    test = test && (field.length() - s_f > numbers[s_n] && field[s_f + numbers[s_n]] != '#' || field.length() - s_f <= numbers[s_n]);

    long long here = test ? rec4_call(field, numbers, s_f + numbers[s_n] + 1, s_n + 1, d++, thread) : 0; 

    long long next = field[s_f] == '?' ? rec4_call(field, numbers, s_f + 1, s_n, d++, thread) : 0;
    
    return next + here;
}


long long rec5_call(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread);
long long rec5(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread);

long long rec5_call(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread) {
    int id = isInCache(field, numbers, s_f, s_n, thread);
    if (id != -1) {
        return o_caches[thread].cache[id].result;
    }

    long long res = rec5(field, numbers, s_f, s_n, d, thread);
    addToCache(field, numbers, s_f, s_n, res, thread);
    return res;
}
int calls = 0;
long long rec5(std::string field, std::vector<int> numbers, int s_f, int s_n, int d, int thread) {
    if (numbers.size() - s_n == 0) {
        for (int i = s_f; i < field.length(); ++i) {
            if (field[i] == '#') {
                return 0;
            }
        }
        return 1;
    }

    int count = 0;    
    for (int i = s_n; i < numbers.size(); ++i) {
        count += numbers[i];
    }
    count += numbers.size() - 1 - s_n;
    if (count != -1 && count > field.length() - s_f) {
        return 0;
    }

    int next_run = 0;
    int i;
    for (i = s_f; i < field.length(); ++i) {
        if ((next_run > 0 && field[i] == '.') || field[i] == '?') {
            break;
        }
        if (field[i] == '#') {
            next_run++;
        }
        if (next_run > numbers[s_n]) {
            return 0;
        }
    }

    if (field[i] == '?') {
        long long res = 0;
        if (next_run == 0) {
            field[i] = '.';
            res += rec4_call(field, numbers, s_f + 1, s_n, d++, thread);
        }
        if (next_run == numbers[s_n]) {
            field[i] = '.';
            res += rec4_call(field, numbers, i + 1, s_n + 1, d++, thread);
        }
        if (next_run < numbers[s_n]) {
            field[i] = '#';
            res += rec4_call(field, numbers, s_f, s_n, d++, thread);
        }
        return res;
    }
    if (next_run != numbers[s_n]) {
        return 0;
    }
    return rec4_call(field, numbers, i + 1, s_n + 1, d++, thread);
}

void thread(int start, int end, int thread) {
    //std::string test = "Start: " + std::to_string(start) + "|End: " + std::to_string(end) + "|Thread: " + std::to_string(thread);
    //std::cout << test << std::endl;
    //return; 
    std::string number = "";
    for (int i = start; i < end; ++i) {
        indexe[thread] = i-start;
        //number = std::to_string(i) + "\n";
        //std::cout << number;
        std::string field = games[i].field;
        std::vector<int> numbers;
        for (int j = 0; j < games[i].numbers.size(); ++j) {
            numbers.push_back(games[i].numbers[j]);
        }

        for (int x = 0; x < 4; ++x) {
            field += "?" + games[i].field;
            for (int j = 0; j < games[i].numbers.size(); ++j) {
                numbers.push_back(games[i].numbers[j]);
            }   
        }
        //o_caches[thread].cacheSize = 0;
        long long result = rec4_call(field, numbers,0 ,0 , 0, thread);
        //std::cout << "Res: " << result << std::endl;
        results[thread] += result;
    }
}

void thread2(int start, int end, int thread) {
    //std::string test = "Start: " + std::to_string(start) + "|End: " + std::to_string(end) + "|Thread: " + std::to_string(thread);
    //std::cout << test << std::endl;
    //return; 
    std::string number = "";
    for (int i = start; i < end; ++i) {
        indexe[thread] = i-start;
        //number = std::to_string(i) + "\n";
        //std::cout << number;
        std::string field = games[i].field;
        std::vector<int> numbers;
        for (int j = 0; j < games[i].numbers.size(); ++j) {
            numbers.push_back(games[i].numbers[j]);
        }

        for (int x = 0; x < 4; ++x) {
            field += "?" + games[i].field;
            for (int j = 0; j < games[i].numbers.size(); ++j) {
                numbers.push_back(games[i].numbers[j]);
            }   
        }
        //o_caches[thread].cacheSize = 0;
        long long result = rec5_call(field, numbers,0 ,0 , 0, thread);
        //std::cout << "Res: " << result << std::endl;
        results[thread] += result;
    }
}

bool finished = false;

void report_thread() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string out;
    while (!finished) {
        int sum = 0;
        for (int i = 0; i < Threads; ++i) {
            sum += indexe[i];
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        out = "Report: (" + std::to_string(sum) + "/" + std::to_string(size) + ")\n";
        out += "Time collapsed: " + std::to_string(duration.count()) + "s\n";
        for (int i = 0; i < Threads; ++i) {
            out += "T(" + std::to_string(i) + "): " + std::to_string(indexe[i]) + "\n";
        }
        out += "Calls: " + std::to_string(calls) + "\n";
        std::cout << out;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return;
}
std::thread report;
long long solution2() {
    
    for (int i = 0; i < Threads; ++i) {
        o_caches[i].cache = new CacheEntry[CacheLimit];
        o_caches[i].cacheSize = 0;
        results[i] = 0;
        indexe[i] = 0;
    }
    int division_size = size / Threads;
    int rest = size % Threads;
    int start = 0;
    for (int i = 0; i < Threads - 1; ++i) {
        threads[i] = std::thread(thread, start, start + division_size, i);
        start += division_size;
    } 
    threads[Threads - 1] = std::thread(thread, start, start + division_size + rest, Threads - 1);
    long long sum = 0;
    for (int i = 0; i < Threads; ++i) {
        threads[i].join();
        sum += results[i];
    }
    return sum;
}
long long solution3() {
    
    for (int i = 0; i < Threads; ++i) {
        o_caches[i].cache = new CacheEntry[CacheLimit];
        o_caches[i].cacheSize = 0;
        results[i] = 0;
        indexe[i] = 0;
    }
    int division_size = size / Threads;
    int rest = size % Threads;
    int start = 0;
    for (int i = 0; i < Threads - 1; ++i) {
        threads[i] = std::thread(thread2, start, start + division_size, i);
        start += division_size;
    } 
    threads[Threads - 1] = std::thread(thread2, start, start + division_size + rest, Threads - 1);
    long long sum = 0;
    for (int i = 0; i < Threads; ++i) {
        threads[i].join();
        sum += results[i];
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        int i = 0;
        size_t number_size;
        Game game;
        game.field =  "";
        while(line[i] != ' ') {
            game.field += line[i];
            i++;
        }
        line = line.substr(i + 1);
        while (line.length() > 0) {
            game.numbers.push_back(stoi(line, &number_size));
            line = line.substr(std::min(line.length(), number_size + 1));
        }

        games[size] = game;
        size++;
    }
    file.close();

    /*for (int i = 0; i < size; ++i) {
        std::cout << "Game(field: " << games[i].field << ", numbers: ";
        for (int j = 0; j < games[i].numbers.size(); ++j) {
            std::cout << games[i].numbers[j] << ", ";
        }
        std::cout << ")" << std::endl;  
    }*/

    //std::cout << "Solution 1: " << solution1() << std::endl;
    
    auto t1 = std::chrono::high_resolution_clock::now();
    long long result = solution2();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    
    t1 = std::chrono::high_resolution_clock::now();
    long long result2 = solution3();
    t2 = std::chrono::high_resolution_clock::now();
    auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    
    std::cout << "Solution 2: " << result << "(" << ms_int1.count()  << "ms)" << std::endl;
    std::cout << "Solution 3: " << result2 << "(" << ms_int2.count()  << "ms)" << std::endl;
    return 0;
}
