#pragma once
#define ARRAY_SIZE 42000000	

class TranspositionTable {
private:
	unsigned __int64 zobristHashTable[2*42];
	struct Pair {
		unsigned __int64 key;
		unsigned __int32 value; //10 bits for score //6 bits for depth // 3 bits for move //1 bit for negative score 
	};
	Pair TPT[ARRAY_SIZE];

public:
	TranspositionTable();
	unsigned __int32 get(unsigned __int64 playerkey, unsigned __int64 aikey);
	void store(unsigned __int64 playerkey, unsigned __int64 aikey, unsigned __int32 value);
	unsigned __int64 hashFunc(unsigned __int64 playerkey, unsigned __int64 aikey);

};