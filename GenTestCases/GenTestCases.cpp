//
//  GenTestCases.cpp
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/29/16.
//  Copyright (c) 2016 Wei Hong. All rights reserved.
//

#include<iostream>
#include<fstream>
#include<random>
using namespace std;

int genRandomNumber(int n) {
    std:random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0, n-1);
    return distribution(generator);
}

int main() {
    int lenOfTestCases = 1000;
    // generate sequential load test cases;
    ofstream testcase;
    testcase.open("sequential_load.txt");
    for(int i = 0; i < lenOfTestCases; i++){
        testcase<< "L" << ","<< i << "\n";
    }
    testcase.close();

    // generate sequential store test cases;
    testcase.open("sequential_store.txt");
    for(int i = 0; i < lenOfTestCases; i++){
	testcase << "S" << "," << i << "," << 123 << "\n";
    }
    testcase.close();

    // generate mixed sequential test cases;
    testcase.open("mixed_sequential_load_store.txt");
    for(int i = 0; i < lenOfTestCases; i++) {
    	if(genRandomNumber(2)==0) {
		testcase << "L" << "," << i << "\n";
	} else {
		testcase << "S" << "," << i << "," << 123 << "\n";

	}
    }
    testcase.close();
    // generate random load test cases;
    testcase.open("random_load.txt");
    for(int i = 0; i < lenOfTestCases; i++) {
	testcase << "L" <<","<< genRandomNumber(lenOfTestCases)<< "\n";
    }    
    testcase.close();
    // generate random store test cases;
    testcase.open("random_store.txt");
    for(int i = 0; i < lenOfTestCases; i++) {
	testcase << "S"<<"," << genRandomNumber(lenOfTestCases)<<"," << 123 << "\n";

    }
    testcase.close();
    
     // generate mixed random test cases;
    testcase.open("mixed_random_load_store.txt");
    for(int i = 0; i < lenOfTestCases; i++) {
        if(genRandomNumber(2)==0) {
                testcase << "L" << "," << genRandomNumber(lenOfTestCases)<< "\n";
        } else {
                testcase << "S" << "," << genRandomNumber(lenOfTestCases) << "," << 123 << "\n";

        }
    }
    testcase.close();
    return 0;
}
