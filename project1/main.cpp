#include <fstream>
#include <algorithm>
#include "BigInteger.h"

using namespace std;

ostream &operator<<(ostream &out, const Node &node) {
    Node n2 = node, *p = &n2;
    out << p->data << " ";
    while (p->next) {
        p = p->next;
        out << p->data << " ";
    }
    return out;
}

ostream &operator<<(ostream &out, const BigInteger &bigInteger) {
    string str = "";
    Node *head = bigInteger.num->head;
    while (head) {
        str += to_string(head->data);
        head = head->next;
    }
    reverse(str.begin(), str.end());
    if (str == "")
        str = "0";
    out << str;
    return out;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }
    ifstream ifs(argv[1]);
    vector<string> v;
    while (!ifs.eof()) {
        string s;
        getline(ifs, s);
        v.push_back(s);
    }
    ifs.close();
    BigInteger int1(v[1]), int2(v[2]), result;
    if (v[0] == "+") {
        result = int1 + int2;
    } else {
        result = int1 * int2;
    }
    ofstream ofs(argv[2]);
    ofs << result;
    ofs.close();
    return 0;
}