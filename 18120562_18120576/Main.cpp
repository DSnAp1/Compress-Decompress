#include "CompressAndDecompress.h"
namespace fs = std::experimental::filesystem;

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

// get main folder name
string GetFolderOrFileName(string path)
{
		string name = "";

		auto index = path.find_last_of('\\');

		for (int i = index + 1; i < path.length(); i++)
				name += path[i];
		
		return name;
}

string GetFolderOrFileName(string path, int startIndex)
{
		string name = "";

		for (int i = startIndex; i < path.length(); i++)
				name += path[i];

		return name;
}


void Compress(string path, string compressPath)
{
		ofstream outFile(compressPath, ios::out | ios::binary);

		///
		/// COMPRESS FOLDER
		///

		vector<string> filePath, folderPath;
		GetAllPath(path, filePath, folderPath);

		string mainFolderName = GetFolderOrFileName(path);
		int startIndex = path.length() - mainFolderName.length();

		string folderCode = "";
		for (auto& item : folderPath)
		{
				folderCode += GetFolderOrFileName(item,startIndex) + '\n';
		}

		unsigned char lenFolderCode = folderCode.length();

		outFile.write((char*)&lenFolderCode, sizeof(lenFolderCode));
		for (int i = 0; i < int(lenFolderCode); i++)
		{
				outFile.write((char*)&folderCode[i], sizeof(folderCode[i]));
		}

		///
		/// COMPRESS FILE
		///

		unsigned char count = 1;

		for (auto& filepath : filePath)
		{
				// ghi so thu tu
				outFile.write((char*)&count, sizeof(count));
				
				string filename = GetFolderOrFileName(filepath, startIndex);
				
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
						return;

				//Tao cay huffman
				Node* HuffmanTree = createHuffmanTree(character, frequence);

				if (HuffmanTree == NULL)
				{
						return;
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
}

void Decompress(string path, string decompressPath)
{
		ifstream inFile(path, ios::in | ios::binary);

		///
		/// DECOMPRESS FOLDER
		///

		unsigned char lenFolderCode;
		inFile.read((char*)&lenFolderCode, sizeof(lenFolderCode));

		string folderCode = "";
		char tmp;
		for (int i = 0; i < int(lenFolderCode); i++)
		{
				inFile.read((char*)&tmp, sizeof(tmp));
				folderCode += tmp;
		}

		vector<string> folderPath;
		string tmpp = "";
		for (int i = 0; i < lenFolderCode; i++)
		{
				if (folderCode[i] == '\n')
				{
						folderPath.push_back(tmpp);
						tmpp = "";
				}
				else
				{
						tmpp += folderCode[i];
				}
		}

		for (auto& item : folderPath)
		{
				fs::create_directory(decompressPath + '\\' + item);
		}

		///
		/// DECOMPRESS FILE
		/// 

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
				char tmpp;
				for (int i = 0; i < int(length_name_file); i++)
				{
						inFile.read((char*)&tmpp, sizeof(char));
						filename += tmpp;
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
				while (str_code.length() < length_str_code)
				{
						//Doc cac ky tu
						inFile.read((char*)&c, sizeof(unsigned char));

						//Chuyen thanh ma nhi phan
						str_code += DecimalToBinary(c);
				}

				//Xoa cac ky tu 0 thua o cuoi ma nhi phan ma hoa file
				str_code.erase(length_str_code);

				//Xay dung lai cay huffman
				Node* HuffmanTree = createHuffmanTree(character, frequence);
				if (HuffmanTree == NULL)
						return;

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
}


int main()
{
		Compress("E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\a", "E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\nen.myzip");
		Decompress("E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\nen.myzip", "E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\ak");

		system("pause");
		return 0;
}