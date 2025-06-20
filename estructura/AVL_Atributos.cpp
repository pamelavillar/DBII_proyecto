#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <algorithm>
#include <memory>
#include <map>
#include <cmath>

#include <queue>
using namespace std;

// Primero declaramos DataValue
using DataValue = std::variant<int, std::string, float>;

// Clase Dato
class Dato {
public:
    DataValue valor;
    vector<int> ids;

    Dato() = default;

    Dato(DataValue v) {
        valor = v;
    }
};

// Función de comparación para DataValue
int compareDataValue(const DataValue& a, const DataValue& b) {
    if (a.index() != b.index()) return a.index() - b.index();

    if (holds_alternative<int>(a))
        return get<int>(a) - get<int>(b);
    else if (holds_alternative<std::string>(a))
        return get<std::string>(a).compare(get<std::string>(b));
    else if (holds_alternative<float>(a)) {
        float diff = get<float>(a) - get<float>(b);
        return (diff > 0) - (diff < 0);
    }
    return 0;
}
    // Estructura para almacenar información de un campo
    struct FieldInfo {
        std::string name;
        std::string type;
        int size;
    };
// Nodo del AVL
struct AVLNode {
    vector<Dato> datos;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const DataValue& val, int ind, int id, int n_attr) {
        datos.resize(n_attr);
        datos[ind] = Dato(val);
        datos[ind].ids.push_back(id);
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

// Clase AVL
class AVLTree {
private:
    AVLNode* root = nullptr;
    vector<AVLNode**> path;

public:
    void insert(const DataValue& val, int indice, int id, int n_attr) {
        path.clear();
        AVLNode** pos;

        if (!find(pos, val, indice)) {
            if (*pos == nullptr) {
                *pos = new AVLNode(val, indice, id, n_attr);
            } else {
                (*pos)->datos[indice].valor = val;
                (*pos)->datos[indice].ids.push_back(id);
            }
            for (int i = path.size() - 1; i >= 0; i--) {
                AVLNode** current = path[i];
                height(*current);
                if(evaluando(current)) break;
            }
            
        } else {
            (*pos)->datos[indice].ids.push_back(id);
        }
    }

    bool find(AVLNode**& pos, const DataValue& val, int ind) {
        pos = &root;
        while (*pos) {
            if ((*pos)->datos[ind].ids.empty()) {
                return false;
            }

            path.push_back(pos);
            int cmp = compareDataValue((*pos)->datos[ind].valor, val);
            if (cmp == 0) return true;
            else if (cmp < 0)
                pos = &((*pos)->right);
            else
                pos = &((*pos)->left);
        }
        return false;
    }


    vector<int> find_atributo(const DataValue& val, int ind) {
        AVLNode** pos;
        if (find(pos, val, ind)) {
            return (*pos)->datos[ind].ids;
        } else {
            return {}; // retornar vector vacío si no se encuentra
        }
    }



    //////--------------- FUNCIONES DE AVL ---------------- /////////
    AVLNode* Root() {
        return root;
    }
    void inorder(AVLNode* n) {
        if (n == nullptr) return;

        inorder(n->left);

        cout << "[";
        for (size_t i = 0; i < n->datos.size(); ++i) {
            std::visit([](auto&& val) {
                cout << val;
            }, n->datos[i].valor);

            if (i != n->datos.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";

        inorder(n->right);
    }

    void inorder2(AVLNode* n) {
        if (n == nullptr) return;

        inorder2(n->left);

        cout << "[";
        for (size_t i = 0; i < n->datos.size(); ++i) {
            if (!n->datos[i].ids.empty()) {
                cout << "Campo " << i << ": ";
                visit([](auto&& val) { cout << val; }, n->datos[i].valor);
                cout << " (IDs: ";
                for (int id : n->datos[i].ids) cout << id << " ";
                cout << ")";
            }
            if (i != n->datos.size() - 1) cout << ", ";
        }
        cout << "]\n";


        inorder2(n->right);
    }

    

    // Dentro de la clase AVLTree

    void printByLevel() {
        if (!root) {
            cout << "Árbol vacío.\n";
            return;
        }

        queue<AVLNode*> q;
        q.push(root);
        int nivel = 0;
        int maxNiveles = 0;

        // Primero calculamos la altura máxima para saber cuántos niveles debe tener
        function<int(AVLNode*)> getHeight = [&](AVLNode* node) -> int {
            if (!node) return 0;
            return 1 + max(getHeight(node->left), getHeight(node->right));
        };
        maxNiveles = getHeight(root);

        int totalNodos = pow(2, maxNiveles) - 1;
        int actualNivel = 0;

        while (!q.empty() && actualNivel < maxNiveles) {
            int nodosEnNivel = pow(2, actualNivel);
            cout << "Nivel " << actualNivel << ": ";

            for (int i = 0; i < nodosEnNivel; ++i) {
                AVLNode* current = q.front();
                q.pop();

                if (current) {
                    cout << "[";
                    for (size_t j = 0; j < current->datos.size(); ++j) {
                        std::visit([](auto&& val) {
                            cout << val;
                        }, current->datos[j].valor);
                        if (j != current->datos.size() - 1)
                            cout << ", ";
                    }
                    cout << "] ";
                    q.push(current->left);
                    q.push(current->right);
                } else {
                    cout << "[] ";
                    q.push(nullptr);
                    q.push(nullptr);
                }
            }
            cout << "\n";
            ++actualNivel;
        }
    }



    int  height(AVLNode* p) {
        if (!p) return 0;
        int l, r;
        l = height(p->left);
        r = height(p->right);

        p->height = 1 + (l > r ? l : r);
        return  p->height;
    }

    int factor_balance(AVLNode* p) {
        if (!p) return false;

        int leftHeight = p->left ? p->left->height : 0;
        int rightHeight = p->right ? p->right->height : 0;
        int factor = rightHeight - leftHeight;

        return factor;
    }


    void rotaRight(AVLNode** p) {
        AVLNode* C = (*p);
        AVLNode* B = C->left;
        AVLNode* A = B->left;

        AVLNode* br = B->right ? B->right : nullptr;

        *p = B;
        C->left = br;
        B->right = C;

        height(C);
        height(B);
    }

    void rotaLeft(AVLNode** p) {
        AVLNode* A = (*p);
        AVLNode* B = A->right;
        AVLNode* C = B->right;

        AVLNode* bl = B->left ? B->left : nullptr;

        *p = B;
        A->right = bl;
        B->left = A;

        height(A); 
        height(B);
    }

    bool evaluando(AVLNode** current){
        
        int balance = factor_balance(*current);

        if (balance == -2) {
            int hijo_izquierdo = factor_balance(((*current)->left));
            if (hijo_izquierdo <= 0) {
                // rotacion simple a la derecha 
                rotaRight(current);
            }
            else {
                // rotacion doble izquierda - derecha
                // rotacion izquierda
                rotaLeft(&((*current)->left));
                //rota derecha
                rotaRight(current);
            }
            return true;
        }
        if (balance == 2) {
            int hijo_derecho = factor_balance((*current)->right);
            if (hijo_derecho >= 0) {
                rotaLeft(current); 
            }
            else {
                rotaRight(&((*current)->right)); 
                rotaLeft(current); 
            }
            return true;
        }
        return false;
    }
    
};

// Función para parsear la cabecera
std::vector<FieldInfo> parseHeader(const std::string& headerLine) {
    std::vector<FieldInfo> fields;
    std::istringstream iss(headerLine);
    std::string fieldDesc;
    
    while (std::getline(iss, fieldDesc, ',')) {
        std::istringstream fss(fieldDesc);
        std::string name, type, sizeStr;
        
        std::getline(fss, name, ':');
        std::getline(fss, type, ':');
        std::getline(fss, sizeStr, ':');
        
        FieldInfo field;
        field.name = name;
        field.type = type;
        field.size = std::stoi(sizeStr);
        
        fields.push_back(field);
    }
    
    return fields;
}

// Función para convertir un string al tipo de dato correcto
DataValue convertToType(const std::string& value, const std::string& type) {
    if (type == "INT") {
        return std::stoi(value);
    } else if (type == "FLOAT") {
        return std::stof(value);
    } else { // VARCHAR u otros se tratan como string
        return value;
    }
}

// Función para parsear una línea de registro
std::vector<DataValue> parseRecord(const std::string& recordLine, 
                                  const std::vector<FieldInfo>& fields) {
    std::vector<DataValue> record;
    std::istringstream iss(recordLine);
    std::string value;
    size_t fieldIndex = 0;
    
    while (std::getline(iss, value, ',') && fieldIndex < fields.size()) {
        record.push_back(convertToType(value, fields[fieldIndex].type));
        fieldIndex++;
    }
    
    return record;
}

int main() {
    AVLTree tree;

    // Insertar 20+ registros con diferentes tipos de datos
    // Enteros (edad)
    tree.insert(25, 0, 101,3);    // ID 101, edad 25
    tree.insert(30, 0, 102,3);
    tree.insert(20, 0, 103,3);
    tree.insert(35, 0, 104,3);
    tree.insert(18, 0, 105,3);
    tree.insert(30, 0, 106,3);
    tree.insert(30, 0, 107,3);
    tree.insert(30, 0, 108,3);
    tree.insert(33, 0, 109,3);
    tree.insert(30, 0, 110,3);

    // Flotantes (promedio)
    tree.insert(15.5f, 1, 201,3); // ID 201, promedio 15.5
    tree.insert(17.3f, 1, 202,3);
    tree.insert(14.2f, 1, 203,3);
    tree.insert(16.8f, 1, 204,3);
    tree.insert(18.0f, 1, 205,3);
    tree.insert(13.7f, 1, 206,3);
    tree.insert(17.3f, 1, 207,3);
    tree.insert(15.0f, 1, 208,3);
    tree.insert(17.3f, 1, 209,3);
    tree.insert(17.3f, 1, 210,3);

    // Strings (nombre)
    tree.insert("Carlos", 2, 301,3); // ID 301, nombre Carlos
    tree.insert("Ana", 2, 302,3);
    tree.insert("Luis", 2, 303,3);
    tree.insert("Maria", 2, 304,3);
    tree.insert("Carlos", 2, 305,3);
    tree.insert("Sofia", 2, 306,3);
    tree.insert("Juan", 2, 307,3);
    tree.insert("Carlos", 2, 308,3);
    tree.insert("Diego", 2, 309,3);
    tree.insert("Elena", 2, 310,3);

    // Mostrar árbol
    cout << "=====================================\n";
    cout << "Recorrido inorder del AVL (ordenado):\n";
    tree.inorder(tree.Root());
    cout << "\n=====================================\n";
    cout << "Recorrido inorder del AVL (ordenado):\n";
    tree.inorder2(tree.Root());
    cout << "\n=====================================\n";
    tree.printByLevel();
    cout << "\n=====================================\n";


    // Pruebas de búsqueda
    cout << "\n--- Pruebas de busqueda ---\n";
    
    // Buscar edades
    cout << "IDs con edad 30: ";
    auto res_edad = tree.find_atributo(30, 0);
    for (int id : res_edad) cout << id << " ";
    


    // Buscar promedios
    cout << "\nIDs con promedio 17.3: ";
    auto res_prom = tree.find_atributo(17.3f, 1);
    for (int id : res_prom) cout << id << " ";



    // Buscar nombres
    cout << "\nIDs con nombre 'Carlos': ";
    auto res_nombre = tree.find_atributo("Carlos", 2);
    for (int id : res_nombre) cout << id << " ";


    // Casos especiales
    cout << "\n--- Casos especiales ---\n";
    cout << "Buscar edad inexistente (50): ";
    auto res_vacio = tree.find_atributo(50, 0);
    if (res_vacio.empty()) cout << "No encontrado";

    cout << "\nBuscar nombre vacio (''): ";
    auto res_vacio_str = tree.find_atributo("", 2);
    if (res_vacio_str.empty()) cout << "No encontrado";

    cout << "\n=====================================\n";
    return 0;
}