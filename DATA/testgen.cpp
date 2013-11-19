/* Kompilowac z -std=c++11 */
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Node {
    protected:
        Node *left, *right;
    public:
        virtual int calculate() = 0;
        virtual string write() = 0;
        Node(Node* _left, Node* _right) : left(_left), right(_right) {}
        virtual ~Node() {
            delete left; delete right;
            left = right = NULL;
        }
};

class Plus : public Node {
    public:
        Plus(Node* _left, Node* _right) : Node(_left, _right) {}
        virtual int calculate() {
            return left->calculate() + right->calculate();
        }
        virtual string write() {
            return left->write() + " " + right->write() + " +";
        }
};

class Minus : public Node {
    public:
        Minus(Node* _left, Node* _right) : Node(_left, _right) {}
        virtual int calculate() {
            return left->calculate() - right->calculate();
        }
        virtual string write() {
            return left->write() + " " + right->write() + " -";
        }
};

class Times : public Node {
    public:
        Times(Node* _left, Node* _right) : Node(_left, _right) {}
        virtual int calculate() {
            return left->calculate() * right->calculate();
        }
        virtual string write() {
            return left->write() + " " + right->write() + " *";
        }
};

class Div : public Node {
    public:
        Div(Node* _left, Node* _right) : Node(_left, _right) {}
        virtual int calculate() {
            return left->calculate() / right->calculate();
        }
        virtual string write() {
            return left->write() + " " + right->write() + " /";
        }
};

class Const : public Node {
    private:
        int val;
    public:
        Const(int _val) : Node(NULL, NULL), val(_val) {}
        virtual int calculate() {
            return val;
        }
        virtual string write() {
            return to_string(val);
        }
};

Node* generate(int opers, int N) {
    Node *v, *l, *r;
    if (opers > 0) {
        opers--;
        int l_opers, r_opers;
        l_opers = rand() % (opers + 1);
        r_opers = opers - l_opers;
        l = generate(l_opers, N);
        r = generate(r_opers, N);
        switch (rand() % 4) {
            case 0:
                v = new Plus(l, r); break;
            case 1:
                v = new Minus(l, r); break;
            case 2:
                v = new Times(l, r); break;
            case 3:
                if (r->calculate() == 0) {
                    Node *l1, *r1;
                    l1 = r;
                    r1 = new Const(1);
                    r = new Plus(l1, r1);
                }
                v = new Div(l, r);
                break;
        }
    }
    else {
        int val = rand() % N;
        if (rand() % 2)
            val = -val;
        v = new Const(val);
    }
    return v;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    cout << "Podaj kolejno: liczbe linii, liczbe operatorow(+,-, etc.), "
         << "nazwe wyniku (bez rozszerzenia), ograniczenie wartosci bezwgl."
         << " pojedynczej liczby w wyrazeniu" << endl;
    int l_lines, l_opers, N;
    string nazwa;
    cin >> l_lines >> l_opers >> nazwa >> N;
    fstream in(nazwa + ".in", ios_base::out), out(nazwa + ".out", ios_base::out);
    in << l_lines << endl;
    for (int i = 1; i <= l_lines; i++) {
        int temp = l_opers;
        Node * root = generate(temp, N);
        in << root->write() << endl;
        out << i << ": " << root->calculate() << endl;
        delete root; root = NULL;
    }
    in.close();
    out.close();
}
