#include <iostream>
#include <vector>
#include <string>
#include <fstream> // Para manejo de archivos
#include <sstream> // Para parsear líneas y palabras
#include <limits>  // Para limpiar el buffer de entrada
#include <cctype>  // Para isalnum

using namespace std;

// Estructura para almacenar la información de cada palabra (compartida)
struct PalabraInfo {
    string palabra;
    string traduccion;
    string funcionalidad; // Aunque el traductor no la usa directamente
};

// --- Prototipos de Funciones Compartidas ---
void cargarDatos(vector<PalabraInfo>& diccionario, const string& nombreArchivo);
void guardarDatos(const vector<PalabraInfo>& diccionario, const string& nombreArchivo);
int buscarIndicePalabra(const vector<PalabraInfo>& diccionario, const string& palabraBuscar);
string traducirPalabra(const string& palabraOriginal, const vector<PalabraInfo>& diccionario);

// --- Prototipos de Funciones del Gestor de Diccionario ---
void gestionarDiccionario(vector<PalabraInfo>& diccionario);
void agregarPalabra(vector<PalabraInfo>& diccionario);
void mostrarPalabras(const vector<PalabraInfo>& diccionario);
void actualizarPalabra(vector<PalabraInfo>& diccionario);
void eliminarPalabra(vector<PalabraInfo>& diccionario);

// --- Prototipos de Funciones del Traductor ---
void traducirCodigo(const vector<PalabraInfo>& diccionario);

// --- Nombre del archivo (compartido) ---
const string NOMBRE_ARCHIVO = "palabras_cpp.txt";

