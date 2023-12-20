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
#define MOD_CAPCITY 128
#define SIGNAL_CAPACITY 10000000

enum Type {
    BROADCAST,
    FLIP_FLOP,
    CONJUNCTION
};

struct Module {
    Type type;
    std::string label;
    bool state[MOD_CAPCITY];
    int out[MOD_CAPCITY];
    int in[MOD_CAPCITY];
    int out_size = 0, in_size = 0;
};

struct Signal {
    int source, target;
    bool state;
};

Module modules[CAPCITY];
int size = 0;
int broadcast = 0;
Signal signals[SIGNAL_CAPACITY];
int signal_size = 0;

void add_signal(bool state, int target, int source) {
    signals[signal_size] = {source, target, state};
    signal_size++;
}

int get_module_index(std::string label) {
    for (int i = 0; i < size; ++i) {
        if (modules[i].label == label) {
            return i;
        }
    }
    int o = size;
    modules[size].label = label;
    size++;
    return o;
}

void add_output_to_module(int index, std::string label) {
    int index2 = get_module_index(label);
    modules[index].out[modules[index].out_size] = index2;
    modules[index].out_size++;
    modules[index2].in[modules[index2].in_size] = index;
    modules[index2].in_size++;
}

long long solution1() {
    broadcast = get_module_index("broadcaster");
    int s = 0;
    for (int k = 0; k < 1000; k++) {
        int temp = s;
        add_signal(false, broadcast, -1);
        while (s < signal_size) {
            Module *mod = &modules[signals[s].target];
            if (mod->type == BROADCAST) {
                for (int i = 0; i < mod->out_size; ++i) {
                    add_signal(signals[s].state, mod->out[i], signals[s].target);
                }
            } else if (mod->type == FLIP_FLOP) {
                if (!signals[s].state) {
                    mod->state[0] = !mod->state[0];
                    for (int i = 0; i < mod->out_size; ++i) {
                        add_signal(mod->state[0], mod->out[i], signals[s].target);
                    }   
                }
            } else {
                for (int i = 0; i < mod->in_size; ++i) {
                    if (mod->in[i] == signals[s].source) {
                        mod->state[i] = signals[s].state;
                        break;
                    }
                }

                bool all_high = true;
                for (int i = 0; i < mod->in_size; ++i) {
                    if (!mod->state[i]) {
                        all_high = false;
                        break;
                    }
                }
                for (int i = 0; i < mod->out_size; ++i) {
                    add_signal(!all_high, mod->out[i], signals[s].target);
                } 
            }
            s++;
        }

        /*for (int i = temp; i < signal_size; ++i) {
            if (signals[i].source == -1) {
                std::cout << "button";
            } else {
                std::cout << modules[signals[i].source].label; 
            }
            std::cout << " -" << (signals[i].state ? "high" : "low") << "-> " << modules[signals[i].target].label << std::endl;
        }
        std::cout << std::endl;*/
    }

    long long high = 0;
    long long low = 0;
    for (int i = 0; i < signal_size; ++i) {
        if (signals[i].state) {
            high++;
        } else {
            low++;
        }
    }
    
    return high * low;
}
long long gcd(long long a, long long b) {
    long long r;
    while (a%b > 0) {
        r = a % b;
        a = b;
        b = r;
    }
    return b;
}
long long lcm(long long a, long long b) {
    return (a * b) / gcd(a, b);
}

long long lcm(long long a, long long b, long long c, long long d) {
    return lcm(lcm(lcm(a, b), c), d);
}

