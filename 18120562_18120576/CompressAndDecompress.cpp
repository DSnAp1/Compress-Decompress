#include "CompressAndDecompress.h"

template <typename T>
void mySwap(T& a, T&b)
{
		T temp = a;
		a = b;
		b = temp;
}

void FindFrequence(string str, vector<int> &hash)
{
		for (int i = 0; i < str.length(); i++)
		{
				hash[int(str[i]) + 128]++;
		}
}

void CreateFrequenceTable(string str, vector <char> &character, vector <int> &frequence)
{
		if (str == "")
				return;

		vector<int>hash(256, 0);
		vector<int>temp1(256,0);
		vector<int>temp2(256,0);
		vector<int>temp3(256,0);

		int n = str.length() / 3;

		thread task1(FindFrequence, str.substr(0, n), ref(temp1));
		thread task2(FindFrequence, str.substr(n, n), ref(temp2));
		thread task3(FindFrequence, str.substr(2 * n), ref(temp3));

		task1.join();
		task2.join();
		task3.join();

		for (int i = 0; i < 256; i++)
		{
				hash[i] = temp1[i] + temp2[i] + temp3[i];
		}

		//Tao bang tan so
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
						str += '0';
				else
						str += '1';

				c /= 2;
		}

		while (str.length() < 8)
				str += '0';

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

		string str = "";

		if (path.find(".txt") != string::npos)
		{
				inFile.seekg(0, inFile.end);
				int size = inFile.tellg();
				inFile.seekg(0, inFile.beg);
	
				char *temp = new char[size + 1];
				inFile.read(temp, size);
				temp[size] = '\0';

				str.assign(temp);

				delete[] temp;
		}
		else
		{
				char temp;

				while (!inFile.eof())
				{
						inFile.read((char*)&temp, sizeof(temp));
						str += temp;
				}

				str.pop_back();
		}

		inFile.close();

		return str;
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
				for (int i = 0; i < len; i++)
				{
						outFile.write((char*)&filename[i],sizeof(filename[i]));
				}

				//Bang tan so
				vector <char> character; //Cac ky tu trong bang 
				vector <int> frequence; //Tan so tuong ung
				string str = ReadFile(filepath);

				//Tao bang tan so
				CreateFrequenceTable(str, character, frequence);


				//Tao cay huffman
				Node* HuffmanTree = createHuffmanTree(character, frequence);

				//Tao bang ma huffman
				vector <HuffmanCode> CodeTable;
				string code = "";
				createHuffmanCodeTable(HuffmanTree, CodeTable, code);


				//Ghi bang tan so vao file
				short int size_frequence_table = character.size();
				outFile.write((char*)&size_frequence_table, sizeof(short int));
	
				if (size_frequence_table != 0)
				{
						outFile.write((char*)&character[0], sizeof(char) * character.size());
						outFile.write((char*)&frequence[0], sizeof(int) * frequence.size());
				}

				//Tao chuoi nhi phan ma hoa file
				string str_code = "";

				vector <string> hash;
				hash.resize(256);

				unsigned int lengthStrCode = 0;

				for (int i = 0; i < CodeTable.size(); i++)
				{
						hash[int(CodeTable[i].c) + 128] = CodeTable[i].code;

						lengthStrCode += CodeTable[i].code.length() * CodeTable[i].frequence;
				}

				outFile.write((char*)&lengthStrCode, sizeof(unsigned int));
				
				vector<unsigned char>decimalCode;

				for (int i = 0; i < str.length(); i++)
				{
						str_code += hash[int(str[i]) + 128];

						while (str_code.length() >= 8)
						{
								decimalCode.push_back(BinaryToDecimal(str_code.substr(0, 8)));
								str_code = str_code.substr(8);
						}
				}

				if (str_code.length() > 0)
				{
						while (str_code.length() < 8)
								str_code += '0';

						decimalCode.push_back(BinaryToDecimal(str_code));
				}
				

				unsigned int decimalCodeLen = decimalCode.size();
				outFile.write((char*)&decimalCodeLen, sizeof(unsigned int));

				if (decimalCodeLen != 0)
				{
						outFile.write((char*)&decimalCode[0], decimalCodeLen);
				}

				if(HuffmanTree != NULL)
						DeleteTree(HuffmanTree);

				count++;
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

		while (!inFile.eof())
		{
				//Doc so thu tu file
				inFile.read((char*)&count, sizeof(unsigned char));

				if (count == count2)
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

						//Tao file luu trong duong dan thu muc o tren voi ten vua duoc doc tu file ma hoa
						string path = decompressPath + '\\' + filename;
						ofstream outFile(path, ios::out | ios::binary);

						//Doc bang tan so
						short int size_frequence_table;
						inFile.read((char*)&size_frequence_table, sizeof(short int));

						vector <char> character(size_frequence_table);
						vector <int> frequence(size_frequence_table);

						if (size_frequence_table != 0)
						{
								inFile.read((char*)&character[0], sizeof(char) * size_frequence_table);
								inFile.read((char*)&frequence[0], sizeof(int) * size_frequence_table);
						}

						//Doc chieu dai ma nhi phan da ma hoa file
						unsigned int lengthStrCode;
						inFile.read((char*)&lengthStrCode, sizeof(unsigned int));

						//Doc cac ma nhi phan da ma hoa file
						string str_code = "";
						vector<string> hash(256, "");

						// Doc chieu dai cua decimal code
						unsigned int decimalCodeLen;
						inFile.read((char*)&decimalCodeLen, sizeof(unsigned int));

						// Doc cac decimal code
						vector<unsigned char>decimalCode(decimalCodeLen);

						if (decimalCodeLen != 0)
						{
								inFile.read((char*)&decimalCode[0], decimalCodeLen);

								for (auto& code : decimalCode)
								{
										if (hash[int(code)] == "")
										{
												hash[int(code)] = DecimalToBinary(code);
												str_code += hash[int(code)];
										}
										else
										{
												str_code += hash[int(code)];
										}
								}
						}

						//Xoa cac ky tu 0 thua o cuoi ma nhi phan ma hoa file
						if (str_code != "")
								str_code.erase(lengthStrCode);


						//Xay dung lai cay huffman
						Node* HuffmanTree = createHuffmanTree(character, frequence);

		
						//Duyet cac ma nhi phan tren cay de tim ky tu duoc ma hoa va dua vao file giai nen
						if (HuffmanTree != NULL)
						{
								unsigned int i = 0;
								Node *p = HuffmanTree;

								while (i <= lengthStrCode)
								{

										if (p->left == NULL && p->right == NULL)
										{
												outFile.write((char*)&p->c, sizeof(p->c));

												p = HuffmanTree;

												if (i == lengthStrCode)
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
						}

						outFile.close();

						if (HuffmanTree != NULL)
								DeleteTree(HuffmanTree);

						count2++;
				}
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

		if (pos == -1 || !DecompressFile(path, decompressPath, pos))
		{
				if (decompressPath.back() != ':')
						fs::remove(decompressPath);

				return false;
		}

		return true;
}