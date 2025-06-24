#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits>
#include "AVL_ID.cpp"
#include "AVL_Busqueda.cpp"
using namespace std;

enum Operador {
    AND,
    OR
};

class Buscador {
private:
    AVL_Busqueda arbol_busqueda;
    AVL_ID arbol_id;
    vector<unsigned int> combinarVec(vector<unsigned int> final, vector<unsigned int> parcial, Operador op);
public:
    Buscador(vector<string>& _campos, vector<string>& _tipoDato, vector<vector<string>>& registros);
    bool busquedaCompleta(vector<string>& camposBuscados, vector<string>& valoresBuscados, Operador ope);
    void print();

};

Buscador::Buscador(vector<string>& _campos, vector<string>& _tipoDato, vector<vector<string>>& registros) {
    arbol_busqueda.setCampos(_campos);
    arbol_busqueda.setTipos(_tipoDato);
    if (arbol_busqueda.campos.size() != arbol_busqueda.tipos.size()) throw invalid_argument("Error en tamaño de vectores :( \n");;

    for (int i = 0; i < registros.size(); ++i) {
        if (!arbol_id.insert(registros[i])) throw invalid_argument("Error en insercion de AVL_ID :( \n");
        for (int j = 1; j < registros[i].size(); ++j) {
            if (j >= arbol_busqueda.tipos.size()) throw invalid_argument("Error! :( \n");
            Campo_Codificado clave;
            clave.campo = j;
            string tipo = arbol_busqueda.tipos[j];
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
                if (!clave.valorString) throw invalid_argument("Error! :(");
            }

            unsigned long raw_id = stoul(registros[i][0]);
            if (raw_id > numeric_limits<unsigned int>::max()) throw invalid_argument("Error! :(");
            unsigned int id = static_cast<unsigned int>(raw_id);
            arbol_busqueda.insertar(clave, id);
        }
    }
}

vector<unsigned int> Buscador::combinarVec(vector<unsigned int> final, vector<unsigned int> parcial, Operador op) {
    int i = 0, j = 0;
    vector<unsigned int> resultado;
    if (op == AND) {
        while (i < final.size() && j < parcial.size()) {
            if (final[i] < parcial[j]) ++i;
            else if (parcial[j] < final[i]) ++j;
            else {
                resultado.push_back(final[i]);
                ++i;
                ++j;
            }
        }
    } else if (op == OR) {
        while (i < final.size() && j < parcial.size()) {
            if (final[i] < parcial[j]) {
                resultado.push_back(final[i]);
                ++i;
            } else if (parcial[j] < final[i]) {
                resultado.push_back(parcial[j]);
                ++j;
            } else {
                resultado.push_back(final[i]);
                ++i;
                ++j;
            }
        }
        while (i < final.size()) {
            resultado.push_back(final[i]);
            ++i;
        }
        while (j < parcial.size()) {
            resultado.push_back(parcial[j]);
            ++j;
        }
    }
    return resultado;
}

bool Buscador::busquedaCompleta(vector<string>& camposBuscados, vector<string>& valoresBuscados, Operador ope) {
    if (camposBuscados.size() != valoresBuscados.size()) return false;
    vector<unsigned int> idFinal;
    for (int i = 0; i < camposBuscados.size(); i++) {
        cout << "\nBuscando IDs donde " << camposBuscados[i] << " == " << valoresBuscados[i] << endl;
        vector<unsigned int> ids;
        if (camposBuscados[i] == "id") {
            unsigned long raw_id = stoul(valoresBuscados[i]);
            if (raw_id <= numeric_limits<unsigned int>::max()) {
                unsigned int id = static_cast<unsigned int>(raw_id);
                CNode** nodoPtr;
                if (arbol_id.find(nodoPtr, id))
                    ids.push_back(id);
            }
        } else {
            ids = arbol_busqueda.buscar(camposBuscados[i], valoresBuscados[i]);
        }

        if (i == 0) idFinal = ids;
        else idFinal = combinarVec(idFinal, ids, ope);
    }

    cout << "\n-----------------------\n";
    if(idFinal.size()<1) cout << "No hay registros con los parametros especificados :(\n";
    //else {
        cout << "Registros encontrados:\n";
        for (unsigned int id : idFinal) {
            CNode** nodoPtr;
            if (arbol_id.find(nodoPtr, id)) {
                const vector<string>& reg = (*nodoPtr)->registro;
                for (int k = 0; k < reg.size(); ++k) {
                    cout << arbol_busqueda.campos[k] << ": " << reg[k];
                    if (k != reg.size() - 1) cout << " | ";
                }
                cout << endl;
            } else {
                cout << "ID " << id << " no encontrado en arbol_id! :(\n";
                return false;
            }
        }
    //}
    
    return true;
}

