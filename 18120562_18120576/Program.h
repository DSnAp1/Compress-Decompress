#pragma once
#include "CompressAndDecompress.h"


class Program
{
private:
		int _userCmd;

public:
		Program(){}
		~Program(){}

		void Menu();
		void GetCommand();
		bool DoCommand();

		void Run();
};