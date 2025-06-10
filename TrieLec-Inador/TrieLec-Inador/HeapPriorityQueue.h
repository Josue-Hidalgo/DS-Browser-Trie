
/*
 *
 * Nombre de Archivo: HeapPriorityQueue.h
 *
 * Descripción General:
 *
 * Clase que implementa una cola de prioridad utilizando un montículo máximo 
 * es decir que sus valores se ubican de cierta manera que al sacarlos vaya 
 * dando los valores máximos. Por su estructura deben ser valores comparables.
 * 
 * Autor: Mauricio Avilés // Josué Hidalgo Sandoval
 *
 */

#pragma once

#include <stdexcept>

#include "PriorityQueue.h"
#include "MaxHeap.h"
#include "KVPair.h"
#include "Def.h"

using std::runtime_error;

template <typename E>
class HeapPriorityQueue : public PriorityQueue<E> {
private:
	MaxHeap<KVPair<int, E>>* pairs;

public:
	HeapPriorityQueue(int max = DEFAULT_MAX) {
		pairs = new MaxHeap<KVPair<int, E>>(max);
	}

	~HeapPriorityQueue() {
		delete pairs;
	}

	void insert(E element, int priority) {
		KVPair<int, E> p(priority, element);
		pairs->insert(p);
	}

	E max() {
		KVPair<int, E> p = pairs->first();
		return p.value;
	}

	E removeMax() {
		KVPair<int, E> p = pairs->removeFirst();
		return p.value;
	}

	void clear() {
		pairs->clear();
	}

	int getSize() {
		return pairs->getSize();
	}

	bool isEmpty() {
		return pairs->isEmpty();
	}

	void print() {
		pairs->print();
	}

};

