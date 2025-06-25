#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits>
//#include "AVL_ID.cpp"
using namespace std;

enum TipoDato {
    TIPO_ENTERO,
    TIPO_FLOTANTE,
    TIPO_STRING
};

struct Campo_Codificado {
    uint8_t campo; //campoo -> nombre atributo codificado
    TipoDato tipo; //valor del atributo
    union {
        int valorEntero;
        float valorFloat;
        const char* valorString;
    };
    bool operator<(const Campo_Codificado& otra) const {
        if (campo != otra.campo) return campo < otra.campo;
        if (tipo != otra.tipo) return tipo < otra.tipo;

        switch (tipo) {
            case TIPO_ENTERO:
                return valorEntero < otra.valorEntero;
            case TIPO_FLOTANTE:
                return valorFloat < otra.valorFloat;
            case TIPO_STRING:
                return strcmp(valorString, otra.valorString) < 0;
            default:
                return false;
        }
    }
    bool operator>(const Campo_Codificado& otra) const {
        return !(*this < otra);
    }
    bool operator==(const Campo_Codificado& otra) const {
        if (campo != otra.campo || tipo != otra.tipo) return false;
        switch (tipo) {
            case TIPO_ENTERO:
                return valorEntero == otra.valorEntero;
            case TIPO_FLOTANTE:
                return valorFloat == otra.valorFloat;
            case TIPO_STRING:
                return strcmp(valorString, otra.valorString) == 0;
            default:
                return false;
        }
    }
};

struct NodoAVL {
    Campo_Codificado clave;
    vector<unsigned int> ids; 
    NodoAVL* izquierdo = nullptr;
    NodoAVL* derecho = nullptr;
    int altura = 1;

    NodoAVL(const Campo_Codificado& k, unsigned int id) : clave(k) {
        ids.push_back(id);
    }
};

class AVL_Busqueda {
private:
    NodoAVL* root;
    int altura(NodoAVL* nodo) {
        return nodo ? nodo->altura : 0; }

    int balance(NodoAVL* nodo) {
        return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0; }

    void actualizarAltura(NodoAVL* nodo) {
        nodo->altura = 1 + max(altura(nodo->izquierdo), altura(nodo->derecho)); }

    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    void inorder(NodoAVL* nodo);
    vector<unsigned int> busquedaInterna(NodoAVL* nodo, const Campo_Codificado& clave);
public:
    AVL_Busqueda() : root(nullptr) {}
    vector<string> campos, tipos;
    NodoAVL* insertar(NodoAVL* nodo, const Campo_Codificado& clave, unsigned int id);
    void insertar(const Campo_Codificado& clave, unsigned int id) {
        root = insertar(root, clave, id);
    }
    void inorder() {
        inorder(root);
    }
    void setCampos(const vector<string>& _campos) {
        campos = _campos;
    }
    void setTipos(const vector<string>& _tipos) {
        tipos = _tipos;
    }
    vector<unsigned int> buscar(const string& campoNombre, const string& valor);
    // Búsqueda por rango
    vector<unsigned int> buscarRango(const string& campo, const string& operador, const string& valor);
};
NodoAVL* AVL_Busqueda::rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierdo;
    NodoAVL* T2 = x->derecho;
    x->derecho = y;
    y->izquierdo = T2;
    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}
NodoAVL* AVL_Busqueda::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecho;
    NodoAVL* T2 = y->izquierdo;
    y->izquierdo = x;
    x->derecho = T2;
    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}
