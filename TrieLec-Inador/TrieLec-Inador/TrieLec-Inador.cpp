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
 * - Marvin
 * - Sebastián
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
		throw runtime_error("File was imposible to open.");
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
			//cout << "Insertando: " << line << " en la línea: " << lineNumber << endl;
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
	// Letras mayúsculas y minúsculas estándar
	for (char c = 'A'; c <= 'Z'; ++c)
		abc->insert(c, c);
	for (char c = 'a'; c <= 'z'; ++c)
		abc->insert(c, c);

	// Letras acentuadas y especiales en ANSI (Windows-1252)
	// Mayúsculas -> Minúsculas
	abc->insert(char(193), char(225)); // Á -> á
	abc->insert(char(201), char(233)); // É -> é
	abc->insert(char(205), char(237)); // Í -> í
	abc->insert(char(211), char(243)); // Ó -> ó
	abc->insert(char(218), char(250)); // Ú -> ú
	abc->insert(char(220), char(252)); // Ü -> ü
	abc->insert(char(209), char(241)); // Ñ -> ñ

	// Minúsculas acentuadas y especiales (se mapean a sí mismas)
	abc->insert(char(225), char(225)); // á
	abc->insert(char(233), char(233)); // é
	abc->insert(char(237), char(237)); // í
	abc->insert(char(243), char(243)); // ó
	abc->insert(char(250), char(250)); // ú
	abc->insert(char(252), char(252)); // ü
	abc->insert(char(241), char(241)); // ñ
}
static void lowercaseDictionary(Dictionary<char, char>*& abc) {
	// Letras estándar
	for (char c = 'A'; c <= 'Z'; ++c)
		abc->insert(c, c + 32);

	// Mayúsculas acentuadas y especiales -> minúsculas
	abc->insert(char(193), char(225)); // Á -> á
	abc->insert(char(201), char(233)); // É -> é
	abc->insert(char(205), char(237)); // Í -> í
	abc->insert(char(211), char(243)); // Ó -> ó
	abc->insert(char(218), char(250)); // Ú -> ú
	abc->insert(char(220), char(252)); // Ü -> ü
	abc->insert(char(209), char(241)); // Ñ -> ñ

	// Minúsculas acentuadas y especiales (se mapean a sí mismas)
	abc->insert(char(225), char(225)); // á
	abc->insert(char(233), char(233)); // é
	abc->insert(char(237), char(237)); // í
	abc->insert(char(243), char(243)); // ó
	abc->insert(char(250), char(250)); // ú
	abc->insert(char(252), char(252)); // ü
	abc->insert(char(241), char(241)); // ñ
}

// Sort Algorithms
static void sort(List<KVPair<string, int>>*& list) {

}

int main() {
	setlocale(LC_ALL, "spanish");
	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

	//UINT cp = GetConsoleOutputCP();
	//cout << "La consola está usando la code page: " << cp << std::endl;

	// 1. Al iniciar el programa se imprime un mensaje de bienvenida muy corto que explica el propósito del programa.
	printWelcomeMessage();
	waitAndJump();

	// 2.	Se solicita al usuario el nombre de un archivo de texto a analizar. No debe agregarse ningún carácter o
	//		extensión al nombre de usuario proporcionado por el usuario.El usuario debe escribir la extensión del
	//		archivo.
	string fileName = inputString("Ingrese el nombre del archivo (con extensión): \n");

	// 3. Si el archivo no existe o no es posible abrirlo, se indica un mensaje de error y el programa termina.
	ifstream file; // Creamos objeto archivo de tipo ifstream
	ifstream ignoreFile; // Creamos objeto archivo de tipo ifstream para el ignore.txt

	ofstream outputFile; // Objeto archivo que usaremos en caso de que se requiera un output por archivo.

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

		// 4. Si no, se abre el archivo y es procesado línea por línea. La línea original leída debe almacenarse en una
		// estructura de rápido acceso que permita localizar el texto de la línea en caso de que se requiera
		// imprimirla.
		processLinePerLine(file,		book,			abcLetters, lowerCaseLetters, lines);
		processLinePerLine(ignoreFile,	bookToIgnore,	abcLetters, lowerCaseLetters, notSave);
		
		int option = -1;
		while (option != 0) {

			option = inputInt("Seleccione una opción:\n"
				"1. Consultar por prefijo.\n"
				"2. Consultar por palabra.\n"
				"3. Consultar por cantidad de letras.\n"
				"4. Ordenar palabras por frecuencia de uso.\n"
				"5. Cargar Archivo.\n"
				"0. Salir del Programa.\n"
				"Ingrese su opción: ");
			printNewPage();

			switch (option) {
			case 0:{
				cout << "Muchas gracias por usar nuestros servicios." << endl;
				break;
			}
			case 1:{
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
			case 2:{

				string word = inputString("Ingrese una palabra a buscar: ");
				if (!book->containsWord(word))
					cout << "La palabra '" << word << "' no fue encontrada." << endl;
				else {
					List<int>* linesList = book->getListLines(word);

					cout << "Resultados de la Búsqueda: " << endl;
					cout << "La palabra '" << word << "' sale " << linesList->getSize() << " veces y fue encontrada en las siguientes líneas: " << endl;
					
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
				break;
			}
			case 5:{
				closeFile(file);
				openFile(file, fileName);
				openFile(ignoreFile, "Libros/ignorar.txt");

				processLinePerLine(file, book, abcLetters, lowerCaseLetters, lines);

				closeFile(file);

				break;
			}
			default:
				cout << "Opción no válida. Por favor, ingrese un número entre 1 y 5." << endl;
				break;
			}
			waitAndJump();
		}
	} catch (const runtime_error& e){
		cout << "ERROR: Runtime Error." << endl;
		cout << "Detail: " << e.what() << endl;
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