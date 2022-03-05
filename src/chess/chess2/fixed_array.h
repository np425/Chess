#ifndef CHESS_FIXED_ARRAY_H
#define CHESS_FIXED_ARRAY_H

/* Fixed size array with tracking of how many elements are stored */

template <typename T, unsigned S>
class FixedArray {
	T elem[S];
	T* head;

public:
	FixedArray();
	T operator[](unsigned) const;

	void append(T item);
	void clear();
	bool isEmpty() const;
	unsigned size() const;

	T* it();
	const T* it() const;
	const T* end() const;
};

template <typename T, unsigned S>
FixedArray<T,S>::FixedArray() {
	head = elem;
}

template <typename T, unsigned S>
T FixedArray<T,S>::operator[](unsigned idx) const {
	return elem[idx];
}

template <typename T, unsigned S>
void FixedArray<T,S>::append(T item) {
	*(head++) = item;
}

template <typename T, unsigned S>
void FixedArray<T,S>::clear() {
	head = elem;
}

template <typename T, unsigned S>
bool FixedArray<T,S>::isEmpty() const {
	return head == elem;
}

template <typename T, unsigned S>
unsigned FixedArray<T,S>::size() const {
	return head-elem;
}

template <typename T, unsigned S>
T* FixedArray<T,S>::it() {
	return elem;
}

template <typename T, unsigned S>
const T* FixedArray<T,S>::it() const {
	return elem;
}

template <typename T, unsigned S>
const T* FixedArray<T,S>::end() const {
	return head;
}

#endif
