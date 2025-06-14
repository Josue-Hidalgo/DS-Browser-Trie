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

#include "Trie.h"
#include "HashTable.h"
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

int main() {
	setlocale(LC_ALL, "spanish");
	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

	UINT cp = GetConsoleOutputCP();
	cout << "La consola está usando la code page: " << cp << std::endl;

	printWelcomeMessage();
	waitAndJump();

	string fileName = inputString("Ingrese el nombre del archivo (con extensión): \n");

	ifstream file;
	ifstream ignoreFile;
	ofstream outputFile;

	Trie* book = new Trie();
	Trie* bookToIgnore = new Trie();

	Dictionary<char, char>* abcLetters = new HashTable<char, char>();
	abecedaryDictionary(abcLetters);

	Dictionary<char, char>* lowerCaseLetters = new HashTable<char, char>();
	lowercaseDictionary(lowerCaseLetters);

	Dictionary<int, string>* lines = new HashTable<int, string>();
	Dictionary<int, string>* notSave = nullptr;

	try {
		openFile(file, fileName);
		openFile(ignoreFile, "Libros/ignorar.txt");

		processLinePerLine(file, book, abcLetters, lowerCaseLetters, lines);
		processLinePerLine(ignoreFile, bookToIgnore, abcLetters, lowerCaseLetters, notSave);

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
			case 1: {
				string prefix = inputString("Ingrese un prefijo a buscar: ");
				List<KVPair<string, int>>* listMatches = book->getPrefixMatches(prefix);
				sort(listMatches);
				cout << "Resultados de la Búsqueda: " << endl;
				for (listMatches->goToStart(); !listMatches->atEnd(); listMatches->next()) {
					KVPair<string, int> pair = listMatches->getElement();
					cout << pair.key << ", " << pair.value << endl;
				}
				delete listMatches;
				break;
			}
			case 2: {
				string word = inputString("Ingrese una palabra a buscar: ");
				if (!book->containsWord(word))
					cout << "La palabra '" << word << "' no fue encontrada." << endl;
				else {
					List<int>* linesList = book->getListLines(word);
					cout << "Resultados de la Búsqueda: " << endl;
					cout << "La palabra '" << word << "' aparece " << linesList->getSize() << " veces y fue encontrada en las siguientes líneas: " << endl;
					for (linesList->goToStart(); !linesList->atEnd(); linesList->next())
						cout << linesList->getElement() << ", ";
					cout << endl;
					delete linesList;
				}
				break;
			}
			case 3: {
				int letterNumber = inputInt("Ingrese la cantidad de letras a buscar: ");
				List<KVPair<string, int>>* listMatches = book->getMatchesLetterNumber(letterNumber);
				sort(listMatches);
				cout << "Resultados de la Búsqueda: " << endl;
				for (listMatches->goToStart(); !listMatches->atEnd(); listMatches->next()) {
					KVPair<string, int> pair = listMatches->getElement();
					cout << pair.key << ", " << pair.value << endl;
				}
				delete listMatches;
				break;
			}
			case 4: {
				int n = inputInt("¿Cuántas palabras quiere ver en el Top?: ");
				List<KVPair<string, int>>* allWords = book->getAllWordsWithFrequency();

				// Heap con KVPair invertido: frecuencia como key, palabra como value
				MaxHeap<KVPair<int, string>> heap(allWords->getSize());

				for (allWords->goToStart(); !allWords->atEnd(); allWords->next()) {
					KVPair<string, int> pair = allWords->getElement();

					if (!bookToIgnore->containsWord(pair.key)) {
						KVPair<int, string> heapPair(pair.value, pair.key); // key=frecuencia, value=palabra
						heap.insert(heapPair);
					}
				}

				cout << "\nTop " << n << " palabras más utilizadas:\n";
				for (int i = 0; i < n && !heap.isEmpty(); ++i) {
					KVPair<int, string> entry = heap.removeFirst();
					cout << i + 1 << ". " << entry.value << " (" << entry.key << " veces)" << endl;
				}

				delete allWords;
				break;
			}
			case 5:
				closeFile(file);
				openFile(file, fileName);
				openFile(ignoreFile, "Libros/ignorar.txt");
				processLinePerLine(file, book, abcLetters, lowerCaseLetters, lines);
				closeFile(file);
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
