//
//  main.cpp
//  matrix_multiplication
//
//  Created by Wei Hong on 3/26/16.
//  Copyright (c) 2016 Wei Hong. All rights reserved.
//

#include <iostream>
using namespace std;

int main(int argc, const char * argv[]) {
    // left matrix of dimension(a*b) * right matrix of dimension (b*c) = res[a][c]
    int a = 2;
    int b = 2;
    int c = 2;
    int lM[2][2] = {{1,2},{3,4}};
    int rM[2][2] = {{5,6},{7,8}};
    int res[2][2]= {{0,0},{0,0}};
    for(int i = 0; i < a; i++) {
        for(int j = 0; j < c; j++) {
            for(int k = 0; k < b; k++) {
                res[i][j] += lM[i][k]*rM[k][j];
            }
        }
    }

    for(int i = 0; i < a; i++) {
        for(int j = 0; j < c; j++) {
                cout << res[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
