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
#define RULE_CAP 128

struct Condition {
    char value;
    bool bigger;
    bool accept = false;
    bool reject = false;
    bool no_con = false;
    std::string next_label;
    int v;
};

struct RuleSet {
    std::string label;
    int size = 0;
    Condition cons[RULE_CAP];
};

RuleSet sets[CAPCITY];
int size = 0;

struct Part {
    int x, m, a, s;
};
Part parts[CAPCITY];
int part_c = 0;

std::string fill_to(int v, int l) {
    std::string out = std::to_string(v);
    while (out.length() < l) {
        out = "0" + out;
    }
    return out;
}

int start = 0;

int find_label(std::string label) {
    for (int i = 0; i < size; ++i) {
        if (sets[i].label == label) {
            return i;
        }
    }
    return -1;
}

long long solution1() {
    long long sum = 0;
    for (int i = 0; i < part_c; ++i) {
        bool accepted = false;
        bool rejected = false;
        int j = start;
        while (!accepted && !rejected) {
            for (int c = 0; c < sets[j].size; ++c) {
                if (sets[j].cons[c].no_con) {
                    if (sets[j].cons[c].accept) {
                        accepted = true;
                        break;
                    } else if (sets[j].cons[c].reject) {
                        rejected = true;
                        break;
                    } else {
                        j = find_label(sets[j].cons[c].next_label);
                        break;
                    }
                } 
                bool is_set = false;
                if (sets[j].cons[c].bigger) {
                    switch (sets[j].cons[c].value)
                    {
                        case 'a': is_set = sets[j].cons[c].v < parts[i].a; break;
                        case 'm': is_set = sets[j].cons[c].v < parts[i].m; break;
                        case 's': is_set = sets[j].cons[c].v < parts[i].s; break;
                        case 'x': is_set = sets[j].cons[c].v < parts[i].x; break;
                    }
                } else {
                    switch (sets[j].cons[c].value)
                    {
                        case 'a': is_set = sets[j].cons[c].v > parts[i].a; break;
                        case 'm': is_set = sets[j].cons[c].v > parts[i].m; break;
                        case 's': is_set = sets[j].cons[c].v > parts[i].s; break;
                        case 'x': is_set = sets[j].cons[c].v > parts[i].x; break;
                    }
                }
                if (is_set) {
                    if (sets[j].cons[c].accept) {
                        accepted = true;
                        break;
                    } else if (sets[j].cons[c].reject) {
                        rejected = true;
                        break;
                    } else {
                        j = find_label(sets[j].cons[c].next_label);
                        break;
                    }
                }
            }
        }
        if (accepted) {
            sum += parts[i].x + parts[i].a + parts[i].m + parts[i].s;
        }
    }
    return sum;
}
struct Range {
    int x_s = 0, x_e = 4000;
    int s_s = 0, s_e = 4000;
    int m_s = 0, m_e = 4000;
    int a_s = 0, a_e = 4000;
    std::vector<std::string> labels;
};

std::vector<Range> get_accepted_range(Range in, int index) {
    in.labels.push_back(sets[index].label);
    RuleSet set = sets[index];
    std::vector<Range> accepted_ranges;
    for (int c = 0; c < set.size; ++c) {
        if (set.cons[c].no_con) {
            if (set.cons[c].accept) {
                accepted_ranges.push_back(in);
            } else if (!set.cons[c].reject) {
                std::vector<Range> res = get_accepted_range(in, find_label(set.cons[c].next_label));
                accepted_ranges.insert(accepted_ranges.end(), res.begin(), res.end());
            }
        } else {
            Range hits = {in.x_s, in.x_e, in.s_s, in.s_e, in.m_s, in.m_e, in.a_s, in.a_e, in.labels};
            bool has_hit = false;
            if (set.cons[c].bigger) {
                switch (set.cons[c].value)
                {
                    case 's': has_hit = hits.s_e > set.cons[c].v; if (!has_hit) break; hits.s_s = std::max(set.cons[c].v + 1, hits.s_s); in.s_e = set.cons[c].v; break;
                    case 'x': has_hit = hits.x_e > set.cons[c].v; if (!has_hit) break; hits.x_s = std::max(set.cons[c].v + 1, hits.x_s); in.x_e = set.cons[c].v; break;
                    case 'm': has_hit = hits.m_e > set.cons[c].v; if (!has_hit) break; hits.m_s = std::max(set.cons[c].v + 1, hits.m_s); in.m_e = set.cons[c].v; break;
                    case 'a': has_hit = hits.a_e > set.cons[c].v; if (!has_hit) break; hits.a_s = std::max(set.cons[c].v + 1, hits.a_s); in.a_e = set.cons[c].v; break;
                }
            } else {
                switch (set.cons[c].value)
                {
                    case 's': has_hit = hits.s_s < set.cons[c].v; if (!has_hit) break; hits.s_e = std::min(set.cons[c].v-1, hits.s_e); in.s_s = set.cons[c].v; break;
                    case 'x': has_hit = hits.x_s < set.cons[c].v; if (!has_hit) break; hits.x_e = std::min(set.cons[c].v-1, hits.x_e); in.x_s = set.cons[c].v; break;
                    case 'm': has_hit = hits.m_s < set.cons[c].v; if (!has_hit) break; hits.m_e = std::min(set.cons[c].v-1, hits.m_e); in.m_s = set.cons[c].v; break;
                    case 'a': has_hit = hits.a_s < set.cons[c].v; if (!has_hit) break; hits.a_e = std::min(set.cons[c].v-1, hits.a_e); in.a_s = set.cons[c].v; break;
                }
            }
            if (has_hit) {
                if (set.cons[c].accept) {
                    accepted_ranges.push_back(hits);
                } else if (!set.cons[c].reject) {
                    std::vector<Range> res = get_accepted_range(hits, find_label(set.cons[c].next_label));
                    accepted_ranges.insert(accepted_ranges.end(), res.begin(), res.end());
                }
            }
        }
    }
    return accepted_ranges;
}

