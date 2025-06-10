/*
 * Nombre del proyecto: TrieLec-Inador
 * 
 * Descripción General: 
 * 
 * El actual proyecto consiste en la implementación de un árbol trie con palabras obtenidas de un archivo de texto
 * en formato ANSI con el fin de consultar y obtener información relevante sobre el archivo analizado
 * 
 * Equipo de Trabajo:
 * - Josue Santiago Hidalgo Sandoval
 * 
 */

 // Importar las librerías necesarias
#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <Windows.h>

// Clases 

// Importar con nombres conocidos
using std::cout;
using std::cin;
using std::stoi;
using std::endl;
using std::getline;
using std::string;
using std::ifstream;
using std::invalid_argument;
using std::ios_base;

/*AUXILIARIES*/
static void printWaitKey() {
	cout << "Presione ENTER para continuar..." << endl;
	string dummy;
	getline(cin, dummy);
}
static void printNewPage() {
	for (int i = 0; i < 50; i++)
		cout << endl;
}
static void waitAndJump() {
	printWaitKey();
	printNewPage();
}

/* INPUT FUNCTIONS */
static int inputInt(const string& message) {
	while (true) {
		try {
			string input;
			cout << message;
			getline(cin, input);
			return stoi(input); // Convierte a entero, lanza excepciones si falla
		}
		catch (const std::invalid_argument&) {
			cout << "Error: Entrada no válida. Por favor, ingrese un número entero." << endl;
		}
		catch (const std::out_of_range&) {
			cout << "Error: Número fuera de rango. Por favor, intente de nuevo." << endl;
		}
	}
}
static string inputString(const string& message) {
	while (true) {
		try {
			string input;
			cout << message;
			getline(cin, input);
			if (input.empty())
				throw invalid_argument("La entrada no puede estar vacía.");
			return input;
		}
		catch (const  invalid_argument& e) {
			cout << "Error: " << e.what() << " Por favor, intente de nuevo." << endl;
		}
	}
}

/*PRINTS*/
static void printWelcomeMessage() {
	cout << "Bienvenido al programa TrieLec-Inador" << endl;
	
	cout << "___________      .__       .____                           .___               __                " << endl;
	cout << "\\__    ___/______|__| ____ |    |    ____   ____           |   | ____ _____ _/  |_  ___________ " << endl;
	cout << "  |    |  \\_  __ \\  |/ __ \\|    |  _/ __ \\_/ ___\\   ______ |   |/    \\ \\__ \\\\   __\\/  _ \\_  __ \\" << endl;
	cout << "  |    |   |  | \\/  \\  ___/|    |__\\  ___/\\  \\___  /_____/ |   |   |  \\/ __ \\|  | (  <_> )  | \\/" << endl;
	cout << "  |____|   |__|  |__|\\___  >_______ \\___  >\\___  >         |___|___|  (____  /__|  \\____/|__|   " << endl;
	cout << "                         \\/        \\/   \\/     \\/                   \\/     \\/                   " << endl;

	cout << "El actual proyecto consiste en la implementación de un árbol trie con palabras obtenidas de un archivo de texto" << endl;
	cout << "en formato ANSI con el fin de consultar y obtener información relevante sobre el archivo analizado como : " << endl;
	cout << "1. Lista de palabras coincidentes con respecto a una palabra o prefijo." << endl;
	cout << "2. Para cada palabra coincidente se muestra una lista con las líneas de texto donde aparece la palabra." << endl;
	cout << "3. Buscar palabras por cantidad de letras." << endl;
	cout << "4. Ordenar las palabras por frecuencia de uso." << endl;
	cout << "Por favor, siga las instrucciones para cargar el archivo y realizar consultas." << endl;
}

/*FILE MANAGEMENT*/
static void openFile(ifstream& file, string name) {
	file.exceptions(ifstream::badbit); // Habilitamos exceptions
	file.open(name); // Abrimos el archivo que pedimos al usuario
}
static void closeFile(ifstream& file) {
	file.close(); // Cerramos el archivo
}
static void processLinePerLine(ifstream& file) {
	string line;
	while (getline(file, line)) {
		// PENDIENTE ALMACENAR LINEA EN UNA ESTRUCTURA DE RAPIDO ACCESO ()
	}
}

int main() {
	// Imprimir caracteres con tildes y más 
	setlocale(LC_ALL, "spanish");
	// Leer/Escribir caracteres con tildes y más 
	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

	// 1. Al iniciar el programa se imprime un mensaje de bienvenida muy corto que explica el propósito del programa.
	printWelcomeMessage();
	waitAndJump();

	// 2.	Se solicita al usuario el nombre de un archivo de texto a analizar. No debe agregarse ningún carácter o
	//		extensión al nombre de usuario proporcionado por el usuario.El usuario debe escribir la extensión del
	//		archivo.
	string fileName = inputString("Ingrese el nombre del archivo (con extensión): \n");

	// 3. Si el archivo no existe o no es posible abrirlo, se indica un mensaje de error y el programa termina.
	ifstream file; // Creamos objeto archivo de tipo ifstream
	try {
		openFile(file, fileName);

		// 4. Si no, se abre el archivo y es procesado línea por línea. La línea original leída debe almacenarse en una
		// estructura de rápido acceso que permita localizar el texto de la línea en caso de que se requiera
		// imprimirla.
		processLinePerLine(file);
		
		closeFile(file);
	} catch (const ios_base::failure& e){
		cout << "ERROR: NO se pudo abrir el archivo." << endl;
		cout << "Detalle: " << e.what() << endl;
	}
	return 0;
}