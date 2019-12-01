#pragma once
#include "CompressAndDecompress.h"


class Program
{
private:
		int _userCmd;

		void StandardizePath(string);
public:
		Program(){}
		~Program(){}

		void Menu();
		void GetCommand();
		bool DoCommand();

		void Run();
};