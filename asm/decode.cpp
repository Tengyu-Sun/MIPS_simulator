#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
   std::string filename(argv[1]);
   std::fstream input(filename, fstream::in | fstream::binary);
   uint32_t ins = 0;
   uint8_t buf[4];
   input.read((char*)buf, 4);
   int flag = 0;
   ins = (buf[0]<<24) | ((uint32_t)buf[1]<<16) | ((uint32_t)buf[2]<<8) | ((uint32_t)buf[3]);
   std::cout<<ins<<std::endl;
   while(input){
      //std::cout<<ins<<std::endl;
      input.read((char*)buf, 4);
      ins = (buf[0]<<24) | ((uint32_t)buf[1]<<16) | ((uint32_t)buf[2]<<8) | ((uint32_t)buf[3]);
      cout<<ins<<" "<<(uint32_t)buf[0]<<" "<<(uint32_t)buf[1]<<" "<<(uint32_t)buf[2]<<" "<<(uint32_t)buf[3]<<endl;
   }
   input.close();
   return 0;
}
