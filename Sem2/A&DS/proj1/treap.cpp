#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <ctime>
using namespace std;

struct Node{
    int key;
    long long cena;
    int size;
    long long sum;
    double priority;
    Node *left,*right;
    Node (int key, double priority, int cena) : key(key), cena(cena), size(1), sum(cena), priority(priority), left(nullptr), right(nullptr) {}
};

typedef Node* Treap;

int getSize(Treap t) {return t ? t-> size : 0;}
long long getSum(Treap t) {return t ? t->sum : 0;}
void pull(Treap t){
    if (t) t->size = 1 + getSize(t->left) + getSize(t->right);
    if (t) t->sum = t->cena + getSum(t->left) + getSum(t->right);
}

void split(Treap t, int key, Treap &l, Treap &r){
    if (!t) l = r = nullptr;
    else if (t->key <= key){
        split(t->right, key, t->right, r), l = t;
        pull(t);
    } else  {
        split(t->left, key, l, t->left), r = t;
        pull(t);
    }
}

void insert(Treap &t, Treap it){
    if (!t) t = it;
    else if (it->priority > t->priority){
        split(t, it->key, it->left, it->right), t = it;
        pull(t);
    } else {
        insert(it->key <= t->key ? t->left : t->right, it);
        pull(t);
    }
}

void merge(Treap &t, Treap l, Treap r){
    if(!l || !r) t = l ? l : r;
    else if (l->priority > r->priority){ 
        merge(l->right, l->right, r), t = l;
        pull(t);
    }
    else{ 
        merge(r->left, l, r->left), t = r;
        pull(t);
    }
}

void erase(Treap &t, int key){
    if(!t) { return; }
    if (t-> key == key){
        Treap th = t;
        merge(t, t->left, t->right);
        delete th;
        pull(t);
    }else {
        erase(key < t->key ? t->left : t->right, key);
        pull(t);
    }
}

void returnValue(Treap t, int min, int max, long long &wynik){
    if (!t) return;
    //int half = (min + max) / 2;
    if(min>max){ wynik = 0; return;}
    Treap left, right;
    split(t, max, left, right);
    Treap left2, middle;
    split(left, min - 1, left2, middle);
    wynik = getSum(middle);
    Treap temp;
    merge(temp, left2, middle);
    merge(t, temp, right);

}

Treap treap = nullptr;

int main(){
    srand(time(0));
    int countOperations;
    cin >> countOperations;
    
    for (int i = 0; i < countOperations; i++){
        char operation;
        cin >> operation;

        if (operation == 'A') {
            int id, cena;
            cin >> id >> cena;
            erase(treap, id);
            double x = (rand() + 0.5) / (RAND_MAX + 1.0);
            insert(treap, new Node(id, x, cena));
        } else if (operation == 'D') {
            int id;
            cin >> id;
            erase(treap, id);
        } else if (operation == 'S') {
            int id_min, id_max;
            cin >> id_min >> id_max;
            long long wynik = 0;
            
            returnValue(treap, id_min, id_max, wynik);

            cout << wynik << '\n';
        } else if (operation == 'C') {
            cout << getSize(treap) << '\n';
        }
    }
    
    return 0;
}