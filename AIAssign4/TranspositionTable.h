#pragma once
#define ARRAY_SIZE 25000000	

class TranspositionTable {
private:
	unsigned __int64 zobristHashTable[2*42];
	struct Pair {
		unsigned __int64 key;
		unsigned __int16 value; //10 bits for score //6 bits for depth // 3 bits for move 
	};
	Pair TPT[ARRAY_SIZE];

public:
	TranspositionTable();
	unsigned __int16 get(unsigned __int64 playerkey, unsigned __int64 aikey);
	void store(unsigned __int64 playerkey, unsigned __int64 aikey, unsigned __int16 value);
	unsigned __int64 hashFunc(unsigned __int64 playerkey, unsigned __int64 aikey);

};