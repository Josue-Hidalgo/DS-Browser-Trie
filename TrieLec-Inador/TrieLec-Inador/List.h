
/*
 *
 * Nombre de Archivo: List.h
 *
 * Descripción General:
 *
 * Clase abstracta que define la interfaz de una lista. Esta clase no puede ser instanciada.
 * Mas bien, es una plantilla que define los métodos que deben implementar las clases que
 * tengan una lista como parte de su implementación o la forma de una lista.
 *
 * Autor: Mauricio Avilés
 *
 */

#pragma once

template <typename E>
class List {
private:
	List(const List<E>& other) {}
	void operator =(const List<E>& other) {}

public:
	List() {}
	virtual ~List() {}
	virtual void insert(E element) = 0;
	virtual void append(E element) = 0;
	virtual E remove() = 0;
	virtual E getElement() = 0;
	virtual void clear() = 0;
	virtual void goToStart() = 0;
	virtual void goToEnd() = 0;
	virtual void goToPos(int pos) = 0;
	virtual void next() = 0;
	virtual void previous() = 0;
	virtual bool atStart() = 0;
	virtual bool atEnd() = 0;
	virtual int getPos() = 0;
	virtual int getSize() = 0;
	virtual void print() = 0;
};

