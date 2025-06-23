/*
 * Nombre del Archivo: TrieNode.h
 * 
 * Descripción General:
 * 
 * El TrieNode es una clase que representa un nodo en la estructura
 * de datos Trie. Cada nodo contiene información sobre si es un
 * nodo final de una palabra, el conteo de prefijos que pasan.
 * 
 * Autor: Mauricio Avilés // Josué Hidalgo
 * 
 */

#pragma once

#include "SplayTreeDictionary.h"
#include "HashTable.h"
#include "List.h"
#include "DLinkedList.h"

class TrieNode {
public:
	bool isFinal;
	int prefixCount;
	int letterCount;
	Dictionary<char, TrieNode*>* children;
	List<int>* lines;
	List<int>* wordLines;

	TrieNode() {
		isFinal = false;
		prefixCount = 0;
		letterCount = 0;
		children = new HashTable<char, TrieNode*>();
		lines = new DLinkedList<int>();
		wordLines = new DLinkedList<int>();
	}
	~TrieNode() {
		delete lines;
		delete children;
	}

	bool containsChild(char c) {
		return children->contains(c);
	}

	TrieNode* getChild(char c) {
		return children->getValue(c);
	}

	void addChild(char c) {
		children->insert(c, new TrieNode());
	}

	void removeChild(char c) {
		children->remove(c);
	}

	List<char>* getChildren() {
		return children->getKeys();
	}
};

