#include "TranspositionTable.h"

TranspositionTable::TranspositionTable()
{
	for (int i = 0; i < ARRAY_SIZE; i++) {
		TPT[i] = { 0, 0 };
	}
}

unsigned __int32 TranspositionTable::get(unsigned __int64 key)
{
	unsigned __int64 zHash = hashFunc(key);
	int index = zHash % ARRAY_SIZE;
	if (zHash == (TPT[index].key ^ TPT[index].value))
		return TPT[index].value;
	return 0;
}

void TranspositionTable::store(unsigned __int64 key, unsigned __int32 value)
{
	unsigned __int64 zHash = hashFunc(key);
	key = zHash ^ value;
	int index = zHash % ARRAY_SIZE;
	TPT[index] = { key , value };
}

unsigned __int64 TranspositionTable::hashFunc(unsigned __int64 key)
{

}
