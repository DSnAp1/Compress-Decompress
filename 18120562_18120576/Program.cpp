#include "Program.h"


void Program::Menu()
{
		system("cls");
		cout << "0. Quit"
				<< "\n1. Compress"
				<< "\n2. Decompress";
}

void Program::GetCommand()
{
		string userCommand = "";
		int tmp = 0;

		Menu();

		while (true)
		{
				cout << "\n\nCommand : ";
				rewind(stdin);
				getline(cin, userCommand);

				try 
				{
						if (userCommand == "")
								throw "";

						for (int i = 0; i < userCommand.length(); i++)
						{
								if (userCommand[i] - '0' < 0 || userCommand[i] - '0' > 9)
										throw "";

								tmp = tmp * 10 + (userCommand[i] - '0');
						}

						if (tmp < 0 || tmp > 2)
								throw "";

						break;
				}
				catch (const char* s)
				{
						cout << "Wrong command !!!";
				}
		}

		this->_userCmd = tmp;
}

void Program::StandardizePath(string &path)
{
		for (int i = 0; i < path.length(); i++)
		{
				if (path[i] == '/' )
						path[i] = '\\';
		}
}

bool Program::DoCommand()
{
		system("cls");

		if (_userCmd == 0)
		{
				return false;
		}
		else if (_userCmd == 1)
		{
				string path = "",
						compressPath = "";

				cout << "Input path of file or folder : ";
				rewind(stdin);
				getline(cin, path);
				cout << "Input compressed file's path (end with .myzip) : ";
				rewind(stdin);
				getline(cin, compressPath);


				StandardizePath(path);
				StandardizePath(compressPath);

				if (Compress(path, compressPath))
				{
						cout << "\nCompress successfully.\n";
				}
				else
				{
						cout << "\nFail to compress.\n";
				}

				system("pause");
				return true;
		}
		else if (_userCmd == 2)
		{
				string path = "",
						decompressPath = "";

				cout << "Input path of compressed file (end with .myzip) : ";
				rewind(stdin);
				getline(cin, path);
				cout << "Input path of decompressed file : ";
				rewind(stdin);
				getline(cin, decompressPath);


				StandardizePath(path);
				StandardizePath(decompressPath);

				if (Decompress(path, decompressPath))
				{
						cout << "\nDecompress successfully.\n";
				}
				else
				{
						cout << "\nFail to decompress.\n";
				}

				system("pause");
				return true;
		}
}

void Program::Run()
{
		while (true)
		{
				GetCommand();

				if (!DoCommand())
						break;
		}

		cout << "\nThank you !!\n";
}