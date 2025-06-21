#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include "AVL_ID.cpp"
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
    vector<int> ids; 
    NodoAVL* izquierdo = nullptr;
    NodoAVL* derecho = nullptr;
    int altura = 1;

    NodoAVL(const Campo_Codificado& k, int id) : clave(k) {
        ids.push_back(id);
    }
};

class AVL_Busqueda {
private:
    NodoAVL* root;
    vector<string> campos;
    int altura(NodoAVL* nodo) {
        return nodo ? nodo->altura : 0; }

    int balance(NodoAVL* nodo) {
        return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0; }

    void actualizarAltura(NodoAVL* nodo) {
        nodo->altura = 1 + max(altura(nodo->izquierdo), altura(nodo->derecho)); }

    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    void inorder(NodoAVL* nodo) {
        if (!nodo) return;
        inorder(nodo->izquierdo);

        //cout << "Campo ";
        if (nodo->clave.campo < campos.size())
            cout << campos[nodo->clave.campo];
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
public:
    AVL_Busqueda() : root(nullptr) {}
    NodoAVL* insertar(NodoAVL* nodo, const Campo_Codificado& clave, int id);
    vector<int> buscar(NodoAVL* nodo, const Campo_Codificado& clave);
    void insertar(const Campo_Codificado& clave, int id) {
        root = insertar(root, clave, id);
    }
    void inorder() {
        inorder(root);
    }
    void setCampos(const vector<string>& _campos) {
        campos = _campos;
    }
    vector<int> buscar(const string& campoNombre, const string& valor, const vector<string>& tipoDato) {
        // Buscar el índice del campo
        int campoIndex = -1;
        for (int i = 0; i < campos.size(); ++i) {
            if (campos[i] == campoNombre) {
                campoIndex = i;
                break;
            }
        }
        if (campoIndex == -1 || campoIndex >= tipoDato.size()) return {};

        Campo_Codificado clave;
        clave.campo = campoIndex;

        string tipo = tipoDato[campoIndex];
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

        return buscar(root, clave);
    }
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
NodoAVL* AVL_Busqueda::insertar(NodoAVL* nodo, const Campo_Codificado& clave, int id) {
    if (!nodo) return new NodoAVL(clave, id);

    if (clave == nodo->clave) {
        nodo->ids.push_back(id);
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

vector<int> AVL_Busqueda::buscar(NodoAVL* nodo, const Campo_Codificado& clave) {
    if (!nodo) return {};

    if (clave == nodo->clave)
        return nodo->ids;
    else if (clave < nodo->clave)
        return buscar(nodo->izquierdo, clave);
    else
        return buscar(nodo->derecho, clave);
}
bool insertarRegistross(vector<string>& campos, vector<string>& tipoDato, vector<vector<string>>& registros, AVL_Busqueda& arbol_busqueda, AVL_ID& arbol_id){
    arbol_busqueda.setCampos(campos);
    if (campos.size() != tipoDato.size()) return false;
    for (int i = 0; i < registros.size(); ++i) {
        if (!arbol_id.insert(registros[i])) return false;
        for (int j = 1; j < registros[i].size(); ++j) {
            if (j >= tipoDato.size()) return false;
            Campo_Codificado clave;
            clave.campo = j; 
            
            string tipo = tipoDato[j];
            string valor = registros[i][j];
            
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
                clave.valorString = strdup(valor.c_str()); 
                if (!clave.valorString) return false;
            }
            
            arbol_busqueda.insertar(clave, stoi(registros[i][0]));
        }
    }
    return true;
}

//PROVISIONAL PARA TESTEO:
int main() {
    vector<string> campos_nombre = {"id", "nombre", "edad", "dni"};
    vector<string> tipoDato = {"int", "string", "int", "string"};
    vector<vector<string>> registros = {
        {"1", "Carlos", "20", "12345678"},
        {"2", "Juana", "22", "87654321"},
        {"3", "Carlos", "21", "11112222"},
        {"4", "Maria", "18", "45651354"},
        {"5", "Ivan", "20", "78945612"}
    };
    AVL_Busqueda arbol_busqueda;
    AVL_ID arbol_id;
    if (!insertarRegistross(campos_nombre, tipoDato, registros, arbol_busqueda, arbol_id)) {
        cout << "Error al insertar registros" << endl;
        return 1;
    } else cout << "Registros insertados exitosamente" << endl;
    arbol_id.inorder(arbol_id.Root());
    cout << "\nÁrbol de atributos (campo:valor → IDs):\n";
    arbol_busqueda.inorder();

    cout << "\nBuscando IDs donde nombre == Carlos\n";
    vector<int> ids = arbol_busqueda.buscar("nombre", "Carlos", tipoDato);
    for (int id : ids)
        cout << "ID: " << id << "  ";

    cout << "\n\nRegistros completos donde nombre == Carlos:\n";
    for (int id : ids) {
        CNode** nodoPtr;
        if (arbol_id.find(nodoPtr, id)) {
            const vector<string>& reg = (*nodoPtr)->registro;
            for (int k = 0; k < reg.size(); ++k) {
                cout << campos_nombre[k] << ": " << reg[k];
                if (k != reg.size() - 1) cout << " | ";
            }
            cout << endl;
        } else {
            cout << "ID " << id << " no encontrado en arbol_id\n";
        }
    }

    return 0;
}