long long solution2() {
    broadcast = get_module_index("broadcaster");
    int rx = get_module_index("rx");
    int qt = get_module_index("qt");
    int mp = get_module_index("mp");
    int ng = get_module_index("ng");
    int qb = get_module_index("qb");
    int qt_s = 0;
    int mp_s = 0;
    int ng_s = 0;
    int qb_s = 0;
    int s = 0;
    long long b = 0;
    while (true) {
        s=0;
        signal_size=0;
        b++;
        if (b % 100000 == 0) {
            //std::cout << b << std::endl;
        }
        add_signal(false, broadcast, -1);
        while (s < signal_size) {
            if (signals[s].source == qt && signals[s].state && !qt_s) {
                qt_s = b;
            }
            if (signals[s].source == mp && signals[s].state && ! mp_s) {
                mp_s = b;
            }
            if (signals[s].source == ng && signals[s].state && !ng_s) {
                ng_s = b;
            }
            if (signals[s].source == qb && signals[s].state && !qb_s) {
                qb_s = b;
            }
            if (qb_s != 0 && mp_s != 0 && ng_s != 0 && qt_s != 0) {
                return lcm(qb_s, mp_s, ng_s, qt_s);
            }
            if (signals[s].target == rx && !signals[s].state) {
                return b;
            }
            Module *mod = &modules[signals[s].target];
            if (mod->type == BROADCAST) {
                for (int i = 0; i < mod->out_size; ++i) {
                    add_signal(signals[s].state, mod->out[i], signals[s].target);
                }
            } else if (mod->type == FLIP_FLOP) {
                if (!signals[s].state) {
                    mod->state[0] = !mod->state[0];
                    for (int i = 0; i < mod->out_size; ++i) {
                        add_signal(mod->state[0], mod->out[i], signals[s].target);
                    }   
                }
            } else {
                for (int i = 0; i < mod->in_size; ++i) {
                    if (mod->in[i] == signals[s].source) {
                        mod->state[i] = signals[s].state;
                        break;
                    }
                }

                bool all_high = true;
                for (int i = 0; i < mod->in_size; ++i) {
                    if (!mod->state[i]) {
                        all_high = false;
                        break;
                    }
                }
                for (int i = 0; i < mod->out_size; ++i) {
                    add_signal(!all_high, mod->out[i], signals[s].target);
                } 
            }
            s++;
        }
    }
    return -1;
}

std::string get_plantuml() {
    std::string define = "";
    std::string connections = "";

    for (int i = 0; i < size; ++i) {
        define += "usecase " + std::to_string(i) + " as \"" + modules[i].label + "\" " + (modules[i].type == BROADCAST ? "#FF0000" : (modules[i].type == FLIP_FLOP ? "#ebb734" : "#34eb71")) +"\n";
            for (int j = 0; j < modules[i].out_size; ++j) {
                connections += std::to_string(i) + " --> " + std::to_string(modules[i].out[j]) + "\n";
            } 
    }
    return "@startuml\n" + define + "\n"  + connections + "@enduml";
}

void print_modules() {
    for (int i = 0; i < size; ++i) {  
        std::cout << "Module(\n\tlabel: "<<modules[i].label<<", \n\ttype: "<<(
            modules[i].type == BROADCAST ? "broadcast" : (modules[i].type == FLIP_FLOP ? "flip_flop" : "conjunction"))<<", \n\tstate: "<<modules[i].state<<", \n\toutputs: ";
            for (int j = 0; j < modules[i].out_size; ++j) {
                std::cout << modules[i].out[j] << ", ";
            } 
            std::cout << ", \n\tins: ";
            for (int j = 0; j < modules[i].in_size; ++j) {
                std::cout << modules[i].in[j] << ", ";
            } 
            std::cout << "\n)\n"<<std::endl;

    }
}

void reset_modules() {
    for (int i = 0; i < size; ++i) {
        if (modules[i].type == FLIP_FLOP) {
            modules[i].state[0] = false;
        } else if (modules[i].type == CONJUNCTION) {
            for (int j = 0; j < modules[i].in_size; ++j) {
                modules[i].state[j] = false;
            }
        }
    }
}

int main() {
    std::ifstream file (FILE_PATH);
    std::string line;
    while (getline(file, line)) {
        Type type;
        if (line[0] == '%') {
            type = FLIP_FLOP;
            line = line.substr(1);
        } else if (line[0] == '&') {
            type = CONJUNCTION;
            line = line.substr(1);
        } else {
            type = BROADCAST;
        }
        int j = 0;
        while (line[j] != ' ') {
            j++;
        }
        int index = get_module_index(line.substr(0, j));
        modules[index].type = type;
        line = line.substr(j + 4);

        while (line.length() != 0) {
            j = 0;
            while (j < line.length() && line[j] != ',') {
                j++;
            }   
            add_output_to_module(index, line.substr(0, j));
            line = line.substr(std::min(j + 2, (int)line.length()));
        }
    }
    file.close();
    //print_modules();
    //std::cout << get_plantuml() << std::endl;
    std::cout << "Solution 1: " << solution1() << std::endl;
    reset_modules();
    std::cout << "Solution 2: " << solution2() << std::endl;
    return 0;
}
