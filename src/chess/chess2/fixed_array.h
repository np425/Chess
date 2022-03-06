#ifndef CHESS_FIXED_ARRAY_H
#define CHESS_FIXED_ARRAY_H

/* Fixed size array with tracking of how many elements are stored */

template <typename T, unsigned S>
class FixedArray {
	T elem[S];
	T* head;

public:
	FixedArray();
	FixedArray(const FixedArray&);
	FixedArray& operator=(FixedArray arr);
	T operator[](unsigned) const;

	void append(T item);
	void pop();
	void reset();
	bool isEmpty() const;
	unsigned size() const;

	T* it();
	const T* it() const;
	const T* end() const;

	T* newIt();
};

template <typename T, unsigned S>
void FixedArray<T,S>::pop() {
	if (head > elem) {
		--head;
	}
}

template <typename T, unsigned S>
FixedArray<T,S>::FixedArray(const FixedArray& arr) {
	head = elem;
	for (const T* it = arr.it(); it < arr.end(); ++it) {
		*(head++) = *it;
	}
}

template <typename T, unsigned S>
FixedArray<T,S>& FixedArray<T,S>::FixedArray::operator=(FixedArray arr) {
	head = elem;
	for (const T* it = arr.it(); it < arr.end(); ++it) {
		*(head++) = *it;
	}
	return *this;
}

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
void FixedArray<T,S>::reset() {
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

template <typename T, unsigned S>
T* FixedArray<T,S>::newIt() {
	return head++;
}

#endif
