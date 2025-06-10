/*
 *
 * Nombre de Archivo: SortedDLinkedList.h
 *
 * Descripción General:
 *
 * Clase que implementa una lista ordenada utilizando herencia
 * especializando la clase DLinkedList. Esta clase ubica los
 * elementos en orden ascendente al insertarlos. Pero siendo que
 * LinkedList al insertar un elemento no requiere iterarlo de nuevo
 * es una alternativa más eficiente que OrderedArrayList.
 *
 * Autor: Josue Hidalgo
 *
 */


#pragma once

#include "DLinkedList.h"

template <typename E>
class SortedDLinkedList : public DLinkedList<E> {
public:
	SortedDLinkedList() : DLinkedList<E>() {}

	void insert(E element) {
		DLinkedList<E>::goToStart();
		while (!DLinkedList<E>::atEnd() && element >= DLinkedList<E>::getElement())
			DLinkedList<E>::next();
		DLinkedList<E>::insert(element);
	}

	void append(E element) {
		insert(element);
	}

};

