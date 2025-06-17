//
// Created by chenk on 25-6-15.
//

#include "RandomNum.h"
#include <iostream>
#include <random>
#include <bits/ostream.tcc>
using namespace std;

RandomNum::RandomNum() {
    cout << "禁止类生成实例" << endl;
}
int RandomNum::range(const int i, const int j) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(i, j);
    return dis(gen);
}