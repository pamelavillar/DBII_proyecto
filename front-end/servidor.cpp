#include "httplib.h"
#include "json.hpp"
#include "procesador_filtro.hpp" 
#include "../DatabaseManager.h" // tu clase que implementa buscar(), constructor, etc.
#include <iostream>
#include <vector>
#include <memory> // para usar std::unique_ptr si quieres

using namespace std;
using json = nlohmann::json;

// Creamos un puntero global al manager (puedes usar raw o smart pointer)
DatabaseManager* dbmanager = nullptr;

int main() {
    httplib::Server svr;

    svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        cout << req.method << " " << req.path << " -> " << res.status << endl;
    });

/*
    // Ruta POST para configurar la base de datos
    svr.Post("/configurar", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json config = json::parse(req.body);

            int platos = stoi(config["platos"].get<string>());
            int pistas = stoi(config["pistas"].get<string>());
            int sectores = stoi(config["sectores"].get<string>());
            int capacidad = stoi(config["capacidad"].get<string>());

            // liberar si ya existía
            if (dbmanager) delete dbmanager;

            // crear nuevo con configuración
            dbmanager = new DatabaseManager(platos,2, pistas, sectores, capacidad);

            res.set_content("Configuración cargada correctamente", "text/plain");
        } catch (const exception& e) {
            cerr << "Error al configurar DBManager: " << e.what() << endl;
            res.status = 400;
            res.set_content("Error en configuración", "text/plain");
        }
    });

    */

    // Ruta POST para búsquedas
    svr.Post("/buscar", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        /*
        
        if (!dbmanager) {
            res.status = 500;
            res.set_content("Base de datos no configurada", "text/plain");
            return;
        }
*/
        try {
            json payload = json::parse(req.body);
            //json resultados = procesarPeticionCompleta(payload, dbmanager);
            json resultados = procesarPeticionCompleta1(payload);
            res.set_content(resultados.dump(), "application/json");
        } catch (const exception& e) {
            cerr << "Error al procesar búsqueda: " << e.what() << endl;
            res.status = 400;
            res.set_content("Error al procesar búsqueda", "text/plain");
        }
    });

    // Preflight
    svr.Options("/configurar", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    svr.Options("/buscar", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    cout << "Servidor escuchando en http://localhost:8080\n";
    svr.listen("localhost", 8080);
}
