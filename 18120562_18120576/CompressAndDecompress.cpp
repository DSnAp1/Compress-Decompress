#include "CompressAndDecompress.h"

int findElementOfVector(vector <char> character, char c)
{
		if (character.empty())
				return -1;
		for (int i = 0; i < character.size(); i++)
				if (character[i] == c)
						return i;
		return -1;
}

template <typename T>
void mySwap(T& a, T&b)
{
		T temp = a;
		a = b;
		b = temp;
}

bool createFrequenceTable(string str, vector <char> &character, vector <int> &frequence)
{
		//Neu chuoi rong tra ve false
		if (str.empty())
				return false;

		//Tao bang tan so
		vector <int> hash;
		hash.resize(256);
		for (int i = 0; i < str.length(); i++)
		{
				hash[int(str[i]) + 128]++;
		}

		for (int i = 0; i < hash.size(); i++)
		{
				if (hash[i] != 0)
				{
						char c = char(i - 128);
						character.push_back(c);
						frequence.push_back(hash[i]);
				}
		}

		//Sap xep lai bang tan so theo thu tu tang dan theo tan so (frequence)
		for (int i = 0; i < frequence.size() - 1; i++)
		{
				int min = i;
				for (int j = i + 1; j < frequence.size(); j++)
						if (frequence[j] < frequence[min])
								min = j;
				mySwap(frequence[i], frequence[min]);
				mySwap(character[i], character[min]);
		}
		return true;
}

unsigned char BinaryToDecimal(string str)
{

		unsigned char S = 0;
		int temp = 0;
		for (int i = str.length() - 1; i >= 0; i--)
		{
				if (str[i] == '1')
						S += pow(2, temp);
				temp++;
		}

		return S;
}

string DecimalToBinary(unsigned char c)
{
		string str = "";
		while (c != 0)
		{
				if (c % 2 == 0)
						str += "0";
				else
						str += "1";
				c = c / 2;
		}
		while (str.length() < 8)
				str += "0";
		for (int i = 0; i < str.length() / 2; i++)
		{
				mySwap(str[i], str[str.length() - 1 - i]);
		}

		return str;
}

string ReadFile(string path)
{
		ifstream inFile(path, ios::in | ios::binary);

		if (inFile.fail() || !inFile.good())
				return "";

		//Doc tung ky tu (tung byte) cua file dua vao chuoi
		string temp = "";
		char c;
		while (!inFile.eof())
		{
				inFile.read((char*)&c, sizeof(char));
				temp += c;
		}

		temp.erase(temp.length() - 1);

		inFile.close();

		return temp;
}


bool CompressFile(string compressPath, vector<string>filePath, int indexFileName)
{
		ofstream outFile(compressPath, ios::out | ios::binary | ios::app);

		if (outFile.fail() || !outFile.good())
				return false;


		unsigned char count = 1;

		for (auto& filepath : filePath)
		{
				// ghi so thu tu
				outFile.write((char*)&count, sizeof(count));

				string filename = GetFileOrFolderName(filepath, indexFileName);

				// ghi do dai ten file
				unsigned char len = filename.length();
				outFile.write((char*)&len, sizeof(len));

				// ghi ten file
				for (int i = 0; i < int(len); i++)
				{
						outFile.write((char*)&filename[i], sizeof(filename[i]));
				}

				//Bang tan so
				vector <char> character; //Cac ky tu trong bang 
				vector <int> frequence; //Tan so tuong ung

				string str = ReadFile(filepath);

				//Tao bang tan so
				if (!createFrequenceTable(str, character, frequence))
						return false;

				//Tao cay huffman
				Node* HuffmanTree = createHuffmanTree(character, frequence);

				if (HuffmanTree == NULL)
				{
						return false;
				}
				else
				{
						//Tao bang ma huffman
						vector <HuffmanCode> CodeTable;
						string code = "";
						createHuffmanCodeTable(HuffmanTree, CodeTable, code);

						//Tao chuoi nhi phan ma hoa file
						string str_code = "";

						vector <string> hash;
						hash.resize(256);

						for (int i = 0; i < CodeTable.size(); i++)
						{
								hash[int(CodeTable[i].c) + 128] = CodeTable[i].code;
						}

						for (int i = 0; i < str.length(); i++)
						{
								str_code += hash[int(str[i]) + 128];
						}


						//Ghi bang tan so vao file
						int size_frequence_table = character.size();
						outFile.write((char*)&size_frequence_table, sizeof(int));

						for (int i = 0; i < size_frequence_table; i++)
						{
								outFile.write((char*)&character[i], sizeof(char));
								outFile.write((char*)&frequence[i], sizeof(int));
						}

						//Ghi chuoi nhi phan ma hoa vao file
						int length_str_code = str_code.length();

						outFile.write((char*)&length_str_code, sizeof(int));

						while (str_code.length() % 8 != 0)
						{
								str_code += "0";
						}

						int temp = 0;
						while (temp * 8 < str_code.length())
						{
								string str_temp = "";
								str_temp += str_code.substr(temp * 8, 8);

								unsigned char decimal = BinaryToDecimal(str_temp);

								outFile.write((char*)&decimal, sizeof(unsigned char));
								temp++;
						}

						count++;
						DeleteTree(HuffmanTree);
				}
		}

		outFile.close();

		return true;
}

