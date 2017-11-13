#pragma once
#define ARRAY_SIZE 25000000	

class TranspositionTable {
private:
	struct Pair {
		unsigned __int64 key;
		unsigned __int32 value; //10 bits for score //6 bits for depth // 3 bits for move 
	};
	Pair TPT[ARRAY_SIZE];
public:
	TranspositionTable();
	unsigned __int32 get(unsigned __int64 key);
	void store(unsigned __int64 key, unsigned __int32 value);
	unsigned __int64 hashFunc(unsigned __int64 key);

};