int main() {
    vector<PalabraInfo> diccionario;
    cargarDatos(diccionario, NOMBRE_ARCHIVO); // Carga datos al iniciar para ambos módulos

    int opcionPrincipal;

    do {
        cout << "\n===== MENU PRINCIPAL =====" << endl;
        cout << "1. Gestionar Diccionario (Agregar/Ver/Editar/Borrar Palabras)" << endl;
        cout << "2. Traducir Codigo C++ (usando diccionario)" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";

        while (!(cin >> opcionPrincipal)) {
            cout << "Entrada invalida. Por favor ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcionPrincipal) {
            case 1:
                gestionarDiccionario(diccionario);
                break;
            case 2:
                traducirCodigo(diccionario);
                break;
            case 3:
                guardarDatos(diccionario, NOMBRE_ARCHIVO);
                cout << "Datos guardados. Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcionPrincipal != 3);

    return 0;
}

// --- Implementación de Funciones Compartidas ---

void cargarDatos(vector<PalabraInfo>& diccionario, const string& nombreArchivo) {
    ifstream archivoEntrada(nombreArchivo);
    string linea;
    PalabraInfo temp;
    bool archivoAbierto = false;

    diccionario.clear();

    if (archivoEntrada.is_open()) {
        archivoAbierto = true;
        while (getline(archivoEntrada, linea)) {
            stringstream ss(linea);
            if (getline(ss, temp.palabra, ',') &&
                getline(ss, temp.traduccion, ',') &&
                getline(ss, temp.funcionalidad))
            {
                diccionario.push_back(temp);
            } else {
                 if (!linea.empty()) { // No mostrar advertencia para líneas vacías en el archivo
                    cerr << "Advertencia: Linea ignorada en '" << nombreArchivo << "' por formato incorrecto: " << linea << endl;
                 }
            }
        }
        archivoEntrada.close();
    } else {
         cout << "Advertencia: No se pudo abrir el archivo '" << nombreArchivo << "' para cargar." << endl;
         cout << "El diccionario estara vacio inicialmente. Use la opcion 1 para agregar palabras." << endl;
    }

    if (archivoAbierto && diccionario.empty() && !archivoEntrada.eof() && archivoEntrada.fail() && !archivoEntrada.bad()) {
        // Si se abrió pero está vacío (y no es por error de lectura)
         cout << "Archivo '" << nombreArchivo << "' encontrado pero esta vacio." << endl;
    } else if (archivoAbierto) {
cout << "Datos cargados desde '" << nombreArchivo << "'." << endl;
    }
     cout << "Actualmente hay " << diccionario.size() << " palabras en el diccionario." << endl;
}


void guardarDatos(const vector<PalabraInfo>& diccionario, const string& nombreArchivo) {
    ofstream archivoSalida(nombreArchivo, ios::trunc);
    if (!archivoSalida.is_open()) {
        cerr << "Error critico: No se pudo abrir el archivo '" << nombreArchivo << "' para guardar." << endl;
        return;
    }
    for (const auto& p : diccionario) {
        archivoSalida << p.palabra << "," << p.traduccion << "," << p.funcionalidad << endl;
    }
    archivoSalida.close();
}

int buscarIndicePalabra(const vector<PalabraInfo>& diccionario, const string& palabraBuscar) {
    for (size_t i = 0; i < diccionario.size(); ++i) {
        if (diccionario[i].palabra == palabraBuscar) {
            return i;
        }
    }
    return -1;
}

string traducirPalabra(const string& palabraOriginal, const vector<PalabraInfo>& diccionario) {
    for (const auto& info : diccionario) {
        if (info.palabra == palabraOriginal) {
            return info.traduccion.empty() ? palabraOriginal : info.traduccion;
        }
    }
    return palabraOriginal;
}


// --- Implementación de Funciones del Gestor de Diccionario ---

void gestionarDiccionario(vector<PalabraInfo>& diccionario) {
    int opcionDiccionario;
    do {
        cout << "\n--- Gestionar Diccionario ---" << endl;
        cout << "1. Agregar Palabra" << endl;
        cout << "2. Mostrar Todas las Palabras" << endl;
        cout << "3. Actualizar Palabra" << endl;
        cout << "4. Eliminar Palabra" << endl;
        cout << "5. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";

        while (!(cin >> opcionDiccionario)) {
            cout << "Entrada invalida. Ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer

        switch (opcionDiccionario) {
            case 1: agregarPalabra(diccionario); break;
            case 2: mostrarPalabras(diccionario); break;
            case 3: actualizarPalabra(diccionario); break;
            case 4: eliminarPalabra(diccionario); break;
            case 5: cout << "Regresando al menu principal..." << endl; break;
            default: cout << "Opcion no valida." << endl; break;
        }
    } while (opcionDiccionario != 5);
}

void agregarPalabra(vector<PalabraInfo>& diccionario) {
    PalabraInfo nuevaPalabra;
    cout << "Ingrese la nueva palabra clave (ej: if): ";
    getline(cin, nuevaPalabra.palabra);
    nuevaPalabra.palabra.erase(0, nuevaPalabra.palabra.find_first_not_of(" \t\n\r"));
    nuevaPalabra.palabra.erase(nuevaPalabra.palabra.find_last_not_of(" \t\n\r") + 1);

    if (nuevaPalabra.palabra.empty()){
        cout << "Error: La palabra clave no puede estar vacia." << endl;
        return;
    }

    if (buscarIndicePalabra(diccionario, nuevaPalabra.palabra) != -1) {
        cout << "Error: La palabra '" << nuevaPalabra.palabra << "' ya existe." << endl;
        return;
    }

    cout << "Ingrese la traduccion (ej: si): ";
    getline(cin, nuevaPalabra.traduccion);
    cout << "Ingrese la funcionalidad (descripcion): ";
    getline(cin, nuevaPalabra.funcionalidad);

    diccionario.push_back(nuevaPalabra);
    cout << "Palabra '" << nuevaPalabra.palabra << "' agregada con exito." << endl;
}

void mostrarPalabras(const vector<PalabraInfo>& diccionario) {
    if (diccionario.empty()) {
        cout << "El diccionario esta vacio." << endl;
        return;
    }
    cout << "\n--- Listado de Palabras en Diccionario ---" << endl;
    for (size_t i = 0; i < diccionario.size(); ++i) {
