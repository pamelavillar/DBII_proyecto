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
enum Orden {
    NO,
    ASC,
    DES
};

class Buscador {
private:
    AVL_Busqueda arbol_busqueda;
    AVL_ID arbol_id;
    vector<unsigned int> combinarVec(vector<unsigned int> final, vector<unsigned int> parcial, Operador op);
public:
    Buscador(vector<string>& _campos, vector<string>& _tipoDato);
    vector<vector<string>> busquedaCompleta(vector<string>& camposBuscados, vector<string>& valoresBuscados, vector<string>& camposEsperados, Operador ope, string& orden, Orden dirOrden);
    void print();
    bool insertReg(vector<string> &registro);
};

Buscador::Buscador(vector<string>& _campos, vector<string>& _tipoDato) {
    arbol_busqueda.setCampos(_campos);
    arbol_busqueda.setTipos(_tipoDato);
    if (arbol_busqueda.campos.size() != arbol_busqueda.tipos.size()) throw invalid_argument("Error en tamaño de vectores :( \n");;
/*
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
        */
}
bool Buscador::insertReg(vector<string> &registro){
    if (!arbol_id.insert(registro)) return false;
    for (int j = 1; j < registro.size(); ++j) {
        if (j >= arbol_busqueda.tipos.size()) return false;
        Campo_Codificado clave;
        clave.campo = j;
        string tipo = arbol_busqueda.tipos[j];
        string valor = registro[j];

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

        unsigned long raw_id = stoul(registro[0]);
        if (raw_id > numeric_limits<unsigned int>::max()) return false;
        unsigned int id = static_cast<unsigned int>(raw_id);
        arbol_busqueda.insertar(clave, id);
    }
    return true;
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

vector<vector<string>> Buscador::busquedaCompleta(vector<string>& camposBuscados, vector<string>& valoresBuscados, vector<string>& camposEsperados, Operador ope, string& orden, Orden dirOrden) {
    vector<vector<string>> resu;
    if (camposBuscados.size() != valoresBuscados.size()) return {{"ERROR"}};
    vector<unsigned int> idFinal;

    for (int i = 0; i < camposBuscados.size(); i++) {
        vector<unsigned int> ids;
        string campo = camposBuscados[i];
        string valor = valoresBuscados[i];

        if (campo == "id") {
            try {
                unsigned long raw_id = stoul(valor);
                if (raw_id <= numeric_limits<unsigned int>::max()) {
                    unsigned int id = static_cast<unsigned int>(raw_id);
                    CNode** nodoPtr;
                    if (arbol_id.find(nodoPtr, id))
                        ids.push_back(id);
                }
            } catch (...) { return {{"ERROR"}}; }
        } else {
            auto itTipo = find(arbol_busqueda.campos.begin(), arbol_busqueda.campos.end(), campo);
            if (itTipo == arbol_busqueda.campos.end()) return {{"ERROR"}};
            int idxTipo = itTipo - arbol_busqueda.campos.begin();
            string tipo = arbol_busqueda.tipos[idxTipo];

            if (valor.find("..") != string::npos) {
                size_t pos = valor.find("..");
                string valMin = valor.substr(0, pos);
                string valMax = valor.substr(pos + 2);
                vector<unsigned int> idsMin = arbol_busqueda.buscarRango(campo, ">=", valMin);
                vector<unsigned int> idsMax = arbol_busqueda.buscarRango(campo, "<=", valMax);
                ids = combinarVec(idsMin, idsMax, AND);
            } else {
                string operador = "=";
                if ((valor[0] == '<' || valor[0] == '>') && valor[1] == '=') {
                    operador = valor.substr(0, 2);
                    valor = valor.substr(2);
                } else if (valor[0] == '<' || valor[0] == '>') {
                    operador = valor.substr(0, 1);
                    valor = valor.substr(1);
                }

                if (operador == "=") {
                    ids = arbol_busqueda.buscar(campo, valor);
                } else {
                    ids = arbol_busqueda.buscarRango(campo, operador, valor);
                }
            }
        }

        if (i == 0) {
            idFinal = ids;
        } else {
            sort(idFinal.begin(), idFinal.end());
            sort(ids.begin(), ids.end());
            idFinal = combinarVec(idFinal, ids, ope);
        }
    }

    if (idFinal.empty()) return {{"VACIO"}};

    for (unsigned int id : idFinal) {
        CNode** nodoPtr;
        if (arbol_id.find(nodoPtr, id)) {
            const vector<string>& reg = (*nodoPtr)->registro;
            vector<string> temp;
            for (int k = 0; k < camposEsperados.size(); ++k) {
                auto it = find(arbol_busqueda.campos.begin(), arbol_busqueda.campos.end(), camposEsperados[k]);
                if (it != arbol_busqueda.campos.end()) {
                    int idx = it - arbol_busqueda.campos.begin();
                    temp.push_back(reg[idx]);
                }
            }
            resu.push_back(temp);
        }
    }

    auto it = find(camposEsperados.begin(), camposEsperados.end(), orden);
    if (it != camposEsperados.end()) {
        int idxOrden = it - camposEsperados.begin();
        auto itTipo = find(arbol_busqueda.campos.begin(), arbol_busqueda.campos.end(), orden);
        int idxTipo = itTipo - arbol_busqueda.campos.begin();
        string tipoOrden = arbol_busqueda.tipos[idxTipo];

        if (tipoOrden == "int") {
            sort(resu.begin(), resu.end(), [=](const vector<string>& a, const vector<string>& b) {
                return dirOrden == ASC ? stoi(a[idxOrden]) < stoi(b[idxOrden]) : stoi(a[idxOrden]) > stoi(b[idxOrden]);
            });
        } else if (tipoOrden == "float") {
            sort(resu.begin(), resu.end(), [=](const vector<string>& a, const vector<string>& b) {
                return dirOrden == ASC ? stof(a[idxOrden]) < stof(b[idxOrden]) : stof(a[idxOrden]) > stof(b[idxOrden]);
            });
        } else {
            sort(resu.begin(), resu.end(), [=](const vector<string>& a, const vector<string>& b) {
                return dirOrden == ASC ? a[idxOrden] < b[idxOrden] : a[idxOrden] > b[idxOrden];
            });
        }
    }

    return resu;
}

void Buscador::print(){
    cout << "Arbol ID:" << endl;
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
        {"14", "Rodrigo", "23", "95135762", "1.62", "18.0"},
        {"15", "Maria", "19", "35745612", "1.60", "16.8"},
        {"16", "Hector", "22", "45632178", "1.85", "19.0"},
        {"17", "Carlos", "21", "74185296", "1.78", "17.4"},
        {"18", "Ivan", "20", "96325874", "1.72", "16.7"},
        {"19", "Carolina", "22", "85274196", "1.75", "15.8"},
        {"20", "Sofia", "20", "74196385", "1.70", "17.9"},
        {"21", "Carlos", "19", "32178965", "1.74", "15.0"},
        {"22", "Juana", "20", "78912345", "1.62", "18.4"},
        {"23", "Maria", "21", "96314725", "1.60", "16.6"},
        {"24", "Pamela", "17", "75325841", "1.76", "19.3"},
        {"25", "Lucia", "23", "78945612", "1.70", "18.2"},
        {"26", "Pedro", "24", "32165487", "1.82", "19.2"},
        {"27", "Carlos", "25", "85296374", "1.79", "15.1"},
        {"28", "Juana", "26", "95125874", "1.62", "18.7"},
        {"29", "Maria", "27", "45698732", "1.60", "16.5"},
        {"30", "Carlos", "28", "78912378", "1.75", "15.0"},
        {"31", "Juana", "19", "95175384", "1.62", "18.8"},
        {"32", "Maria", "30", "35715926", "1.60", "16.9"},
        {"33", "Hector", "31", "25896314", "1.85", "19.3"},
        {"34", "Carlos", "32", "14785236", "1.78", "17.5"},
        {"35", "Ivan", "13", "36925874", "1.72", "16.8"},
        {"36", "Carlos", "34", "74196385", "1.75", "15.6"},
        {"37", "Sofia", "15", "85274196", "1.70", "18.1"},
        {"38", "Carla", "16", "96325874", "1.74", "15.2"},
        {"39", "Juana", "37", "15975326", "1.62", "18.5"},
        {"40", "Maria", "18", "75315984", "1.60", "16.7"}
    };
    try {
        //creacion
        Buscador buscador(campos_nombre, tipoDato);
        
        for(auto& registro : registros){
            if(!buscador.insertReg(registro)) cerr << "ERROR en la insercion de registross :(\n";
        }
        cout << "REGISTROS INSERTADOS EXITOSAMENTE :)\n\n";
        //impresion
        buscador.print();
        //ejemplo de busquedaa
        vector<string> campos_bus = {"edad","promedio"};
        vector<string> valores_bus = {">=17",">19"}; // soporte de operadores: <  >  <=  >= ..
        vector<string> valores_esp = {"nombre", "edad","promedio"}; //ingresar en orden correcto
        string ord = "edad"; //en base a q se ordena

        vector<vector<string>> resuConsulta = buscador.busquedaCompleta(campos_bus, valores_bus, valores_esp, AND, ord, ASC);//especificar operador OR o AND - especificar ASC, DES, NO

        if (resuConsulta.size() == 1 && resuConsulta[0][0] == "ERROR") {
            cout << "Error en la consulta.\n";
        } else if (resuConsulta.size() == 1 && resuConsulta[0][0] == "VACIO") {
            cout << "No se encontraron resultados.\n";
        } else {
            cout << "--------------------------------------------------\nResultados encontrados: " << resuConsulta.size() << "\n";
            for (const auto& fila : resuConsulta) {
                for (int i = 0; i < fila.size(); ++i) {
                    cout << fila[i];
                    if (i != fila.size() - 1) cout << " | ";
                }
                cout << '\n';
            }
            cout << '\n';
        }

    } catch (const exception& e) {
        cerr << "Error al crear Buscador: " << e.what();
    }
    
    return 0;
}