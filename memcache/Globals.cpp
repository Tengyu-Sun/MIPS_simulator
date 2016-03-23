//
//  Globals.cpp
//  MIPS_Simulator2
//
//  Created by Wei Hong on 2/29/16.
//  Copyright (c) 2016 Wei Hong. All rights reserved.
//

#include "Globals.h"
#include<random>
int genRandomNumber(int ways) {
    // srand(time(NULL));
    // int ran = rand()*ways;
    // return ran;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,ways-1);
    return distribution(generator);
}

long global_clock = 0;