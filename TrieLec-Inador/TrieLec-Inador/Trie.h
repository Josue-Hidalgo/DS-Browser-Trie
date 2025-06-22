/*
 * Nombre Archivo: Trie.h
 * 
 * Descripción General: 
 * 
 * El Trie es una estructura de datos que permite almacenar
 * palabras de manera eficiente, facilitando la búsqueda
 * y que además permite realizar operaciones como
 * busqueda por prefijo y palabra.
 * 
 * Autor: Mauricio Avilés // Josué Hidalgo y Sebastián Masís
 * 
 */

#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include "TrieNode.h"
#include "SplayTreeDictionary.h"

using std::string;
using std::cout;
using std::endl;
using std::runtime_error;

class Trie {
private:
	TrieNode* root;

	TrieNode* findNode(string word) {
		TrieNode* current = root;
		for (char c : word) {
			if (!current->containsChild(c))
				return nullptr;
			current = current->getChild(c);
		}
		return current;
	}

	void clearAux(TrieNode* current) {
		List<char>* children = current->getChildren();
		for (children->goToStart(); !children->atEnd(); children->next()) {
			TrieNode* child = current->getChild(children->getElement());
			clearAux(child);
		}
		delete children;
	}
	
	void getMatchesAux(TrieNode* current, string prefix, List<string>* matches) {
		if (current->isFinal)
			matches->append(prefix);
		List<char>* children = current->getChildren();
		for (children->goToStart(); !children->atEnd(); children->next()) {
			char c = children->getElement();
			TrieNode* child = current->getChild(c);
			getMatchesAux(child, prefix + c, matches);
		}
		delete children;
	}

	// Metodos Privados para Proyecto
	void getPrefixMatchesAux(TrieNode* current, string prefix, List<KVPair<string, List<int>*>>* matches) {
		if (current->isFinal) {
			KVPair<string, List<int>*> pair(prefix, current->lines);  // incluir lista de líneas
			matches->append(pair);
		}
		List<char>* children = current->getChildren();
		for (children->goToStart(); !children->atEnd(); children->next()) {
			char c = children->getElement();
			TrieNode* child = current->getChild(c);
			getPrefixMatchesAux(child, prefix + c, matches);
		}
		delete children;
	}

	void getMatchesLetterNumberAux(TrieNode* current, const int letterNumber, string prefix, List<KVPair<string, List<int>*>>* matches) {
		if (current->isFinal && current->letterCount == letterNumber) {
			KVPair<string, List<int>*> pair(prefix, current->lines);
			matches->append(pair);
		}
		List<char>* children = current->getChildren();
		for (children->goToStart(); !children->atEnd(); children->next()) {
			char c = children->getElement();
			TrieNode* child = current->getChild(c);
			getMatchesLetterNumberAux(child, letterNumber, prefix + c, matches);
		}
		delete children;
	}

	void getAllWordsWithFrequencyAux(TrieNode* current, string prefix, List<KVPair<string, int>>* result) {
		if (current->isFinal) {
			int count = current->lines->getSize();
			KVPair<string, int> pair(prefix, count);
			result->append(pair);
		}

		List<char>* children = current->getChildren();
		for (children->goToStart(); !children->atEnd(); children->next()) {
			char c = children->getElement();
			TrieNode* child = current->getChild(c);
			getAllWordsWithFrequencyAux(child, prefix + c, result);
		}
		delete children;
	}

	bool contains(List<int>* list, int value) {
		for (list->goToStart(); !list->atEnd(); list->next()) {
			if (list->getElement() == value)
				return true;
		}
		return false;
	}


public:
	Trie() {
		root = new TrieNode();
	}

	~Trie() {
		clear();
		delete root;
	}

	void insert(string word, int lineNumber) {
		TrieNode* current;
		int depth = 0;

		if (containsWord(word)) {
			current = findNode(word);
			if (!contains(current->lines, lineNumber)) // Verifica si la línea ya está registrada
				current->lines->append(lineNumber);
			return;
		}

		current = root;
		for (char c : word) {
			depth++;
			current->prefixCount++;
			if (!current->containsChild(c))
				current->addChild(c);
			current = current->getChild(c);
			current->letterCount = depth;
			if (!contains(current->lines, lineNumber))
				current->lines->append(lineNumber);
		}
		current->prefixCount++;
		current->isFinal = true;
	}

	bool containsWord(string word) {
		TrieNode* current = findNode(word);
		if (current == nullptr) return false;
		return current->isFinal;
	}

	bool containsPrefix(string prefix) {
		TrieNode* current = findNode(prefix);
		if (current == nullptr) return false;
		return true;
	}

	int prefixCount(string prefix) {
		TrieNode* current = findNode(prefix);
		if (current == nullptr) return 0;
		return current->prefixCount;
	}

	void remove(string word) {
		if (!containsWord(word))
			throw runtime_error("Word not found.");
		TrieNode* current = root;
		for (char c : word) {
			current->prefixCount--;
			TrieNode* child = current->getChild(c);
			if (child->prefixCount == 1)
				current->removeChild(c);
			if (current->prefixCount == 0)
				delete current;
			current = child;
		}
		current->prefixCount--;
		current->isFinal = false;
		if (current->prefixCount == 0)
			delete current;
	}

	void clear() {
		clearAux(root);
		root = new TrieNode();
	}
	
	List<string>* getMatches(string prefix) {
		List<string>* matches = new DLinkedList<string>();

		TrieNode* current = findNode(prefix);
		if (current != nullptr)
			getMatchesAux(current, prefix, matches);
		return matches;
	}

	void print() {
		List<string>* words = getMatches("");
		words->print();
		delete words;
	}

	// Nuevos Métodos para Proyecto
	void printWordsWithLines() {
		List<string>* words = getMatches("");
		for (words->goToStart(); !words->atEnd(); words->next()) {
			string word = words->getElement();
			TrieNode* node = findNode(word);
			cout << word << ": ";
			// Imprimir las líneas
			for (node->lines->goToStart(); !node->lines->atEnd(); node->lines->next()) {
				cout << node->lines->getElement();
				if (!node->lines->atEnd()) // Si no es el último elemento
					cout << ", ";
			}
			cout << endl;
		}
		delete words;
	}

	List<KVPair<string, List<int>*>>* getPrefixMatches(string prefix) {
		List<KVPair<string, List<int>*>>* matches = new DLinkedList<KVPair<string, List<int>*>>();
		TrieNode* current = findNode(prefix);
		if (current != nullptr)
			getPrefixMatchesAux(current, prefix, matches);
		return matches;
	}

	List<int>* getListLines(string word) { // b. Cantidad de veces (getSize) y número de lineas 
		if (!containsWord(word))
			throw runtime_error("Word was not found.");
		TrieNode* current = findNode(word);
		return current->lines;
	}

	List<KVPair<string, List<int>*>>* getMatchesLetterNumber(int letterNumber) {
		List<KVPair<string, List<int>*>>* matches = new DLinkedList<KVPair<string, List<int>*>>();
		getMatchesLetterNumberAux(root, letterNumber, "", matches);
		return matches;
	}

	List<KVPair<string, int>>* getAllWordsWithFrequency() { // d. Obtener todas las palabras con su frecuencia 
		List<KVPair<string, int>>* result = new DLinkedList<KVPair<string, int>>();
		getAllWordsWithFrequencyAux(root, "", result);
		return result;
	}
};

