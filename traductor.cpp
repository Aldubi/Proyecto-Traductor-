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
cout << i + 1 << ". Palabra:      " << diccionario[i].palabra << endl;
        cout << "   Traduccion:   " << diccionario[i].traduccion << endl;
        cout << "   Funcionalidad:" << diccionario[i].funcionalidad << endl;
        cout << "------------------------------------------" << endl;
    }
}

void actualizarPalabra(vector<PalabraInfo>& diccionario) {
     if (diccionario.empty()) {
        cout << "El diccionario esta vacio. No hay palabras para actualizar." << endl;
        return;
    }
    string palabraBuscar;
    cout << "Ingrese la palabra clave que desea actualizar: ";
    getline(cin, palabraBuscar);

    int indice = buscarIndicePalabra(diccionario, palabraBuscar);

    if (indice != -1) {
        cout << "Palabra encontrada. Ingrese los nuevos datos:" << endl;
        cout << "Traduccion actual ('" << diccionario[indice].traduccion << "'). Nueva traduccion: ";
        getline(cin, diccionario[indice].traduccion);
        cout << "Funcionalidad actual ('" << diccionario[indice].funcionalidad << "'). Nueva funcionalidad: ";
        getline(cin, diccionario[indice].funcionalidad);
        cout << "Palabra '" << palabraBuscar << "' actualizada con exito." << endl;
    } else {
        cout << "Error: La palabra '" << palabraBuscar << "' no se encontro." << endl;
    }
}

void eliminarPalabra(vector<PalabraInfo>& diccionario) {
    if (diccionario.empty()) {
        cout << "El diccionario esta vacio. No hay palabras para eliminar." << endl;
        return;
    }
    string palabraBuscar;
    cout << "Ingrese la palabra clave que desea eliminar: ";
    getline(cin, palabraBuscar);

    int indice = buscarIndicePalabra(diccionario, palabraBuscar);

    if (indice != -1) {
        diccionario.erase(diccionario.begin() + indice);
        cout << "Palabra '" << palabraBuscar << "' eliminada con exito." << endl;
    } else {
        cout << "Error: La palabra '" << palabraBuscar << "' no se encontro." << endl;
    }
}


// --- Implementación de Funciones del Traductor ---

void traducirCodigo(const vector<PalabraInfo>& diccionario) {
    if (diccionario.empty()) {
        cout << "El diccionario esta vacio. No se pueden traducir palabras clave." << endl;
        cout << "Use la opcion 1 del menu principal para agregar palabras primero." << endl;
        return;
    }

    cout << "\n--- Traductor de Codigo C++ ---" << endl;
    // ***** Instrucciones actualizadas *****
    cout << "Ingrese/pegue el codigo C++. Escriba ###END### en una nueva linea y presione Enter para finalizar:" << endl;

    string linea;
    vector<string> lineasCodigo;

    // ***** Bucle modificado para leer hasta encontrar "###END###" *****
    while (getline(cin, linea) && linea != "###END###") {
        lineasCodigo.push_back(linea);
    }

    if (lineasCodigo.empty()) {
        cout << "No se ingreso codigo para traducir (o solo se ingreso ###END###)." << endl;
        return;
    }

    cout << "\n--- Codigo Traducido ---" << endl;

    string palabraActual;
    string lineaTraducida;

    for (const string& lineaActual : lineasCodigo) {
        lineaTraducida = "";
        palabraActual = "";

        for (char c : lineaActual) {
            // Si es letra, número o guión bajo, es parte de una palabra
            if (isalnum(c) || c == '_') {
                palabraActual += c;
            } else {
                // Si encontramos un delimitador y teníamos una palabra formada
                if (!palabraActual.empty()) {
                    // Traducir la palabra y añadirla a la línea traducida
                    lineaTraducida += traducirPalabra(palabraActual, diccionario);
                    palabraActual = ""; // Reiniciar la palabra
                }
                // Añadir el carácter delimitador (espacio, ;, {, }, etc.)
                lineaTraducida += c;
            }
        }
        // Si la línea termina con una palabra, procesarla
        if (!palabraActual.empty()) {
            lineaTraducida += traducirPalabra(palabraActual, diccionario);
        }

        cout << lineaTraducida << endl; // Imprimir la línea traducida
    }
    cout << "--- Fin del Codigo Traducido ---" << endl;
}
