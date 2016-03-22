#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int encoding(string opcode, vector<string> para) {
  cout<<opcode<<" ";
  for(int i=0; i<para.size(); ++i){
    cout<<para[i]<<" ";
  }
  cout<<endl;
  return 0;
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
