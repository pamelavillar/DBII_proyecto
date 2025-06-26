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
using DataValue = std::variant<int, std::string, float, bool, char>;

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
    else if (holds_alternative<bool>(a))
        return get<bool>(a) - get<bool>(b); // false < true
    else if (holds_alternative<char>(a))
        return get<char>(a) - get<char>(b);
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
    Dato datos;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const DataValue& val, int id) {
        datos = Dato(val);
        datos.ids.push_back(id);
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
    void insert(const DataValue& val, int id) {
        path.clear();
        AVLNode** pos;

        if (!find(pos, val)) {
            if (*pos == nullptr) {
                *pos = new AVLNode(val, id);
            } else {
                (*pos)->datos.valor = val;
                (*pos)->datos.ids.push_back(id);
            }
            for (int i = path.size() - 1; i >= 0; i--) {
                AVLNode** current = path[i];
                height(*current);
                if(evaluando(current)) break;
            }
            
        } else {
            (*pos)->datos.ids.push_back(id);
        }
    }

    bool find(AVLNode**& pos, const DataValue& val) {
        pos = &root;
        while (*pos) {
            path.push_back(pos);
            int cmp = compareDataValue((*pos)->datos.valor, val);
            if (cmp == 0) return true;
            else if (cmp < 0)
                pos = &((*pos)->right);
            else
                pos = &((*pos)->left);
        }
        return false;
    }


    vector<int> find_atributo(const DataValue& val) {
        AVLNode** pos;
        if (find(pos, val)) {
            return (*pos)->datos.ids;
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
        std::visit([](auto&& val) {
                        cout << val;
                    }, n->datos.valor);
                cout << "\n";

        inorder(n->right);
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
                    std::visit([](auto&& val) {
                            cout << val;
                        }, current->datos.valor);

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
    AVLTree tree_edad;
    AVLTree tree_promedio;
    AVLTree tree_nombre;

    // Insertar 20+ registros con diferentes tipos de datos
    // Enteros (edad)
    tree_edad.insert(25, 101);    // ID 101, edad 25
    tree_edad.insert(30,102);
    tree_edad.insert(20, 103);
    tree_edad.insert(35, 104);
    tree_edad.insert(18, 105);
    tree_edad.insert(30, 106);
    tree_edad.insert(30, 107);
    tree_edad.insert(30, 108);
    tree_edad.insert(33, 109);
    tree_edad.insert(30, 110);

    // Flotantes (promedio)
    tree_promedio.insert(15.5f, 201); // ID 201, promedio 15.5
    tree_promedio.insert(17.3f,  202);
    tree_promedio.insert(14.2f,  203);
    tree_promedio.insert(16.8f,  204);
    tree_promedio.insert(18.0f,  205);
    tree_promedio.insert(13.7f,  206);
    tree_promedio.insert(17.3f,  207);
    tree_promedio.insert(15.0f,  208);
    tree_promedio.insert(17.3f,  209);
    tree_promedio.insert(17.3f,  210);

    // Strings (nombre)
    tree_nombre.insert("Carlos", 301); // ID 301, nombre Carlos
    tree_nombre.insert("Ana",  302);
    tree_nombre.insert("Luis",  303);
    tree_nombre.insert("Maria",  304);
    tree_nombre.insert("Carlos",  305);
    tree_nombre.insert("Sofia", 306);
    tree_nombre.insert("Juan",  307);
    tree_nombre.insert("Carlos",  308);
    tree_nombre.insert("Diego", 309);
    tree_nombre.insert("Elena", 310);

    // Mostrar árbol
    cout << "=====================================\n";
    cout << "Recorrido inorder del AVL (ordenado):\n";
    tree_edad.inorder(tree_edad.Root());
    tree_promedio.inorder(tree_promedio.Root());
    tree_nombre.inorder(tree_nombre.Root());
    cout << "\n=====================================\n";

    cout << "\n=====================================\n";
    tree_edad.printByLevel();
    tree_promedio.printByLevel();
    tree_nombre.printByLevel();
    cout << "\n=====================================\n";


    // Pruebas de búsqueda
    cout << "\n--- Pruebas de busqueda ---\n";
    
    // Buscar edades
    cout << "IDs con edad 30: ";
    auto res_edad = tree_edad.find_atributo(30);
    for (int id : res_edad) cout << id << " ";
    


    // Buscar promedios
    cout << "\nIDs con promedio 17.3: ";
    auto res_prom = tree_promedio.find_atributo(17.3f);
    for (int id : res_prom) cout << id << " ";



    // Buscar nombres
    cout << "\nIDs con nombre 'Carlos': ";
    auto res_nombre = tree_nombre.find_atributo("Carlos");
    for (int id : res_nombre) cout << id << " ";


    // Casos especiales
    cout << "\n--- Casos especiales ---\n";
    cout << "Buscar edad inexistente (50): ";
    auto res_vacio = tree_edad.find_atributo(50);
    if (res_vacio.empty()) cout << "No encontrado";

    cout << "\nBuscar nombre vacio (''): ";
    auto res_vacio_str = tree_nombre.find_atributo("");
    if (res_vacio_str.empty()) cout << "No encontrado";

    cout << "\n=====================================\n";
    return 0;
}