/*
 * Nombre del proyecto: TrieLec-Inador
 * 
 * Descripci�n General: 
 * 
 * El actual proyecto consiste en la implementaci�n de un �rbol trie con palabras obtenidas de un archivo de texto
 * en formato ANSI con el fin de consultar y obtener informaci�n relevante sobre el archivo analizado
 * 
 * Equipo de Trabajo:
 * - Josue Santiago Hidalgo Sandoval
 * - Marvin
 * - Sebasti�n
 */

 // Importar las librer�as necesarias
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
			cout << "Error: Entrada no v�lida. Por favor, ingrese un n�mero entero." << endl;
		}
		catch (const std::out_of_range&) {
			cout << "Error: N�mero fuera de rango. Por favor, intente de nuevo." << endl;
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
				throw invalid_argument("La entrada no puede estar vac�a.");
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

	cout << "El actual proyecto consiste en la implementaci�n de un �rbol trie con palabras obtenidas de un archivo de texto" << endl;
	cout << "en formato ANSI con el fin de consultar y obtener informaci�n relevante sobre el archivo analizado como : " << endl;
	cout << "1. Lista de palabras coincidentes con respecto a una palabra o prefijo." << endl;
	cout << "2. Para cada palabra coincidente se muestra una lista con las l�neas de texto donde aparece la palabra." << endl;
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
static void processLinePerLine(ifstream& file, Trie*& book, Dictionary<char, char>* validDict, Dictionary<char, char>* toLowerDict) {
	string line;
	string word;
	int lineNumber = 0;

	while (getline(file, line)) {
		bool inWord = false;
		for (char c : line) {
			if (validDict->contains(c) || toLowerDict->contains(c)) {
				inWord = true;
				if (toLowerDict->contains(c))
					c = toLowerDict->getValue(c);
				word += c;
			}
			else {
				if (inWord && !word.empty()) {
					book->insert(word, lineNumber);
					word.clear();
					inWord = false;
				}
			}
		}
		if (inWord && !word.empty()) {
			book->insert(word, lineNumber);
			word.clear();
		}
		lineNumber++;
	}
}

// Initialize variables
static void abecedaryDictionary(Dictionary<char, char>*& abc) {
	// Letras may�sculas y min�sculas est�ndar
	for (char c = 'A'; c <= 'Z'; ++c)
		abc->insert(c, c);
	for (char c = 'a'; c <= 'z'; ++c)
		abc->insert(c, c);

	// Letras acentuadas y especiales en ANSI (Windows-1252)
	// May�sculas -> Min�sculas
	abc->insert(char(193), char(225)); // � -> �
	abc->insert(char(201), char(233)); // � -> �
	abc->insert(char(205), char(237)); // � -> �
	abc->insert(char(211), char(243)); // � -> �
	abc->insert(char(218), char(250)); // � -> �
	abc->insert(char(220), char(252)); // � -> �
	abc->insert(char(209), char(241)); // � -> �

	// Min�sculas acentuadas y especiales (se mapean a s� mismas)
	abc->insert(char(225), char(225)); // �
	abc->insert(char(233), char(233)); // �
	abc->insert(char(237), char(237)); // �
	abc->insert(char(243), char(243)); // �
	abc->insert(char(250), char(250)); // �
	abc->insert(char(252), char(252)); // �
	abc->insert(char(241), char(241)); // �
}

static void lowercaseDictionary(Dictionary<char, char>*& abc) {
	// Letras est�ndar
	for (char c = 'A'; c <= 'Z'; ++c)
		abc->insert(c, c + 32);

	// May�sculas acentuadas y especiales -> min�sculas
	abc->insert(char(193), char(225)); // � -> �
	abc->insert(char(201), char(233)); // � -> �
	abc->insert(char(205), char(237)); // � -> �
	abc->insert(char(211), char(243)); // � -> �
	abc->insert(char(218), char(250)); // � -> �
	abc->insert(char(220), char(252)); // � -> �
	abc->insert(char(209), char(241)); // � -> �

	// Min�sculas acentuadas y especiales (se mapean a s� mismas)
	abc->insert(char(225), char(225)); // �
	abc->insert(char(233), char(233)); // �
	abc->insert(char(237), char(237)); // �
	abc->insert(char(243), char(243)); // �
	abc->insert(char(250), char(250)); // �
	abc->insert(char(252), char(252)); // �
	abc->insert(char(241), char(241)); // �
}


int main() {

	UINT cp = GetConsoleOutputCP();
	cout << "La consola est� usando la code page: " << cp << std::endl;

	setlocale(LC_ALL, "spanish");
	SetConsoleCP(1252);
	SetConsoleOutputCP(1252);
	

	// 1. Al iniciar el programa se imprime un mensaje de bienvenida muy corto que explica el prop�sito del programa.
	printWelcomeMessage();
	waitAndJump();

	// 2.	Se solicita al usuario el nombre de un archivo de texto a analizar. No debe agregarse ning�n car�cter o
	//		extensi�n al nombre de usuario proporcionado por el usuario.El usuario debe escribir la extensi�n del
	//		archivo.
	string fileName = inputString("Ingrese el nombre del archivo (con extensi�n): \n");

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
	
	try {
		openFile(file, fileName);
		openFile(ignoreFile, "Libros/ignorar.txt");

		// 4. Si no, se abre el archivo y es procesado l�nea por l�nea. La l�nea original le�da debe almacenarse en una
		// estructura de r�pido acceso que permita localizar el texto de la l�nea en caso de que se requiera
		// imprimirla.
		processLinePerLine(file,		book,			abcLetters, lowerCaseLetters);
		processLinePerLine(ignoreFile,	bookToIgnore,	abcLetters, lowerCaseLetters);

		//book->print();

		closeFile(file);
		closeFile(ignoreFile);
	} catch (const runtime_error& e){
		cout << "ERROR: Runtime Error." << endl;
		cout << "Detail: " << e.what() << endl;
	}
	
	delete book;
	delete abcLetters;
	delete bookToIgnore;
	delete lowerCaseLetters;
	
	return 0;
}