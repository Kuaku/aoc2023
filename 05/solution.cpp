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
struct RangeMap {
    long long source_start;
    long long dest_start;
    long long range;
};

struct Range {
    long long start;
    long long range;
};

void printRange(Range range) {
    std::cout << "Range(start: " << range.start << ", range: " << range.range << ")" << std::endl;
}

void printRangeMap(RangeMap map) {
    std::cout << "RangeMap(source_start: " << map.source_start << ", dest_start: " << map.dest_start << ", range: " << map.range << ")" << std::endl;
}

void printRangeVector(std::vector<Range> list) {
    for (int i = 0;  i < list.size(); ++i) {
        printRange(list[i]);
    }
}

std::vector<long long> seeds;
std::vector<RangeMap> ranges[7];
Range rangeOut;
Range rangeIn1;
Range rangeIn2;

bool isInRange(long long source, RangeMap range) {
    return range.source_start <= source && range.source_start + range.range > source;
}

long long findDestInRange(long long source, int range) {
    for (int i = 0; i < ranges[range].size(); ++i) {
        if (isInRange(source, ranges[range][i])) {
            return ranges[range][i].dest_start + (source - ranges[range][i].source_start); 
        }
    }
    return source;
}

long long convertSeedToLocation(long long seed) {
    for (int i = 0; i < 7; ++i) {
        seed = findDestInRange(seed, i);
    }
    return seed;
}

long long solution1() {
    long long lowest = INT_MAX;
    for (int i = 0; i < seeds.size(); ++i) {
        long long location = convertSeedToLocation(seeds[i]);
        lowest = location < lowest ? location : lowest;
    }
    return lowest;
}

bool canGenerateRange(Range range, RangeMap map) {
    return isInRange(range.start, map) || 
        isInRange(range.start + range.range, map) || 
        (range.start < map.source_start && range.start + range.range >= map.source_start + map.range);
}


std::vector<Range> getPossibleRanges(Range range, int rangeMap) {
    std::vector<Range> in = { range };
    std::vector<Range> out;

    for (int i = 0; i < ranges[rangeMap].size(); ++i) {
        RangeMap map = ranges[rangeMap][i];
        std::vector<Range> temp;
        for (int j = 0; j < in.size(); ++j) {
            Range range = in[j];
            if (canGenerateRange(range, map)) {
                if (range.start < map.source_start && range.start + range.range < map.source_start + map.range) {
                    rangeOut.start = map.dest_start;
                    rangeOut.range = (range.start + range.range) - map.source_start;
                    rangeIn1.start = range.start;
                    rangeIn1.range = map.source_start - range.start;
                    if (rangeIn1.range != 0) {
                        temp.push_back(rangeIn1);
                    }
                } else if (range.start + range.range > map.source_start + map.range && range.start >= map.source_start) {
                    rangeOut.start = map.dest_start + (range.start -  map.source_start);
                    rangeOut.range = (map.source_start + map.range) - range.start;
                    rangeIn1.start = map.source_start + map.range;
                    rangeIn1.range = (range.start + range.range) - (map.source_start + map.range);
                    if (rangeIn1.range != 0) {
                        temp.push_back(rangeIn1);
                    }
                } else if (range.start <= map.source_start && range.start + range.range >= map.source_start + map.range) {
                    rangeOut.start = map.dest_start;
                    rangeOut.range = map.range;
                    rangeIn1.start = range.start;
                    rangeIn1.range = map.source_start - range.start;
                    rangeIn2.start = map.source_start + map.range;
                    rangeIn2.range = (range.start + range.range) - (map.source_start + map.range); 
                    if (rangeIn1.range != 0) {
                        temp.push_back(rangeIn1);
                    }
                    if (rangeIn2.range != 0) {
                        temp.push_back(rangeIn2);
                    }
                } else {
                    rangeOut.start = map.dest_start + (range.start -  map.source_start);
                    rangeOut.range = range.range;
                }
                out.push_back(rangeOut);
            } else {
                rangeIn1.start = range.start;
                rangeIn1.range = range.range;
                temp.push_back(rangeIn1);
            }
        }
        in = temp;
    }
    if (in.size() > 0) {
        out.insert(out.end(), in.begin(), in.end());
    }
    return out;
}

int solution2() {
    long long lowest = LLONG_MAX;
    std::vector<Range> ranges;
    std::vector<Range> swap;
    for (int i = 0; i < seeds.size(); i += 2) {
        Range range;
        range.start = seeds[i];
        range.range = seeds[i + 1];
        ranges.push_back(range); 
    }
    for (int i = 0; i < 7; i++) {
        swap.clear();
        for (int j = 0; j < ranges.size(); ++j) {
            std::vector<Range> result = getPossibleRanges(ranges[j], i);
            swap.insert(swap.end(), result.begin(), result.end());
        }
        ranges = swap;
    }
    
    for (int i = 0; i < ranges.size(); ++i) {
        if (ranges[i].start  < lowest) {
            lowest = ranges[i].start;
        }
    }

    return lowest;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;

    int number;
    char *number_size;
    getline(file, line);
    line = line.substr(7);
    while (line.length() > 0) {
        seeds.push_back(strtoll(line.c_str(), &number_size, 10));
        line = line.substr(number_size - line.c_str());
    }
    getline(file, line);
    getline(file, line);
    int i = 0; 
    while (getline(file, line)) {
        if (line == "") {
            getline(file, line);
            i++;
            continue;
        }
        RangeMap range;
        
        range.dest_start = strtoll(line.c_str(), &number_size, 10);
        line = line.substr(number_size - line.c_str());
        range.source_start = strtoll(line.c_str(), &number_size, 10);
        line = line.substr(number_size - line.c_str());
        range.range = strtoll(line.c_str(), &number_size, 10);
        line = line.substr(number_size - line.c_str());
        ranges[i].push_back(range);
    }
    file.close();

    /*std::cout << "Seeds:";
    for (int i = 0; i < seeds.size(); ++i) {
        std::cout << " " << seeds[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < 7; ++i) {
        std::cout << "Range " << (i + 1) << ":" << std::endl;
        for (int j = 0; j < ranges[i].size(); ++j) {
            std::cout << ranges[i][j].dest_start << " " << ranges[i][j].source_start << " " << ranges[i][j].range << std::endl;
        }
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
