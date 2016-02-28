#include<iostream>
#include<stdio.h>
using namespace std;

typedef struct {
	int value;
	bool ok;
}message;


class Memcache {



};


class Cache 
{

};


class Memory: public Memcache
{
  public:
	int countdown;
	Memcache *nextLevel;	
	Memory(int size, Memcache *nextLevel);
	void setSize();
	void setData(int size);
	int* getData();
	int getSize();
	message load(int address);
	message store(int address, int value);
 private:
	int size;
	int* data;
  	
};

Memory::Memory(int size, Memcache* nextLevel)
{
	this->countdown = 100;
	this->size = size;
	this->data = new int[size];
	this->nextLevel = NULL;
}

int* Memory::getData()
{
	if(this->data == NULL)
	{
		cout << "No memory is allocated!" << endl;
		return NULL;
	}
	return this->data;

}

int Memory::getSize()
{
	return this->size;
}

message Memory::load(int address)
{
	int size = this->size;
	int* data = this->data;
	message msg;
	if(address > size - 1 || address < 0) 
	{
		cout << "The address requested is out of bound" << endl;
		return msg;
	} else {
		if(countdown == 0) 
		{ 
			msg.value = data[address];
			msg.ok = true;
			countdown = 100;
			return msg;
		} else {
			countdown--;
			msg.ok = false;
			return msg;
		}
	}
}

message Memory::store(int address, int value)
{
	int size = this->size;
	int* data = this->data;
	message msg;
	if(address > size - 1 || address < 0)
	{
		cout << "The address requested is out of bound" << endl;
		return msg;
	} else {
		if(countdown == 0) 
		{
			data[address] = value;
			msg.ok = true;
			countdown = 100;
			return msg;
		} else {
			countdown--;
			msg.ok = false;
			return msg;
		}
	}
}



int main() 
{
 Memory memory(100,NULL);
 cout << memory.countdown <<endl;
 int* data = memory.getData();
 cout << data[10] <<endl;
 message write_msg = memory.store(10,2222);
 while(!write_msg.ok) {
	write_msg = memory.store(10,2222);
 } 
 message msg = memory.load(10); 
 while(!msg.ok) {
	msg = memory.load(10);
 }
 cout << msg.value << endl;
 return 0;
}
