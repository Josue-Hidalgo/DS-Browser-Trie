/*
 *
 * Nombre de Archivo: SplayTreeDictionary.h
 *
 * Descripción General:
 *
 * Este archivo contiene la definición de la clase SplayTreeDictionary, que es una implementación
 * de un diccionario utilizando un árbol Splay el cual
 *
 * Autor: Josue Hidalgo // Mauricio Avilés (Basado en BSTree)
 *
 */

#pragma once

#include <stdexcept>
#include <iostream>
#include "Dictionary.h"
#include "SplayTree.h"
#include "KVPair.h"

template <typename K, typename V>
class SplayTreeDictionary : public Dictionary<K, V> {
private:
	SplayTree<KVPair<K, V>>* pairs;

public:
	SplayTreeDictionary() {
		pairs = new SplayTree<KVPair<K, V>>;
	}

	~SplayTreeDictionary() {
		delete pairs;
	}

	void insert(K key, V value) {
		KVPair<K, V> pair(key, value);
		if (pairs->contains(pair))
			throw runtime_error("Key already exists in Dictionary.");
		pairs->insert(pair);
	}

	V remove(K key) {
		KVPair<K, V> pair(key);
		if (!pairs->contains(pair))
			throw runtime_error("Key not found.");
		return pairs->remove(pair).value;
	}

	V getValue(K key) {
		KVPair<K, V> pair(key);
		if (!pairs->contains(pair))
			throw runtime_error("Key not found.");
		return pairs->find(pair).value;
	}

	void setValue(K key, V value) {
		KVPair<K, V> pair(key, value);
		if (!pairs->contains(pair))
			throw runtime_error("Key not found.");
		remove(key);
		insert(key, value);
	}

	bool contains(K key) {
		KVPair<K, V> pair(key);
		return pairs->contains(pair);
	}

	void clear() {
		pairs->clear();
	}

	List<K>* getKeys() {
		List<K>* keys = new DLinkedList<K>();
		List<KVPair<K, V>>* elements = pairs->getElements();
		for (elements->goToStart(); !elements->atEnd(); elements->next())
			keys->append(elements->getElement().key);
		delete elements;
		return keys;
	}

	List<V>* getValues() {
		List<V>* values = new DLinkedList<V>();
		List<KVPair<K, V>>* elements = pairs->getElements();
		for (elements->goToStart(); !elements->atEnd(); elements->next())
			values->append(elements->getElement().value);
		delete elements;
		return values;
	}

	int getSize() {
		return pairs->getSize();
	}

	void update(Dictionary<K, V>* D) {

		List<K>* newDKeys = D->getKeys();

		for (newDKeys->goToStart(); !newDKeys->atEnd(); newDKeys->next()) {
			if (contains(newDKeys->getElement()))
				setValue(newDKeys->getElement(), D->getValue(newDKeys->getElement()));
			else
				insert(newDKeys->getElement(), D->getValue(newDKeys->getElement()));
		}
		delete newDKeys;
	}

	void zip(List<K>* keys, List<V>* values) {
		int minSize = keys->getSize() < values->getSize() ? minSize = keys->getSize() : minSize = values->getSize();
		keys->goToStart();
		values->goToStart();
		for (int i = 0; i < minSize; i++) {
			if (contains(keys->getElement()))
				setValue(keys->getElement(), values->getElement());
			else
				insert(keys->getElement(), values->getElement());
			keys->next();
			values->next();
		}
	}

	void print() {
		List<KVPair<K, V>>* elements = pairs->getElements();
		for (elements->goToStart(); !elements->atEnd(); elements->next()) {
			cout << elements->getElement().key << ": " << elements->getElement().value << endl;
		}
		delete elements;
	}
};