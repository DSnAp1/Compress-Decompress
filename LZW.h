#pragma once
#include "UtilityLib.h"

class LZW
{
public:
		static vector<int> Compress(string data);
		static string Decompress(vector<int> code);
};

