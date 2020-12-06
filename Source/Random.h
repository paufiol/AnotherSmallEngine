#ifndef _RANDOM_H_
#define _RANDOM_H_
#include "Globals.h"

struct Random {
public:
	Random() {};
	Random(uint32 num);
	~Random() {};

	uint32 GenerateRandomIntRange(int first, int last);
	uint32 GenerateRandomInt();

private:
	uint32 num;

};
#endif // ! _RANDOM_H_