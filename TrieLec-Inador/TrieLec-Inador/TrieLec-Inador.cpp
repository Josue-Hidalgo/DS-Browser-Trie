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
 * - Sebastián Masís Solano
 * - Marvin 
 */

 // Importar las librerías necesarias
#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include <chrono>

#include "Trie.h"
#include "HashTable.h"
#include "SplayTreeDictionary.h"
#include "MaxHeap.h"

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
using std::runtime_error;
using std::ofstream;
using std::ios_base;


// Test - DSs
static std::chrono::high_resolution_clock::time_point obtenerTiempoActual() {
	return std::chrono::high_resolution_clock::now();
}
static void imprimirDiferenciaTiempo(const std::chrono::high_resolution_clock::time_point& inicio,
	const std::chrono::high_resolution_clock::time_point& fin) {
	auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();
	std::cout << "Tiempo transcurrido: " << duracion << " ms" << std::endl;
}

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
	file.open(name); // Abrimos el archivo que pedimos al usuario
	if (!file.is_open())
		throw runtime_error("No fue posible abrir el archivo.");
}
static void closeFile(ifstream& file) {
	file.close(); // Cerramos el archivo
}
static void processLinePerLine(ifstream& file, Trie*& book, Dictionary<char, char>* validDict, Dictionary<char, char>* toLowerDict, Dictionary<int, string>*& linesList) {
	string line;
	string word;
	int lineNumber = 0;

	while (getline(file, line)) {
		if (!line.empty() && linesList != nullptr) {
			linesList->insert(lineNumber, line);
		}
		bool inword = false;
		for (char c : line) {
			if (validDict->contains(c) || toLowerDict->contains(c)) {
				inword = true;
				if (toLowerDict->contains(c))
					c = toLowerDict->getValue(c);
				word += c;
			}
			else {
				if (inword && !word.empty()) {
					book->insert(word, lineNumber);
					word.clear();
					inword = false;
				}
			}
		}
		if (inword && !word.empty()) {
			book->insert(word, lineNumber);
			word.clear();
		}
		lineNumber++;
	}
}

enum class PrintMode {
	Consola,
	Archivo
};
static string getCurrentDateTimeFileName() {
	time_t now = time(nullptr); // Tiempo formato UNIX
	tm localTime;
	localtime_s(&localTime, &now); // Convierte a tm 
	std::ostringstream oss; // stream para construir nombre
	oss << std::put_time(&localTime, "%Y%m%d_%H%M%S")
		<< ".txt";
	return oss.str();
}
static void printer(const string& texto, PrintMode modo) {
	if (modo == PrintMode::Consola) {
		cout << texto;
	}
	else {
		string fileName = "Archivos/" + getCurrentDateTimeFileName();
		ofstream out(fileName);
		if (!out.is_open()) {
			cout << "No se pudo crear el archivo de salida: " << fileName << endl;
			return;
		}
		out << texto;
		out.close();
		cout << "Resultado guardado en el directorio: " << fileName << endl;
	}
}

