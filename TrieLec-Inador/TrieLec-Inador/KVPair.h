
/*
 *
 * Nombre de Archivo: KVPair.h
 *
 * Descripción General:
 *
 * Este archivo contiene la definición de la clase KVPair, que representa un par clave-valor.
 *
 * Autor: Mauricio Avilés
 *
 */

#pragma once
#include <iostream>

using std::ostream;

template <typename K, typename V>
class KVPair {
public:
	K key;
	V value;

	KVPair() {}

	KVPair(K key) {
		this->key = key;
	}

	KVPair(K key, V value) {
		this->key = key;
		this->value = value;
	}

	KVPair(const KVPair<K, V>& other) {
		key = other.key;
		value = other.value;
	}

	void operator =(const KVPair<K, V>& other) {
		key = other.key;
		value = other.value;
	}

	bool operator ==(const KVPair<K, V>& other) {
		return key == other.key;
	}

	bool operator !=(const KVPair<K, V>& other) {
		return key != other.key;
	}

	bool operator <(const KVPair<K, V>& other) {
		return key < other.key;
	}

	bool operator <=(const KVPair<K, V>& other) {
		return key <= other.key;
	}

	bool operator >(const KVPair<K, V>& other) {
		return key > other.key;
	}

	bool operator >=(const KVPair<K, V>& other) {
		return key >= other.key;
	}

};

template <typename K, typename V>
ostream& operator <<(ostream& os, const KVPair<K, V>& pair) {
	os << "(" << pair.key << ", " << pair.value << ")";
	return os;
}
