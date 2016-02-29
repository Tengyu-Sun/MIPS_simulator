//
//  Memory.cpp
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#include "Memory.h"
#include <iostream>
using namespace std;

Memory::Memory(int size, int blocksize, Memcache* nextLevel)
{
    this->countdown = 100;
    this->size = size;
    this->blocksize = blocksize;
    this->data = new int[size];
    this->nextLevel = NULL;
}
//load a block from specific address
message Memory:: load(int address)
{
    int size = this->size;
    int blockNumber = address / blocksize;
    int* data = this->data;
    message msg;
    if(address > size - 1 || address < 0)
    {
        //   cout << "The address requested is out of bound" << endl;
        msg.data = NULL;
        msg.ok = false;
        return msg;
    } else {
        if(countdown == 0)
        {
            msg.data = &data[blockNumber*blocksize];
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

// write a block of data into memory
message Memory:: store(int address, int *block)
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
                int blockNumber = address / blocksize;
                for(int i = 0; i < blocksize; i++) {
                    data[blockNumber*blocksize + i] = *block++;
                }
                msg.data = &data[address];
                msg.ok = true;
                countdown = 100;
                return msg;
            } else {
                countdown--;
                msg.ok = false;
                return msg;
            }
    }
    return msg;
}


// write integer to specific address
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
            msg.data = &data[address];
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
    if(this->data == NULL)
    {
        return NULL;
    }
    return this->data;
    
}

int Memory::getSize()
{
    return this->size;
}
