#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define TEMP_FILE "temp.txt"
using namespace std;

map<string, uint32_t> opcode_map = {
    //data transfer
    {"lb",      0b0010000},
    {"lbu",     0b0010001},
    {"sb",      0b0011000},
    {"lw",      0b0010010},
    {"sw",      0b0011001},
    {"lsp",     0b0010100},
    //arithmetic
    {"add",     0b1000000},
    {"sub",     0b1000001},
    {"addi",    0b1010011},
    {"subi",    0b1010100},
    {"mul",     0b1000010},
    {"muh",     0b1000011},
    {"mulu",    0b1000100},
    {"muhu",    0b1000101},
    {"div",     0b1000110},
    {"divu",    0b1000111},
    {"modu",    0b1001000},
    {"add",     0b1001001},
    {"or",      0b1001010},
    {"not",     0b1001011},
    {"xor",     0b1001100},
    {"rr",      0b1001101},
    {"srl",     0b1001110},
    {"sra",     0b1001111},
    {"sl",      0b1010000},
    {"slt",     0b1010001},
    {"sltu",    0b1010010},
    {"slti",    0b1010101},
    {"sltiu",   0b1010110},
    //control
    {"j",       0b0000001},
    {"jal",     0b0000010},
    {"beq",     0b0000011},
    {"bneq",    0b0000100},
    {"bgez",    0b0000101},
    {"bgtz",    0b0000110},
    {"blez",    0b0000111},
    {"bltz",    0b0001000},
    {"break",   0},
    //floating point
    {"addsp",   0b0100000},
    {"subsp",   0b0100001},
    {"mulsp",   0b0100010},
    {"divsp",   0b0100011},
    {"sltsp",   0b0100100},
    {"witf",    0b0100101},
    {"wfti",    0b0100110},
    //cache
    {"pref",    0b0110000},
    //simd
    {"move",    0b1101010},
    {"copys",   0b1101011},
    {"copyu",   0b1101100},
    {"insertb", 0b1101101},
    {"fillb",   0b1101110},
    {"vaddb",   0b1100000},
    {"vsubb",   0b1100001},
    {"vmulb",   0b1100010},
    {"vdivv",   0b1100011},
    {"vmodb",   0b1100100},
    {"ceqb",    0b1100101},
    {"cleb",    0b1100110},
    {"cleub",   0b1100111},
    {"cltb",    0b1101000},
    {"cltub",   0b1101001}
};

// store label in a symbol map. non label clauses start with two whitespaces
void first_pass(fstream& input, map<string, int>& symbol_map) {
  int line_num = -1;
  string line;
  while(input) {
    std::getline(input, line);
    line_num += 1;
    if (line.size() == 0) {
      break;
    }
    if (isspace(line[line.size()-1])) {
      line = line.substr(0, line.size()-1);
    }
    string opcode;
    vector<string> para;
    int i = 0;
    int s = 0;
    while(i<line.size() && ' ' == line[i]) {
      ++i;
    }
    if(i == 0) {
      s = i;
      while(i<line.size() && line[i] != ' ') {
        ++i;
      }
      if(line.substr(s,1) == "#") {
	--line_num;
	continue;// it's a comment
      }
      string label  = line.substr(s, i - s - 1);
      symbol_map[label] = line_num;
      cout << "Label: " << label << " at line " << symbol_map[label] << endl;
      --line_num;
    }
  }
}


uint32_t encoding(string opcode, vector<string> para, map<string,int>& symbol_map, int line_num) {
  cout<<opcode<<" ";
  for(int i=0; i<para.size(); ++i) {
    cout<<para[i]<<" ";
  }
  cout<<endl;
  uint32_t code = 0;
  auto entry = opcode_map.find(opcode);
  if(entry == opcode_map.end()) {
    cout<<"error opcode: "<<opcode<<endl;
    return 0;
  } else {
    code = entry->second;
//    cout << entry->first << " " << code << endl;
    if(code == 0) return 0;
  }

  if (para.size() == 1) {
    // the variable exists in the symbol map, it's a jump
    int offset = 0;
    if(symbol_map.find(para[0]) != symbol_map.end()) {
	offset = symbol_map[para[0]] - line_num - 1;
    } else {
	offset = stoi(para[0]);
    }
    std::cout<<opcode<<" "<<para[0]<<" "<<code<<" "<<offset<<endl;
    code <<= 25;
    code |= (offset & 0x1ffffff);

  } else if (para.size() == 2) {
    code <<= 4;
    code |= stoi(para[0].substr(1));
    if (para[1][0] == '$') {
      code <<= 4;
      code |= stoi(para[1].substr(1));
      code <<= 17;
    } else {
      int offset = 0;
      if(symbol_map.find(para[1]) != symbol_map.end()) {
	 offset = symbol_map[para[1]] - line_num - 1;
       } else {
      	  offset = stoi(para[1]);
       }
      code <<=21;
      code |= (offset & 0x1ffff);
    }
  } else if (para.size() == 3) {
    code <<= 4;
    code |= stoi(para[0].substr(1));
    code <<= 4;
    code |= stoi(para[1].substr(1));
    if (para[2][0] == '$') {
      code <<= 4;
      code |= stoi(para[2].substr(1));
      code <<=13;
    } else {
      int imm;
      if(symbol_map.find(para[2]) != symbol_map.end()) {
	        imm = symbol_map[para[2]] - line_num - 1;
       } else {
      	  imm = stoi(para[2]);
       }
      code <<= 17;
      code |= imm & 0x1ffff;
    }

  } else {
    cout<<"error para: ";
    for (int i=0; i<para.size(); ++i) {
      cout<<para[i]<<" ";
    }
    cout<<endl;
    return 0;
  }

  return code;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout<<"Usage: asm FILENAME"<<std::endl;
    return 0;
  }
  std::string filename(argv[1]);
  std::fstream input(filename, std::fstream::in);
  std::fstream output(filename.substr(0,filename.size()-4)+".out", std::fstream::out);
  std::string line;
  map<string, int> symbol_map;
  first_pass(input, symbol_map);
  input.clear();
  input.seekg(0, ios::beg);
  int line_num = -1;
  while(input) {
    std::getline(input, line);
    if (line.size() == 0) {
      break;
    }
    if (isspace(line[line.size()-1])) {
      line = line.substr(0, line.size()-1);
    }
    string opcode;
    vector<string> para;
    int i = 0;
    int s = 0;
    while(i<line.size() && ' ' == line[i]) {
      ++i;
    }
    // this is a label or empty line
    if (i == 0 || i == line.size()) {
       continue;
    }
    ++line_num;
    s = i;
    while(i<line.size() && line[i] != ' ') {
      ++i;
    }
    opcode = line.substr(s, i-s);
    while(' ' == line[i]) {
      ++i;
    }
    s = i;
    for (int k=i; k<line.size(); ++k) {
      if (line[k] == ',') {
        para.push_back(line.substr(s, k-s));
        s = k + 1;
      }
    }
    if (s < line.size()){
      para.push_back(line.substr(s, line.size()-s));
    }
    uint32_t code = encoding(opcode, para, symbol_map, line_num);
    output<<code<<endl;
    std::cout<<line<<" "<<code<<endl;
  }
  return 0;
}