bool DecompressFile(string path, string decompressPath, int pos)
{
		ifstream inFile(path, ios::in | ios::binary);

		if (inFile.fail() || !inFile.good())
				return false;

		inFile.seekg(pos, ios::beg);

		unsigned char count; //So thu tu file duoc doc 
		unsigned char count2 = 1; //So thu tu file tao de so sanh

		//Doc so thu tu file
		inFile.read((char*)&count, sizeof(unsigned char));

		while (!inFile.eof() && count == count2)
		{
				//Doc do dai ten file
				unsigned char length_name_file;

				inFile.read((char*)&length_name_file, sizeof(unsigned char));

				//Doc ten file
				string filename = "";
				char tmp;
				for (int i = 0; i < int(length_name_file); i++)
				{
						inFile.read((char*)&tmp, sizeof(char));
						filename += tmp;
				}

				//Doc bang tan so
				vector <char> character;
				vector <int> frequence;
				int size_frequence_table;
				
				inFile.read((char*)&size_frequence_table, sizeof(int));

				character.resize(size_frequence_table);
				frequence.resize(size_frequence_table);
				for (int i = 0; i < size_frequence_table; i++)
				{
						inFile.read((char*)&character[i], sizeof(char));
						inFile.read((char*)&frequence[i], sizeof(int));
				}

				//Doc chieu dai ma nhi phan da ma hoa file
				int length_str_code;
				inFile.read((char*)&length_str_code, sizeof(int));

				//Doc cac ma nhi phan da ma hoa file
				unsigned char c;
				string str_code = "";

				vector<string> hash;
				hash.resize(256);
				for (int i = 0; i < 256; i++)
					hash[i] = "";
				while (str_code.length() < length_str_code)
				{
						//Doc cac ky tu
						inFile.read((char*)&c, sizeof(unsigned char));

						//Chuyen thanh ma nhi phan
						if (hash[int(c)] == "")
						{
							hash[int(c)] = DecimalToBinary(c);
							str_code += hash[int(c)];
						}
						else
							str_code += hash[int(c)];
				}

				//Xoa cac ky tu 0 thua o cuoi ma nhi phan ma hoa file
				str_code.erase(length_str_code);
				//Xay dung lai cay huffman
				Node* HuffmanTree = createHuffmanTree(character, frequence);
				if (HuffmanTree == NULL)
						return false;

				//Tao file luu trong duong dan thu muc o tren voi ten vua duoc doc tu file ma hoa
				string path = decompressPath + '\\' + filename;
				ofstream outFile(path, ios::out | ios::binary);

				//Duyet cac ma nhi phan tren cay de tim ky tu duoc ma hoa va dua vao file giai nen
				int i = 0;
				Node *p = HuffmanTree;
				while (i <= length_str_code)
				{

						if (p->left == NULL && p->right == NULL)
						{
								outFile.write((char*)&p->c, sizeof(char));

								p = HuffmanTree;

								if (i == length_str_code)
										i++;

								continue;
						}
						if (str_code[i] == '0')
						{
								p = p->left;
						}
						else
						{
								if (str_code[i] == '1')
										p = p->right;
						}

						i++;
				}


				outFile.close();
				DeleteTree(HuffmanTree);

				//Doc so thu tu file tiep theo
				inFile.read((char*)&count, sizeof(unsigned char));
				count2++;
		}

		inFile.close();

		return true;
}



