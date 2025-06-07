#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <algorithm>
#include <memory>
#include <map>

// Definición de los tipos de datos soportados
using DataValue = std::variant<int, std::string, float>;

// Estructura para almacenar información de un campo
struct FieldInfo {
    std::string name;
    std::string type;
    int size;
};

// Nodo del AVL
template <typename T>
struct AVLNode {
    std::vector<T> data;
    std::unique_ptr<AVLNode<T>> left;
    std::unique_ptr<AVLNode<T>> right;
    int height;
    
    AVLNode(const std::vector<T>& val) 
        : data(val), left(nullptr), right(nullptr), height(1) {}
};

// Clase AVL
template <typename T>
class AVLTree {
private:
    std::unique_ptr<AVLNode<T>> root;
    
    // Métodos de implementación del AVL (insert, balance, rotations, etc.)
    // ...
    
public:
    void insert(const std::vector<T>& data) {
        // Implementación de inserción
    }
    
    // Otros métodos necesarios
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
    std::ifstream file("datos.txt");
    std::string line;
    
    // Leer cabecera
    std::getline(file, line);
    auto fields = parseHeader(line);
    
    // Crear AVL tree
    AVLTree<DataValue> database;
    
    // Leer registros
    while (std::getline(file, line)) {
        auto record = parseRecord(line, fields);
        database.insert(record);
    }
    
    // Operaciones con la base de datos...
    
    return 0;
}