#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include "LinkedList.h"

using namespace std;

class BigInteger {

public:
    BigInteger(int number = 0);

    BigInteger(const string &bigInteger);

    BigInteger operator+(const BigInteger &list);

    BigInteger operator*(const BigInteger &list);

    BigInteger operator*(int i);

    BigInteger(const BigInteger &other);

    BigInteger &operator=(const BigInteger &list);

    ~BigInteger();

public:
    LinkedList *num = 0;
};

#endif