// Initialize variables
static void abecedaryDictionary(Dictionary<char, char>*& abc) {
	for (char c = 'A'; c <= 'Z'; ++c)
		abc->insert(c, c);
	for (char c = 'a'; c <= 'z'; ++c)
		abc->insert(c, c);

	// Letras acentuadas y especiales en ANSI (Windows-1252)
	abc->insert(char(193), char(225)); // Á -> á
	abc->insert(char(201), char(233)); // É -> é
	abc->insert(char(205), char(237)); // Í -> í
	abc->insert(char(211), char(243)); // Ó -> ó
	abc->insert(char(218), char(250)); // Ú -> ú
	abc->insert(char(220), char(252)); // Ü -> ü
	abc->insert(char(209), char(241)); // Ñ -> ñ

	abc->insert(char(225), char(225)); // á
	abc->insert(char(233), char(233)); // é
	abc->insert(char(237), char(237)); // í
	abc->insert(char(243), char(243)); // ó
	abc->insert(char(250), char(250)); // ú
	abc->insert(char(252), char(252)); // ü
	abc->insert(char(241), char(241)); // ñ
}
static void lowercaseDictionary(Dictionary<char, char>*& abc) {
	for (char c = 'A'; c <= 'Z'; ++c)
		abc->insert(c, c + 32);

	abc->insert(char(193), char(225)); // Á -> á
	abc->insert(char(201), char(233)); // É -> é
	abc->insert(char(205), char(237)); // Í -> í
	abc->insert(char(211), char(243)); // Ó -> ó
	abc->insert(char(218), char(250)); // Ú -> ú
	abc->insert(char(220), char(252)); // Ü -> ü
	abc->insert(char(209), char(241)); // Ñ -> ñ

	abc->insert(char(225), char(225)); // á
	abc->insert(char(233), char(233)); // é
	abc->insert(char(237), char(237)); // í
	abc->insert(char(243), char(243)); // ó
	abc->insert(char(250), char(250)); // ú
	abc->insert(char(252), char(252)); // ü
	abc->insert(char(241), char(241)); // ñ
}

// Format
static string lowercase(Dictionary<char, char>*& lowercaseDictionary, const string& word) {
	string result;
	for (char c : word) {
		if (lowercaseDictionary->contains(c))
			result += lowercaseDictionary->getValue(c);
		else
			result += c;
	}
	return result;
}

// Sort Algorithms
static string normalize(const string& word) {
	string result;
	for (char c : word) {
		switch (c) {
		case 'á': case 'Á': c = 'a'; break;
		case 'é': case 'É': c = 'e'; break;
		case 'í': case 'Í': c = 'i'; break;
		case 'ó': case 'Ó': c = 'o'; break;
		case 'ú': case 'Ú': c = 'u'; break;
		case 'ü': case 'Ü': c = 'u'; break;
		case 'ñ': case 'Ñ': c = 'n'; break;
		default:
			if (c >= 'A' && c <= 'Z')
				c = c + 32; // Convertir a minúscula
		}
		result += c;
	}
	return result;
}
static void sort(List<KVPair<string, int>>*& list) {
	int n = list->getSize();
	if (n <= 1) return;

	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; i++) {
			list->goToPos(i);
			KVPair<string, int> temp = list->getElement();
			string tempKey = normalize(temp.key);

			int j = i;
			while (j >= gap) {
				list->goToPos(j - gap);
				KVPair<string, int> prev = list->getElement();
				string prevKey = normalize(prev.key);

				if (prevKey > tempKey) {
					list->goToPos(j);
					list->remove();          // Eliminamos el actual
					list->insert(prev);      // Insertamos el anterior
				}
				else {
					break;
				}

				j -= gap;
			}
			list->goToPos(j);
			list->remove();              // Eliminamos lo que quedó
			list->insert(temp);         // Insertamos el original
		}
	}
}