bool CompressFolder(string compressPath, vector<string>folderPath, int indexFileName)
{
		ofstream outFile(compressPath, ios::out | ios::binary);

		if (outFile.fail() || !outFile.good())
				return false;

		if (folderPath.size() != 0)
		{
				string folderCode = "";

				for (auto& item : folderPath)
				{
						folderCode += GetFileOrFolderName(item, indexFileName) + '\n';
				}

				unsigned char lenFolderCode = folderCode.length();

				outFile.write((char*)&lenFolderCode, sizeof(lenFolderCode));
				for (int i = 0; i < int(lenFolderCode); i++)
				{
						outFile.write((char*)&folderCode[i], sizeof(folderCode[i]));
				}
		}
		else
		{
				unsigned char lenFolderCode = 0;

				outFile.write((char*)&lenFolderCode, sizeof(unsigned char));
		}

		outFile.close();

		return true;

}

int DecompressFolder(string path, string decompressPath)
{
		ifstream inFile(path, ios::in | ios::binary);

		if (inFile.fail() || !inFile.good())
				return -1;

		unsigned char lenFolderCode;
		inFile.read((char*)&lenFolderCode, sizeof(lenFolderCode));

		if (int(lenFolderCode) > 0)
		{
				vector<string> folderPath;
				string folderCode = "";
				char tmp;

				for (int i = 0; i < int(lenFolderCode); i++)
				{
						inFile.read((char*)&tmp, sizeof(tmp));

						if (tmp == '\n')
						{
								folderPath.push_back(folderCode);
								folderCode = "";
						}
						else
						{
								folderCode += tmp;
						}
				}

				if (fs::exists(decompressPath + '\\' + folderPath[0]))
						return -1;

				for (auto& item : folderPath)
				{		
						fs::create_directory(decompressPath + '\\' + item);
				}
		}

		return inFile.tellg();
}



void GetAllPath(string path, vector<string> &filePath, vector<string> &folderPath)
{
		if (!fs::is_directory(path))
		{
				filePath.push_back(path);
				return;
		}
		else
		{
				folderPath.push_back(path);

				for (auto& entry : fs::directory_iterator(path))
				{
						GetAllPath(entry.path().string(), filePath, folderPath);
				}
		}
}

string GetFileOrFolderName(string path, int startIndex)
{
		return path.substr(startIndex);
}

bool Compress(string path, string compressPath)
{
		if (!fs::exists(path))
				return false;

		if (compressPath.find(".myzip") == string::npos)
				return false;

		vector<string> filePath, folderPath;

		GetAllPath(path, filePath, folderPath);

		int indexFileName = path.find_last_of('\\') + 1;

		if (!CompressFolder(compressPath, folderPath, indexFileName) ||
				!CompressFile(compressPath, filePath, indexFileName))
		{
				fs::remove(compressPath);
				return false;
		}

		return true;
}

bool Decompress(string path, string decompressPath)
{
		if (!fs::exists(path) || path.find(".myzip") == string::npos)
				return false;
	
		
		if (fs::exists(decompressPath))
		{
				// Kiem tra co phai o dia hay khong
				if(decompressPath.back() != ':')
						return false;
		}
		else
		{
				fs::create_directory(decompressPath);
		}
		

		int pos = DecompressFolder(path, decompressPath);

		if (pos == -1)
				return false;

		if (!DecompressFile(path, decompressPath, pos))
				return false;

		return true;
}