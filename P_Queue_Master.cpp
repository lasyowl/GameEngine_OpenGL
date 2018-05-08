#include "P_Queue_Master.h"

template<typename KEY, typename DATA>
P_Queue_Master<KEY, DATA>::P_Queue_Master() : numData(0), endPtr(0), startPtr(0) {
	heap.resize(1000);
}

template<typename KEY, typename DATA>
P_Queue_Master<KEY, DATA>::P_Queue_Master(const int &arraySize) : numData(0), endPtr(0), startPtr(0) {
	heap.resize(arraySize);
}

template<typename KEY, typename DATA>
P_Queue_Master<KEY, DATA>::~P_Queue_Master() {

}

template<typename KEY, typename DATA>
void P_Queue_Master<KEY, DATA>::Insert(const KEY &key, const DATA &data) {
	heap[endPtr].key = key;
	heap[endPtr].data = data;

	int tempPtr = endPtr;
	int sortFlag = HEAP_SORTING;
	while (sortFlag == HEAP_SORTING) {
		sortFlag = Sort_Insert(tempPtr);
	}
	
	endPtr++;
	numData++;
}

template<typename KEY, typename DATA>
int P_Queue_Master<KEY, DATA>::Sort_Insert(int &arrayPtr) {
	int parent = (arrayPtr + 1) / 2 - 1;
	if (parent < 0) {
		return HEAP_SORT_DONE;
	}
	else {
		if (heap[parent].key < heap[arrayPtr].key) {
			return HEAP_SORT_DONE;
		}
		else {
			Heap<KEY, DATA> temp = heap[arrayPtr];
			heap[arrayPtr] = heap[parent];
			heap[parent] = temp;
			arrayPtr = parent;
			return HEAP_SORTING;
		}
	}
}

template<typename KEY, typename DATA>
DATA P_Queue_Master<KEY, DATA>::Delete() {
	DATA retValue;
	endPtr--;
	numData--;
	if (numData < 0) {
		numData = 0;
		endPtr = 0;
		return NULL;
	}
	retValue = heap[startPtr].data;

	heap[startPtr] = heap[endPtr];

	int tempPtr = startPtr;
	int sortFlag = HEAP_SORTING;
	while (sortFlag == HEAP_SORTING) {
		sortFlag = Sort_Delete(tempPtr);
	}
	return retValue;
}

template<typename KEY, typename DATA>
int P_Queue_Master<KEY, DATA>::Sort_Delete(int &arrayPtr) {
	int left = arrayPtr * 2 + 1;
	int right = arrayPtr * 2 + 2;
	Heap<KEY, DATA> temp;
	if (left > numData) {
		return HEAP_SORT_DONE;
	}
	else if (right > numData) {
		if (heap[arrayPtr].key > heap[left].key) {
			temp = heap[arrayPtr];
			heap[arrayPtr] = heap[left];
			heap[left] = temp;
		}
		return HEAP_SORT_DONE;
	}
	else {
		if (heap[left].key < heap[right].key) {
			if (heap[arrayPtr].key < heap[left].key) {
				return HEAP_SORT_DONE;
			}
			else {
				temp = heap[arrayPtr];
				heap[arrayPtr] = heap[left];
				heap[left] = temp;
				arrayPtr = left;
				return HEAP_SORTING;
			}
		}
		else {
			if (heap[arrayPtr].key < heap[right].key) {
				return HEAP_SORT_DONE;
			}
			else {
				temp = heap[arrayPtr];
				heap[arrayPtr] = heap[right];
				heap[right] = temp;
				arrayPtr = right;
				return HEAP_SORTING;
			}
		}
	}
}

template<typename KEY, typename DATA>
DATA P_Queue_Master<KEY, DATA>::Begin() {
	return heap[startPtr].data;
}

template<typename KEY, typename DATA>
void P_Queue_Master<KEY, DATA>::Clear() {
	numData = 0;
	endPtr = 0;
}