// Auxiliary Functions
static PrintMode seleccionarModoImpresion() {
	int mode = 0;
	while (mode != 1 && mode != 2)
		mode = inputInt("¿Cómo desea imprimir el resultado?\n1. Consola\n2. Archivo\nOpción: ");
	return (mode == 2) ? PrintMode::Archivo : PrintMode::Consola;
}
static bool deseaImpresionCompleta() {
	int fullMode = 0;
	while (fullMode != 1 && fullMode != 2)
		fullMode = inputInt("¿Desea impresión completa de líneas?\n1. Sí (número y texto)\n2. No (solo números de línea)\nOpción: ");
	return (fullMode == 1);
}
static void consultarPorPrefijo(Trie* book, Dictionary<char, char>* lowerCaseLetters, PrintMode printMode) {
	string prefix = lowercase(lowerCaseLetters, inputString("Ingrese un prefijo a buscar: "));
	
	std::chrono::high_resolution_clock::time_point inicio = obtenerTiempoActual();

	List<KVPair<string, int>>* listMatches = book->getPrefixMatches(prefix);
	if (listMatches == nullptr || listMatches->getSize() == 0) {
		cout << "No hay resultados con '" << prefix << "' número de letras." << endl;
		return;
	}
	sort(listMatches);
	string output = "\nResultados de la Búsqueda: \n";
	for (listMatches->goToStart(); !listMatches->atEnd(); listMatches->next()) {
		auto pair = listMatches->getElement();
		output += "Palabra: " + pair.key + ", Cantidad: " + std::to_string(pair.value) + "\n";
	}
	printer(output, printMode);
	delete listMatches;

	imprimirDiferenciaTiempo(inicio, obtenerTiempoActual());
}
static void consultarPorPalabra(Trie* book, Dictionary<char, char>* lowerCaseLetters, Dictionary<int, string>* lines, PrintMode printMode) {
	bool impresionCompleta = deseaImpresionCompleta();
	string word = lowercase(lowerCaseLetters, inputString("Ingrese una palabra a buscar: "));

	std::chrono::high_resolution_clock::time_point inicio = obtenerTiempoActual();

	if (!book->containsWord(word)) {
		cout << "La palabra '" << word << "' no fue encontrada en el texto." << endl;
		return;
	}
	List<int>* linesList = book->getListLines(word);
	string output = "\nResultados de la Búsqueda: \n";
	output += "La palabra '" + word + "' aparece " + std::to_string(linesList->getSize()) + " veces y fue encontrada en las siguientes líneas:\n";
	for (linesList->goToStart(); !linesList->atEnd(); linesList->next()) {
		int numLinea = linesList->getElement();
		if (impresionCompleta && lines->contains(numLinea)) {
			output += std::to_string(numLinea) + ": " + lines->getValue(numLinea) + "\n";
		}
		else {
			output += std::to_string(numLinea) + ", ";
		}
	}
	output += "\n";
	printer(output, printMode);

	imprimirDiferenciaTiempo(inicio, obtenerTiempoActual());
}
static void consultarPorCantidadLetras(Trie* book, PrintMode printMode) {
	int letterNumber = inputInt("Ingrese la cantidad de letras a buscar: ");

	std::chrono::high_resolution_clock::time_point inicio = obtenerTiempoActual();

	List<KVPair<string, int>>* listMatches = book->getMatchesLetterNumber(letterNumber);
	if (listMatches == nullptr || listMatches->getSize() == 0) {
		cout << "No hay resultados con '" << letterNumber << "' número de letras." << endl;
		return;
	}
	sort(listMatches);
	string output = "\nResultados de la Búsqueda: \n";
	for (listMatches->goToStart(); !listMatches->atEnd(); listMatches->next()) {
		auto pair = listMatches->getElement();
		output += "Palabra: " + pair.key + ", " + std::to_string(pair.value) + "\n";
	}
	printer(output, printMode);
	delete listMatches;

	imprimirDiferenciaTiempo(inicio, obtenerTiempoActual());
}
static void mostrarTopPalabras(Trie* book, Trie* bookToIgnore, PrintMode printMode) {
	int topNumber = inputInt("¿Cuántas palabras quiere ver en el Top?: ");
	
	std::chrono::high_resolution_clock::time_point inicio = obtenerTiempoActual();
	
	List<KVPair<string, int>>* allWords = book->getAllWordsWithFrequency();
	MaxHeap<KVPair<int, string>> heap(allWords->getSize());
	for (allWords->goToStart(); !allWords->atEnd(); allWords->next()) {
		auto pair = allWords->getElement();
		if (!bookToIgnore->containsWord(pair.key)) {
			heap.insert(KVPair<int, string>(pair.value, pair.key));
		}
	}
	string output = "\nTop " + std::to_string(topNumber) + " palabras más utilizadas:\n";
	for (int i = 0; i < topNumber && !heap.isEmpty(); ++i) {
		auto entry = heap.removeFirst();
		output += std::to_string(i + 1) + ". " + entry.value + " (" + std::to_string(entry.key) + " veces)\n";
	}
	printer(output, printMode);
	delete allWords;

	imprimirDiferenciaTiempo(inicio, obtenerTiempoActual());
}
static void cargarArchivo(Trie* book, Dictionary<int, string>* lines, ifstream& file, string& fileName,
	Dictionary<char, char>* abcLetters, Dictionary<char, char>* lowerCaseLetters) {

	std::chrono::high_resolution_clock::time_point inicio = obtenerTiempoActual();
	book->clear();
	lines->clear();
	if (file.is_open())
		closeFile(file);
	while (!file.is_open()) {
		fileName = inputString("Ingrese el nombre del nuevo archivo (con extensión): \n");
		file.open(fileName);
	}
	processLinePerLine(file, book, abcLetters, lowerCaseLetters, lines);

	imprimirDiferenciaTiempo(inicio, obtenerTiempoActual());
}