long long solution2() {
    long long sum = 0;
    Range test = {1, 4000, 1, 4000, 1, 4000, 1, 4000};
    std::vector<Range> test2 = get_accepted_range(test, start);
    for (int i = 0; i < test2.size(); ++i) {
        sum += ((long long)(test2[i].x_e) - (long long)(test2[i].x_s) + 1) * ((long long)(test2[i].m_e) - (long long)(test2[i].m_s) + 1) * ((long long)(test2[i].a_e) - (long long)(test2[i].a_s) + 1) * ((long long)(test2[i].s_e) - (long long)(test2[i].s_s) + 1);
    }
    return sum;
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        if (line == "")
            break;
        RuleSet next;
        std::string label = "";
        int i = 0;
        while (line[i] != '{') {
            i++;
        }
        next.label = line.substr(0, i);
        i++;
        if (next.label == "in") {
            start = size;
        }
        while(i < line.length() && line[i] != '}') {
            Condition con;
            if (line[i] == 'A') {
                con.accept = true;
                con.no_con = true;
                i++;
            } else if (line[i] == 'R') {
                con.reject = true;
                con.no_con = true;
                i++;
            } else {
                if (line[i+1] != '<' && line[i+1] != '>') {
                    con.no_con = true;
                    int j = i;
                    while (line[j] != '}') {
                        j++;
                    }
                    con.next_label = line.substr(i, j-i);
                    i = j;
                } else {
                    size_t number_size;
                    con.value = line[i];
                    con.bigger = line[i+1] == '>';
                    con.v = stoi(line.substr(i+2),&number_size);
                    int j = i + 2 + number_size + 1;
                    if (line[j] == 'A') {
                        con.accept = true;
                        i = j + 2;
                    } else if (line[j] == 'R') {
                        con.reject = true;
                        i = j + 2;
                    } else {
                        while (line[j] != ',') {
                            j++;
                        }
                        con.next_label = line.substr(i + 2 + number_size + 1, j-(i + 2 + number_size + 1));
                        i = j + 1;
                    }
                }
            }
            next.cons[next.size] = con;
            next.size++;
        }
        sets[size] = next;
        size++;
    }
    while (getline(file, line)) {
        Part part;
        size_t number_size;
        line = line.substr(3);
        part.x = stoi(line, &number_size);
        line = line.substr(number_size + 3);
        part.m = stoi(line, &number_size);
        line = line.substr(number_size + 3);
        part.a = stoi(line, &number_size);
        line = line.substr(number_size + 3);
        part.s = stoi(line, &number_size);

        parts[part_c] = part;
        part_c++;
    }
    file.close();

    /*for (int i = 0; i < size; ++i) {
        std::cout <<    "RuleSet(label: "<<sets[i].label<<", [";
        for (int j = 0; j < sets[i].size; ++j) {
            Condition con = sets[i].cons[j];
            std::cout << "(value: "<<con.value<<",bigger: "<<con.bigger<<",accept: "<<con.accept<<",reject: "<<con.reject<<",no_con: "<<con.no_con<<", next_label: "<<con.next_label<<",v: "<<con.v<<"), ";
        };
        std::cout << "])" << std::endl;
    }*/

    std::cout << "Solution 1: " << solution1() << std::endl;
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
