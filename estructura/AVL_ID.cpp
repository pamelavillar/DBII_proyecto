#include <iostream>
#include <string>
using namespace std;
class CNode{
    public:
        size_t id;
        int height;
        CNode* node[2];
        vector<string> registro;
        long int byte_inicial;
        long int byte_fin;
        vector<pair<long int, long int>> particion_dirs;
        CNode(size_t _id, vector<string> reg, long int dir, long int dir2, vector<pair<long int, long int>> p_dir){
            node[0] = nullptr;
            node[1] = nullptr;
            registro = reg;
            byte_inicial = dir;
            byte_fin = dir2;
            particion_dirs = p_dir;
            id = stoi(reg[0]);
        }

};

class AVLTree {
public:
    CNode* root;
    vector<CNode**> path;
    AVLTree() {
        root = nullptr;
    }
    bool insert(size_t _id, vector<string> reg, long int dir, long int dir2, vector<pair<long int, long int>> p_dir);
    bool evaluando(CNode** current);
    CNode** rep(CNode** p);
    void inorder(CNode* p);
    CNode* Root();

    bool find(CNode**& p, int v);
    int height(CNode* p);
    int factor_balance(CNode* p);
    void rotaRight(CNode** p);
    void rotaLeft(CNode** p);
};
CNode* AVLTree::Root() {
    return root;
}
void AVLTree::inorder(CNode* n) {
    if (n == nullptr) return;

    inorder(n->node[0]);//paso 1: elemento del lado izquierdo 
    cout << n->id << " "; //paso 2: elemento del root
    inorder(n->node[1]); //paso 3: elemento del lado derecho
}

bool AVLTree::find(CNode**& p, int v) {
    p = &root;
    path.clear();
    while (*p && (*p)->id != v) {
        path.push_back(p);
        p = &((*p)->node[v > (*p)->id]);
    }
    return *p != nullptr;
}


void AVLTree::rotaRight(CNode** p) {
    CNode* C = (*p);
    CNode* B = C->node[0];
    CNode* A = B->node[0];

    CNode* br = B->node[1] ? B->node[1] : nullptr;

    *p = B;
    C->node[0] = br;
    B->node[1] = C;

    height(C);
    height(B);
}

void AVLTree::rotaLeft(CNode** p) {
    CNode* A = (*p);
    CNode* B = A->node[1];
    CNode* C = B->node[1];

    CNode* bl = B->node[0] ? B->node[0] : nullptr;

    *p = B;
    A->node[1] = bl;
    B->node[0] = A;

    height(A); 
    height(B);
}

bool AVLTree::evaluando(CNode** current){
    
    int balance = factor_balance(*current);

    if (balance == -2) {
        int hijo_izquierdo = factor_balance(((*current)->node[0]));
        if (hijo_izquierdo <= 0) {
            // rotacion simple a la derecha 
            rotaRight(current);
        }
        else {
            // rotacion doble izquierda - derecha
            // rotacion izquierda
            rotaLeft(&((*current)->node[0]));
            //rota derecha
            rotaRight(current);
        }
        return true;
    }
    if (balance == 2) {
        int hijo_derecho = factor_balance((*current)->node[1]);
        if (hijo_derecho >= 0) {
            rotaLeft(current); 
        }
        else {
            rotaRight(&((*current)->node[1])); 
            rotaLeft(current); 
        }
        return true;
    }
    return false;
}
bool AVLTree::insert(size_t _id, vector<string> reg, long int dir, long int dir2, vector<pair<long int, long int>> p_dir) {
    path.clear();
    CNode** pos;
    if (find(pos, _id)) return false;
    *pos = new CNode(_id, reg, dir, dir2, p_dir);

    for (int i = path.size() - 1; i >= 0; i--) {
        CNode** current = path[i];
        height(*current);
        if(evaluando(current)) break;
    }
    return true;
}

int  AVLTree::height(CNode* p) {
    if (!p) return 0;
    int l, r;
    l = height(p->node[0]);
    r = height(p->node[1]);

    p->height = 1 + (l > r ? l : r);
    return  p->height;
}

int AVLTree::factor_balance(CNode* p) {
    if (!p) return false;

    int leftHeight = p->node[0] ? p->node[0]->height : 0;
    int rightHeight = p->node[1] ? p->node[1]->height : 0;
    int factor = rightHeight - leftHeight;

    return factor;
}

