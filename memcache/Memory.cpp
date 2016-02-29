//
//  Memory.cpp
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#include "Memory.h"

Memory::Memory(int size, Memcache* nextLevel)
{
    this->countdown = 100;
    this->size = size;
    this->data = new int[size];
    this->nextLevel = nullptr;
}

message Memory:: load(int address)
{
    int size = this->size;
    int* data = this->data;
    message msg;
    if(address > size - 1 || address < 0)
    {
        //   cout << "The address requested is out of bound" << endl;
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

message Memory:: store(int address, int value)
{
    int size = this->size;
    int* data = this->data;
    message msg;
    if(address > size - 1 || address < 0)
    {
        // cout << "The address requested is out of bound" << endl;
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



int* Memory::getData()
{
    if(this->data == nullptr)
    {
        return nullptr;
    }
    return this->data;

}

int Memory::getSize()
{
    return this->size;
}
