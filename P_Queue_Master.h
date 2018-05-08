#ifndef __P_QUEUE_MASTER_H
#define __P_QUEUE_MASTER_H

// * Priority queue controller using minimum complete binary heap tree(array)
// * Supports any type of data structure
// * Each heap has a pair of key and data

#include <vector>

enum Heap_Flag { HEAP_SORTING, HEAP_SORT_DONE };

template<typename KEY,typename DATA>
struct Heap {
	KEY key;
	DATA data;
};

template<typename KEY, typename DATA>
class P_Queue_Master {
public:
	P_Queue_Master();
	P_Queue_Master(const int &arraySize);
	~P_Queue_Master();

	void Insert(const KEY &key, const DATA &data);
	DATA Delete();
	DATA Begin();
	void Clear();

	std::vector<Heap<KEY, DATA> > heap;
	int numData;
	int startPtr;
	int endPtr;

private:
	int Sort_Insert(int &arrayPtr);
	int Sort_Delete(int &arrayPtr);
};
#endif