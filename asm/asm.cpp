#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

uint32_t encoding(string opcode, vector<string> para) {
  uint32_t code = 0;
  if (opcode == "lb") {
    code = 0b0010000;
  } else if (opcode == "add") {
    code = 0b1000000;
  } else if (opcode == "sub") {
    code = 0b1000001;
  } else if (opcode == "bgez") {
    code = 0b0000101;
  } else if (opcode == "sb") {
    code = 0b0011000;
  } else if (opcode == "break") {
    return 0;
  } else {
    cout<<"error opcode: "<<opcode<<endl;
    return 0;
  }

  if (para.size() == 1) {
    code <<= 25;
    code |= stoi(para[0]);
  } else if (para.size() == 2) {
    code <<= 4;
    code |= stoi(para[0].substr(1));
    if (para[1][0] == '$') {
      code <<= 4;
      code |= stoi(para[1].substr(1));
      code <<= 17;
    } else {
      code <<=21;
      code |= stoi(para[1]);
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
      code <<= 17;
      code |= stoi(para[2]);
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
  while(input) {
    std::getline(input, line);
    if (line.size() == 0) {
      break;
    }
    string opcode;
    vector<string> para;
    int i = 0;
    int s = 0;
    while(' ' == line[i]) {
      ++i;
    }
    s = i;
    while(line[i] != ' ') {
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
        s = k+1;
      }
    }
    if (s < line.size()){
      para.push_back(line.substr(s, line.size()-s));
    }
    output<<encoding(opcode, para)<<endl;
  }
  return 0;
}