void Buscador::print(){
    arbol_id.inorder(arbol_id.Root());
    cout << "\nÁrbol de atributos (campo:valor → IDs):\n";
    arbol_busqueda.inorder();
}

//PROVISIONAL PARA TESTEO:
int main() {
    vector<string> campos_nombre = {"id", "nombre", "edad", "dni", "altura", "promedio"};
    vector<string> tipoDato = {"int", "string", "int", "string", "float", "float"};
    vector<vector<string>> registros = {
        {"1", "Carlos", "20", "12345678", "1.75", "15.5"},
        {"2", "Juana", "22", "87654321", "1.62", "18.3"},
        {"3", "Carlos", "21", "11112222", "1.80", "17.0"},
        {"4", "Maria", "18", "45651354", "1.60", "14.9"},
        {"5", "Ivan", "20", "78945612", "1.72", "16.4"},
        {"6", "Carlos", "20", "32165498", "1.75", "15.5"},
        {"7", "Juana", "19", "99999999", "1.62", "17.8"},
        {"8", "Pedro", "23", "65498712", "1.82", "19.1"},
        {"9", "Maria", "21", "14725836", "1.60", "16.0"},
        {"10", "Lucia", "22", "85236974", "1.70", "18.0"},
        {"11", "Andres", "24", "35795148", "1.78", "19.5"},
        {"12", "Julia", "25", "15975326", "1.65", "17.2"},
        {"13", "Carlos", "20", "75315984", "1.75", "15.5"},
        {"14", "Juana", "23", "95135762", "1.62", "18.0"},
        {"15", "Maria", "19", "35745612", "1.60", "16.8"},
        {"16", "Hector", "22", "45632178", "1.85", "19.0"},
        {"17", "Carlos", "21", "74185296", "1.78", "17.4"},
        {"18", "Ivan", "20", "96325874", "1.72", "16.7"},
        {"19", "Carlos", "22", "85274196", "1.75", "15.8"},
        {"20", "Sofia", "20", "74196385", "1.70", "17.9"},
        {"21", "Carlos", "19", "32178965", "1.74", "15.0"},
        {"22", "Juana", "20", "78912345", "1.62", "18.4"},
        {"23", "Maria", "21", "96314725", "1.60", "16.6"},
        {"24", "Carlos", "22", "75325841", "1.76", "15.3"},
        {"25", "Lucia", "23", "78945612", "1.70", "18.2"},
        {"26", "Pedro", "24", "32165487", "1.82", "19.2"},
        {"27", "Carlos", "25", "85296374", "1.79", "15.1"},
        {"28", "Juana", "26", "95125874", "1.62", "18.7"},
        {"29", "Maria", "27", "45698732", "1.60", "16.5"},
        {"30", "Carlos", "28", "78912378", "1.75", "15.0"}
    };
    try {
        //creacion
        Buscador buscador(campos_nombre, tipoDato, registros);
        cout << "REGISTROS INSERTADOS EXITOSAMENTE\nArbol ID:" << endl;
        //impresion
        buscador.print();
        //ejemplo de busquedaa
        vector<string> campos_bus = {"id", "nombre"};
        vector<string> valores_bus = {"20", "Ivan"};
        if (!buscador.busquedaCompleta(campos_bus, valores_bus, AND)) {//especificar operador OR o AND xd
            cout << "Error en la busqueda" << endl;
            return 1;
        } else cout << "\nBUSQUEDA REALIZADA EXITOSAMENTE\n" << endl;

    } catch (const exception& e) {
        cerr << "Error al crear Buscador: " << e.what();
    }
    

    return 0;
}