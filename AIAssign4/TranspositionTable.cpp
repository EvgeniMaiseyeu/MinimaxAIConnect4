#include "stdafx.h"
#include "TranspositionTable.h"
#include <random>

TranspositionTable::TranspositionTable()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	unsigned __int64 upperBorder = 0b1111111111111111111111111111111111111111111111111111111111111111;
	std::uniform_int_distribution<unsigned __int64> dis(0, upperBorder);

	for (int i = 0; i < 84; i++)
	{
		zobristHashTable[i] = dis(gen);
	}
	for (int i = 0; i < ARRAY_SIZE; i++) {
		TPT[i] = { 0, 0 };
	}
}

unsigned __int16 TranspositionTable::get(unsigned __int64 playerkey, unsigned __int64 aikey)
{
	unsigned __int64 zHash = hashFunc(playerkey, aikey);
	int index = zHash % ARRAY_SIZE;
	if (zHash == (TPT[index].key ^ TPT[index].value))
		return TPT[index].value; 
	return 0;
}

void TranspositionTable::store(unsigned __int64 playerkey, unsigned __int64 aikey, unsigned __int16 value)
{
	unsigned __int64 zHash = hashFunc(playerkey, aikey);
	unsigned __int64 key = zHash ^ value;
	int index = zHash % ARRAY_SIZE;
	TPT[index] = { key , value };
}

unsigned __int64 TranspositionTable::hashFunc(unsigned __int64 playerkey, unsigned __int64 aikey)
{
	unsigned __int64 zobristHash;
	unsigned __int64 check = 1;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			if (playerkey & check) {
				zobristHash = zobristHash ^ zobristHashTable[(i*6 + j)];
			}
			if (aikey & check) {
				zobristHash = zobristHash ^ zobristHashTable[(i * 6 + j) + 42];
			}
			check = check << 1;
		}
		check = check << 1;
	}
	return zobristHash;
}