NodoAVL* AVL_Busqueda::insertar(NodoAVL* nodo, const Campo_Codificado& clave, unsigned int id) {
    if (!nodo) return new NodoAVL(clave, id);

    if (clave == nodo->clave) {
        nodo->ids.push_back(id);
        if(nodo->ids.size() > 1){
            if(nodo->ids[nodo->ids.size()-1] < nodo->ids[nodo->ids.size()-2]) sort(nodo->ids.begin(), nodo->ids.end());
        }
        return nodo;
    }
    if (clave < nodo->clave)
        nodo->izquierdo = insertar(nodo->izquierdo, clave, id);
    else
        nodo->derecho = insertar(nodo->derecho, clave, id);
    actualizarAltura(nodo);
    int bal = balance(nodo);
    if (bal > 1 && clave < nodo->izquierdo->clave)
        return rotarDerecha(nodo);

    if (bal < -1 && nodo->derecho && clave > nodo->derecho->clave)
        return rotarIzquierda(nodo);

    if (bal > 1 && nodo->izquierdo && clave > nodo->izquierdo->clave) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && nodo->derecho && clave < nodo->derecho->clave) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }
    return nodo;
}
vector<unsigned int> AVL_Busqueda::buscar(const string& campoNombre, const string& valor) {
    // Buscar el índice del campo
    int campoIndex = -1;
    for (int i = 0; i < campos.size(); ++i) {
        if (campos[i] == campoNombre) {
            campoIndex = i;
            break;
        }
    }
    if (campoIndex == -1 || campoIndex >= tipos.size()) return {};

    Campo_Codificado clave;
    clave.campo = campoIndex;

    string tipo = tipos[campoIndex];
    if (tipo == "int") {
        clave.tipo = TIPO_ENTERO;
        clave.valorEntero = stoi(valor);
    }
    else if (tipo == "float") {
        clave.tipo = TIPO_FLOTANTE;
        clave.valorFloat = stof(valor);
    }
    else {
        clave.tipo = TIPO_STRING;
        clave.valorString = valor.c_str();
    }

    return busquedaInterna(root, clave);
}

vector<unsigned int> AVL_Busqueda::busquedaInterna(NodoAVL* nodo, const Campo_Codificado& clave) {
    if (!nodo) return {};

    if (clave == nodo->clave)
        return nodo->ids;
    else if (clave < nodo->clave)
        return busquedaInterna(nodo->izquierdo, clave);
    else
        return busquedaInterna(nodo->derecho, clave);
}
void AVL_Busqueda::inorder(NodoAVL* nodo) {
    if (!nodo) return;
    inorder(nodo->izquierdo);
    //cout << "Campo ";
    if (nodo->clave.campo < campos.size())
        cout << campos[nodo->clave.campo]; //decodificar nombre del campo para print
    else
        cout << static_cast<int>(nodo->clave.campo);
    cout << ": ";

    switch (nodo->clave.tipo) {
        case TIPO_ENTERO:
            cout << nodo->clave.valorEntero;
            break;
        case TIPO_FLOTANTE:
            cout << nodo->clave.valorFloat;
            break;
        case TIPO_STRING:
            cout << nodo->clave.valorString;
            break;
    }

    cout << " → IDs: ";
    for (int id : nodo->ids)
        cout << id << " ";
    cout << endl;

    inorder(nodo->derecho);
}


vector<unsigned int> AVL_Busqueda::buscarRango(const string& campo, const string& operador, const string& valor) {
    vector<unsigned int> res;
    int campoIndex = -1;
    for (int i = 0; i < campos.size(); ++i) {
        if (campos[i] == campo) {
            campoIndex = i;
            break;
        }
    }
    if (campoIndex == -1) return res;

    function<void(NodoAVL*)> recorrer = [&](NodoAVL* nodo) {
        if (!nodo) return;
        recorrer(nodo->izquierdo);
        // Solo filtrar por campo y condición aquí, no abortar temprano
        if (nodo->clave.campo == campoIndex) {
            bool cumple = false;
            if (nodo->clave.tipo == TIPO_ENTERO) {
                int cmp = nodo->clave.valorEntero;
                int v = stoi(valor);
                cumple = (operador == "<" && cmp < v) ||
                         (operador == ">" && cmp > v) ||
                         (operador == "<=" && cmp <= v) ||
                         (operador == ">=" && cmp >= v);
            } else if (nodo->clave.tipo == TIPO_FLOTANTE) {
                float cmp = nodo->clave.valorFloat;
                float v = stof(valor);
                cumple = (operador == "<" && cmp < v) ||
                         (operador == ">" && cmp > v) ||
                         (operador == "<=" && cmp <= v) ||
                         (operador == ">=" && cmp >= v);
            }
            if (cumple)
                res.insert(res.end(), nodo->ids.begin(), nodo->ids.end());
        }
        recorrer(nodo->derecho);
    };

    recorrer(root);
    return res;
}