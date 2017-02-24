#include "BigInteger.h"

BigInteger::BigInteger(int number) {
    num = new LinkedList;
    while (number > 0) {
        (*num).pushTail(number % 10);
        number = number / 10;
    }
}

BigInteger::BigInteger(const string &bigInteger) {
    num = new LinkedList;
    for (int i = 0; i < bigInteger.length(); i++) {
        (*num).pushHead(int(bigInteger[i]) - '0');
    }
}

BigInteger carry(const BigInteger &crowded) {
    BigInteger normal;
    Node *head = crowded.num->head;
    int carrier = 0;
    while (head) {
        int n = head->data + carrier;
        normal.num->pushTail(n % 10);
        carrier = n / 10;
        head = head->next;
    }
    if (carrier > 0) {
        normal.num->pushTail(carrier);
    }
    return normal;
}

BigInteger BigInteger::operator+(const BigInteger &list) {
    BigInteger sum;
    *sum.num = *num + *list.num;
    return carry(sum);
}

BigInteger BigInteger::operator*(const BigInteger &list) {
    BigInteger result;
    int digit = 0;
    Node *headThis = num->head;
    while (headThis) {
        Node *headList = list.num->head;
        BigInteger draft1;
        while (headList) {
            draft1.num->pushTail(headList->data * headThis->data);
            headList = headList->next;
        }
        BigInteger draft2 = carry(draft1);
        for (int i = 0; i < digit; i++) {
            draft2.num->pushHead(0);
        }
        result = result + draft2;
        headThis = headThis->next;
        digit++;
    }
    return result;
}

BigInteger BigInteger::operator*(int i) {
    return BigInteger::operator*(BigInteger(i));
}

BigInteger::BigInteger(const BigInteger &other) {
    num = new LinkedList(*other.num);
}

BigInteger &BigInteger::operator=(const BigInteger &list) {
    delete num;
    num = new LinkedList(*list.num);
    return *this;
}

BigInteger::~BigInteger() {
    delete num;
}