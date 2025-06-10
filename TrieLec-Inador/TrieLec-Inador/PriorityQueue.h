/*
 *
 * Nombre de Archivo: PriorityQueue.h
 *
 * Descripción General:
 *
 * Clase abstracta que define la interfaz de una cola de prioridad. Esta clase no puede ser instanciada.
 * Esta clase define los métodos que deben implementar las clases que tengan una cola de prioridad como 
 * parte de su implementación o la forma de una cola de prioridad. Nos funcionará para las clases
 * HeapPriorityQueue y LinkedPriorityQueue.
 *
 * Autor: Mauricio Avilés // Josué Hidalgo
 *
 */

#pragma once

template <typename E>
class PriorityQueue {
private:
	PriorityQueue(const PriorityQueue<E>& other) {}
	void operator =(const PriorityQueue<E>& other) {}

public:
	PriorityQueue() {}
	virtual ~PriorityQueue() {}
	virtual void insert(E element, int priority) = 0;
	virtual E min() = 0;
	virtual E removeMin() = 0;
	virtual void clear() = 0;
	virtual int getSize() = 0;
	virtual bool isEmpty() = 0;
	virtual void print() = 0;
};

