/*
 *
 * Nombre de Archivo: MaxHeap.h
 *
 * Descripción General:
 *
 * Este archivo contiene la implementación de una clase MaxHeap, que es un
 * montículo máximo. Esta estructura de datos permite almacenar elementos de
 * valor numérico y mantener la propiedad de montículo, donde el elemento
 * que se encuentra en la raíz es el mayor de todos los elementos.
 *
 * Autor: Josue Hidalgo
 *
 */

#pragma once

#include <stdexcept>
#include <iostream>
#include "Def.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class MaxHeap {
private:
	E* elements;
	int max;
	int size;

	void siftUp(int pos) {
		while (pos != 0 && elements[pos] > elements[parent(pos)]) {
			swap(pos, parent(pos));
			pos = parent(pos);
		}
	}

	void siftDown(int pos) {
		while (!isLeaf(pos) && elements[pos] < elements[maxChild(pos)]) {
			int child = maxChild(pos);
			swap(pos, child);
			pos = child;
		}
	}

	int parent(int pos) {
		return (pos - 1) / 2;
	}

	int left(int pos) {
		return 2 * pos + 1;
	}

	int right(int pos) {
		return 2 * pos + 2;
	}

	int maxChild(int pos) {
		if (right(pos) < size
			&& elements[right(pos)] > elements[left(pos)]) {
			return right(pos);
		}
		return left(pos);
	}

	void swap(int pos1, int pos2) {
		E temp = elements[pos1];
		elements[pos1] = elements[pos2];
		elements[pos2] = temp;
	}

	bool isLeaf(int pos) {
		return left(pos) >= size;
	}

public:
	MaxHeap(int max = DEFAULT_MAX) {
		if (max < 1)
			throw runtime_error("Invalid max size.");
		elements = new E[max];
		this->max = max;
		size = 0;
	}

	~MaxHeap() {
		delete[] elements;
	}

	void insert(E element) {
		if (size == max)
			throw runtime_error("Heap is full.");
		elements[size] = element;
		siftUp(size);
		size++;
	}

	E first() {
		if (size == 0)
			throw runtime_error("Heap is empty.");
		return elements[0];
	}

	E removeFirst() {
		if (size == 0)
			throw runtime_error("Heap is empty.");
		return remove(0);
	}

	E remove(int pos) {
		if (pos < 0 || pos >= size)
			throw runtime_error("Index out of range.");
		swap(pos, size - 1);
		size--;
		siftDown(pos);
		return elements[size];
	}

	void clear() {
		size = 0;
	}

	int getSize() {
		return size;
	}

	bool isEmpty() {
		return size == 0;
	}

	void print() {
		cout << "[ ";
		for (int i = 0; i < size; i++)
			cout << elements[i] << " ";
		cout << "]" << endl;
	}

};

