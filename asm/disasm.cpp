//
// Created by wei on 4/14/16.
//
// Decode a 32-byte integer instruction into an assembly clause

// highest 7 bits -> opcode

// Type 1, Type 2, Type 3
#include<string>
#include<map>
#include<iostream>
#include<fstream>
using namespace std;

// struct{
//     string opcode;
//     int type;
// } decoding;

map<uint32_t, string> opcode_decode_map = {
    //data transfer
    {0b0010000,      "lb"},
    {0b0010001,     "lbu"},
    {0b0011000,      "sb"},
    {0b0010010,      "lw"},
    {0b0011001,      "sw"},
    {0b0010100,     "lsp"},
    //arithmetic
    {0b1000000,     "add"},
    {0b1000001,     "sub"},
    {0b1010011,    "addi"},
    {0b1010100,    "subi"},
    {0b1000010,     "mul"},
    {0b1000011,     "muh"},
    {0b1000100,    "mulu"},
    {0b1000101,    "muhu"},
    {0b1000110,     "div"},
    {0b1000111,    "divu"},
    {0b1001000,    "modu"},
    {0b1001001,     "and"},
    {0b1001010,      "or"},
    {0b1001011,     "not"},
    {0b1001100,     "xor"},
    {0b1001101,      "rr"},
    {0b1001110,     "srl"},
    {0b1001111,     "sra"},
    {0b1010000,      "sl"},
    {0b1010001,     "slt"},
    {0b1010010,    "sltu"},
    {0b1010101,    "slti"},
    {0b1010110,   "sltiu"},
    //control
    {0b0000001,       "j"},
    {0b0000010,     "jal"},
    {0b0000011,     "beq"},
    {0b0000100,    "bneq"},
    {0b0000101,    "bgez"},
    {0b0000110,    "bgtz"},
    {0b0000111,    "blez"},
    {0b0001000,    "bltz"},
    {0,           "break"},
    //floating point
    {0b0100000,   "addsp"},
    {0b0100001,   "subsp"},
    {0b0100010,   "mulsp"},
    {0b0100011,   "divsp"},
    {0b0100100,   "sltsp"},
    {0b0100101,    "witf"},
    {0b0100110,    "wfti"},
    //cache
    {0b0110000,    "pref"},
    //simd
    {0b1101010,    "move"},
    {0b1101011,   "copys"},
    {0b1101100,   "copyu"},
    {0b1101101, "insertb"},
    {0b1101110,   "fillb"},
    {0b1100000,   "vaddb"},
    {0b1100001,   "vsubb"},
    {0b1100010,   "vmulb"},
    {0b1100011,   "vdivb"},
    {0b1100100,   "vmodb"},
    {0b1100101,    "ceqb"},
    {0b1100110,    "cleb"},
    {0b1100111,   "cleub"},
    {0b1101000,    "cltb"},
    {0b1101001,   "cltub"}
};

map<string, uint32_t> type_map = {
    //data transfer
    {"lb",      2},
    {"lbu",     2},
    {"sb",      2},
    {"lw",      2},
    {"sw",      2},
    {"lsp",     2},
    //arithmetic
    {"add",     3},
    {"sub",     3},
    {"addi",    2},
    {"subi",    2},
    {"mul",     3},
    {"muh",     3},
    {"mulu",    3},
    {"muhu",    3},
    {"div",     3},
    {"divu",    3},
    {"modu",    3},
    {"and",     3},
    {"or",      3},
    {"not",     3},
    {"xor",     3},
    {"rr",      3},
    {"srl",     3},
    {"sra",     3},
    {"sl",      3},
    {"slt",     3},
    {"sltu",    3},

    {"slti",    2},
    {"sltiu",   2},
    //control
    {"j",       1},
    {"jal",     1},
    {"beq",     2},
    {"bneq",    2},
    {"bgez",    2},
    {"bgtz",    2},
    {"blez",    2},
    {"bltz",    2},
    {"break",   1}, //?
    //floating point
    {"addsp",   3},
    {"subsp",   3},
    {"mulsp",   3},
    {"divsp",   3},
    {"sltsp",   3},
    {"witf",    3},
    {"wfti",    3},
    //cache
    {"pref",    2}, //?
    //simd
    {"move",    2},
    {"copys",   2},
    {"copyu",   2},
    {"insertb", 2},
    {"fillb",   2},
    {"vaddb",   3},
    {"vsubb",   3},
    {"vmulb",   3},
    {"vdivb",   3},
    {"vmodb",   3},
    {"ceqb",    3},
    {"cleb",    3},
    {"cleub",   3},
    {"cltb",    3},
    {"cltub",   3}
};



string decode(uint32_t code) {
    // get highest 7 bits -> opcode
    int opcode = (code & 0xfe000000) >> 25;
    // lookup from the opcode map
    string op = "";
    if(opcode == 0) return "break";

    if(opcode_decode_map.find(opcode) == opcode_decode_map.end()) {
        cout << "error opcode" << endl;
        return "";
    } else {
        op = opcode_decode_map.find(opcode)->second;
    }
    int inst_type = 0;
    if(type_map.find(op) == type_map.end()) {
        cout << "instruction type not supported" << endl;
        return "";
    } else {
        inst_type = type_map.find(op)->second;
    }


    if(inst_type == 1) {
        int offset = 0x01ffffff & code;
        return op + " " + to_string(offset);
    } else if(inst_type == 2) {
        int reg1 =   (code & 0x01e00000) >> 21;
        int reg2 =   (code & 0x001e0000) >> 17;
        int offset = (code & 0x0001ffff);
        return op + " " + "$" + to_string(reg1) + "," + "$" + to_string(reg2) + "," + to_string(offset);
    } else if(inst_type == 3) {
        int reg1 =  (code & 0x01e00000) >> 21;
        int reg2 =  (code & 0x001e0000) >> 17;
        int reg3 =  (code & 0x0001e000) >> 13;
        return op + " " + "$" + to_string(reg1) + "," + "$" + to_string(reg2) + "," + "$" + to_string(reg3);
    } else {
        cout << "error instruction type" << endl;
        return "";
    }
}


int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: disam FILENAME" << endl;
    }
    string filename(argv[1]);
    std::ifstream file(filename);
    std::string line;
    while(std::getline(file, line)) {
        if(line.size() == 0) {
            break;
        }
        cout<<line<<" "<<line.size()<<" ";
        cout << decode(stol(line)) <<endl;
    }
    file.close();
}