int main() {
	setlocale(LC_ALL, "spanish");
	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

	// UINT cp = GetConsoleOutputCP();
	// cout << "La consola está usando la code page: " << cp << std::endl;

	printWelcomeMessage();
	waitAndJump();

	string fileName = inputString("Ingrese el nombre del archivo (con extensión): \n");

	ifstream file;
	ifstream ignoreFile;
	ofstream outputFile;

	Trie* book = new Trie();
	Trie* bookToIgnore = new Trie();

	Dictionary<char, char>* abcLetters = new SplayTreeDictionary<char, char>();
	abecedaryDictionary(abcLetters);
	Dictionary<char, char>* lowerCaseLetters = new SplayTreeDictionary<char, char>();
	lowercaseDictionary(lowerCaseLetters);

	Dictionary<int, string>* lines = new SplayTreeDictionary<int, string>();
	Dictionary<int, string>* notSave = nullptr;

	try {
		std::chrono::high_resolution_clock::time_point inicio, fin;
		openFile(file, fileName);
		openFile(ignoreFile, "Libros/ignorar.txt");
		inicio = obtenerTiempoActual();
		processLinePerLine(file, book, abcLetters, lowerCaseLetters, lines);

		processLinePerLine(ignoreFile, bookToIgnore, abcLetters, lowerCaseLetters, notSave);
		fin = obtenerTiempoActual();
		imprimirDiferenciaTiempo(inicio, fin);


		int option = -1;
		while (option != 0) {

			option = inputInt("Seleccione una opción:\n"
				"1. Consultar por prefijo.\n"
				"2. Consultar por palabra.\n"
				"3. Consultar por cantidad de letras.\n"
				"4. Obtener Top Palabras Más Frecuentes.\n"
				"5. Cargar Archivo.\n"
				"0. Salir del Programa.\n"
				"Ingrese su opción: ");
			printNewPage();

			switch (option) {
			case 0:
				cout << "Muchas gracias por usar nuestros servicios." << endl;
				break;
			case 1:
				consultarPorPrefijo(book, lowerCaseLetters, seleccionarModoImpresion());
				break;
			case 2:
				consultarPorPalabra(book, lowerCaseLetters, lines, seleccionarModoImpresion());
				break;
			case 3:
				consultarPorCantidadLetras(book, seleccionarModoImpresion());
				break;
			case 4:
				mostrarTopPalabras(book, bookToIgnore, seleccionarModoImpresion());
				break;
			case 5:
				cargarArchivo(book, lines, file, fileName, abcLetters, lowerCaseLetters);
				break;
			default:
				cout << "Opción no válida. Por favor, ingrese un número entre 1 y 5." << endl;
				break;
			}

			waitAndJump();
		}
	}
	catch (const runtime_error& e) {
		cout << "ERROR: Runtime Error." << endl;
		cout << "Detalle: " << e.what() << endl;
	}

	closeFile(file);
	closeFile(ignoreFile);

	delete book;
	delete lines;
	delete notSave;
	delete abcLetters;
	delete bookToIgnore;
	delete lowerCaseLetters;

	return 0;
}
