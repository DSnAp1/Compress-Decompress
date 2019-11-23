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
	for (int i = 0; i < str.length(); i++)
	{
		//Neu ky tu chua co trong bang thi them vao, neu da co thi tang tan so len 1
		if (findElementOfVector(character, str[i]) == -1)
		{
			character.push_back(str[i]);
			frequence.push_back(1);
		}
		else
			frequence[findElementOfVector(character, str[i])]++;
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

string ReadFile(string filename)
{
	FILE* f = NULL;
	fopen_s(&f, filename.c_str(), "rb");

	//Neu khong mo duoc file thi tra ve chuoi rong
	if (f == NULL)
		return "";

	//Doc tung ky tu (tung byte) cua file dua vao chuoi
	string temp = "";
	char c;
	while (!feof(f))
	{
		fread(&c, sizeof(char), 1, f);
		temp += c;
	}
	temp.erase(temp.length() - 1);
	fclose(f);
	return temp;
}

bool CompressFile(string infilename, string outfilename)
{
	//Bang tan so
	vector <char> character; //Cac ky tu trong bang 
	vector <int> frequence; //Tan so tuong ung

	string str = ReadFile(infilename);
	//Tao bang tan so
	if (!createFrequenceTable(str, character, frequence))
		return false;
	//Tao cay huffman
	Node* HuffmanTree = createHuffmanTree(character, frequence);

	if (HuffmanTree == NULL)
		return false;
	else
	{
		//Tao bang ma huffman
		vector <HuffmanCode> CodeTable;
		string code = "";
		createHuffmanCodeTable(HuffmanTree, CodeTable, code);

		//Tao chuoi nhi phan ma hoa file
		string str_code = "";
		for (int i = 0; i < str.length(); i++)
		{
			for (int j = 0; j < CodeTable.size(); j++)
				if (str[i] == CodeTable[j].c)
				{
					str_code += CodeTable[j].code;
					break;
				}
		}

		FILE* outfile;
		fopen_s(&outfile, outfilename.c_str(), "wb");
		if (outfile == NULL)
			return false;

		//Ghi bang tan so vao file
		int size_frequence_table = character.size();
		fwrite(&size_frequence_table, sizeof(int), 1, outfile);
		for (int i = 0; i < size_frequence_table; i++)
		{
			fwrite(&character[i], sizeof(char), 1, outfile);
			fwrite(&frequence[i], sizeof(int), 1, outfile);
		}

		//Ghi chuoi nhi phan ma hoa vao file
		int length_str_code = str_code.length();

		fwrite(&length_str_code, sizeof(int), 1, outfile);

		while (str_code.length() % 8 != 0)
		{
			str_code += "0";
		}

		int temp = 0;
		while (temp * 8 < str_code.length())
		{
			string str_temp = "";
			for (int i = temp * 8; i < temp * 8 + 8; i++)
				str_temp += str_code[i];
			unsigned char decimal = BinaryToDecimal(str_temp);
			fwrite(&decimal, sizeof(unsigned char), 1, outfile);
			temp++;
		}

		fclose(outfile);
		DeleteTree(HuffmanTree);
		return true;
	}
}

bool DecompressFile(string infilename, string outfilename)
{
	FILE* infile;
	fopen_s(&infile, infilename.c_str(), "rb");
	if (infile == NULL)
		return false;

	//Doc bang tan so
	vector <char> character;
	vector <int> frequence;
	int size_frequence_table;

	fread(&size_frequence_table, sizeof(int), 1, infile);
	character.resize(size_frequence_table);
	frequence.resize(size_frequence_table);
	for (int i = 0; i < size_frequence_table; i++)
	{
		fread(&character[i], sizeof(char), 1, infile);
		fread(&frequence[i], sizeof(int), 1, infile);
	}

	//Doc chieu dai ma nhi phan da ma hoa file
	int length_str_code;
	fread(&length_str_code, sizeof(int), 1, infile);

	//Doc cac ma nhi phan da ma hoa file
	unsigned char c;
	string str_code = "";
	while (str_code.length() < length_str_code)
	{
		//Doc cac ky tu
		fread(&c, sizeof(unsigned char), 1, infile);
		//Chuyen thanh ma nhi phan
		str_code += DecimalToBinary(c);
	}

	//Xoa cac ky tu 0 thua o cuoi ma nhi phan ma hoa file
	str_code.erase(length_str_code);

	//Xay dung lai cay huffman
	Node* HuffmanTree = createHuffmanTree(character, frequence);
	if (HuffmanTree == NULL)
		return false;

	FILE* outfile;
	fopen_s(&outfile, outfilename.c_str(), "wb");
	if (outfile == NULL)
		return false;

	//Duyet cac ma nhi phan tren cay de tim ky tu duoc ma hoa va dua vao file giai nen
	int i = 0;
	Node *p = HuffmanTree;
	while (i <= length_str_code)
	{

		if (p->left == NULL && p->right == NULL)
		{
			fwrite(&p->c, sizeof(char), 1, outfile);
			p = HuffmanTree;
			if (i == length_str_code)
				i++;
			continue;
		}
		if (str_code[i] == '0')
			p = p->left;
		else
			if (str_code[i] == '1')
				p = p->right;
		i++;
	}

	fclose(infile);
	fclose(outfile);
	DeleteTree(HuffmanTree);

	return true;
}

bool CompressFolder(string path, string outfilename)
{
	FILE* outfile;
	fopen_s(&outfile, outfilename.c_str(), "wb");
	if (outfile == NULL)
		return false;
	int count = 1;

	for (auto& entry : experimental::filesystem::directory_iterator(path))
	{
		//Ghi so thu tu file
		fwrite(&count, sizeof(int), 1, outfile);
		//Ghi do dai ten file
		string filename = "";
		for (int i = entry.path().string().length() - 1; i >= 0; i--)
		{
			if (entry.path().string()[i] != '\\')
				filename += entry.path().string()[i];
			else
				break;
		}

		int length_name_file = filename.length();
		fwrite(&length_name_file, sizeof(int), 1, outfile);
		//Ghi ten file
		for (int i = length_name_file - 1; i >= 0; i--)
			fwrite(&filename[i], sizeof(char), 1, outfile);


		//Bang tan so
		vector <char> character; //Cac ky tu trong bang 
		vector <int> frequence; //Tan so tuong ung

		string str = ReadFile(entry.path().string());
		//Tao bang tan so
		if (!createFrequenceTable(str, character, frequence))
			return false;
		//Tao cay huffman
		Node* HuffmanTree = createHuffmanTree(character, frequence);

		if (HuffmanTree == NULL)
			return false;
		else
		{
			//Tao bang ma huffman
			vector <HuffmanCode> CodeTable;
			string code = "";
			createHuffmanCodeTable(HuffmanTree, CodeTable, code);

			//Tao chuoi nhi phan ma hoa file
			string str_code = "";
			for (int i = 0; i < str.length(); i++)
			{
				for (int j = 0; j < CodeTable.size(); j++)
					if (str[i] == CodeTable[j].c)
					{
						str_code += CodeTable[j].code;
						break;
					}
			}


			//Ghi bang tan so vao file
			int size_frequence_table = character.size();
			fwrite(&size_frequence_table, sizeof(int), 1, outfile);
			for (int i = 0; i < size_frequence_table; i++)
			{
				fwrite(&character[i], sizeof(char), 1, outfile);
				fwrite(&frequence[i], sizeof(int), 1, outfile);
			}

			//Ghi chuoi nhi phan ma hoa vao file
			int length_str_code = str_code.length();

			fwrite(&length_str_code, sizeof(int), 1, outfile);

			while (str_code.length() % 8 != 0)
			{
				str_code += "0";
			}

			int temp = 0;
			while (temp * 8 < str_code.length())
			{
				string str_temp = "";
				for (int i = temp * 8; i < temp * 8 + 8; i++)
					str_temp += str_code[i];
				unsigned char decimal = BinaryToDecimal(str_temp);
				fwrite(&decimal, sizeof(unsigned char), 1, outfile);
				temp++;
			}

			count++;
			DeleteTree(HuffmanTree);

		}
	}
	std::fclose(outfile);
	return true;
}

bool DecompressFolder(string infilename, string path)
{
	//Tao thu muc
	if (_mkdir(path.c_str()) != 0)
		return false;
	FILE* infile;

	fopen_s(&infile, infilename.c_str(), "rb");
	if (infile == NULL)
		return false;
	int count; //So thu tu file duoc doc 
	int count2 = 1; //So thu tu file tao de so sanh
	//Doc so thu tu file
	fread(&count, sizeof(int), 1, infile);
	while (!feof(infile) && count == count2)
	{
		//Doc do dai ten file
		int length_name_file;
		fread(&length_name_file, sizeof(int), 1, infile);

		//Doc ten file
		string filename = "";
		for (int i = 0; i < length_name_file; i++)
		{
			char c;
			fread(&c, sizeof(char), 1, infile);
			filename += c;
		}

		//Doc bang tan so
		vector <char> character;
		vector <int> frequence;
		int size_frequence_table;

		fread(&size_frequence_table, sizeof(int), 1, infile);
		character.resize(size_frequence_table);
		frequence.resize(size_frequence_table);
		for (int i = 0; i < size_frequence_table; i++)
		{
			fread(&character[i], sizeof(char), 1, infile);
			fread(&frequence[i], sizeof(int), 1, infile);
		}

		//Doc chieu dai ma nhi phan da ma hoa file
		int length_str_code;
		fread(&length_str_code, sizeof(int), 1, infile);

		//Doc cac ma nhi phan da ma hoa file
		unsigned char c;
		string str_code = "";
		while (str_code.length() < length_str_code)
		{
			//Doc cac ky tu
			fread(&c, sizeof(unsigned char), 1, infile);
			//Chuyen thanh ma nhi phan
			str_code += DecimalToBinary(c);
		}

		//Xoa cac ky tu 0 thua o cuoi ma nhi phan ma hoa file
		str_code.erase(length_str_code);

		//Xay dung lai cay huffman
		Node* HuffmanTree = createHuffmanTree(character, frequence);
		if (HuffmanTree == NULL)
			return false;

		//Tao file luu trong duong dan thu muc o tren voi ten vua duoc doc tu file ma hoa
		FILE* outfile;
		string s_path = path + '\\' + filename;
		fopen_s(&outfile, s_path.c_str(), "wb");
		if (outfile == NULL)
			return false;

		//Duyet cac ma nhi phan tren cay de tim ky tu duoc ma hoa va dua vao file giai nen
		int i = 0;
		Node *p = HuffmanTree;
		while (i <= length_str_code)
		{

			if (p->left == NULL && p->right == NULL)
			{
				fwrite(&p->c, sizeof(char), 1, outfile);
				p = HuffmanTree;
				if (i == length_str_code)
					i++;
				continue;
			}
			if (str_code[i] == '0')
				p = p->left;
			else
				if (str_code[i] == '1')
					p = p->right;
			i++;
		}


		fclose(outfile);
		DeleteTree(HuffmanTree);

		//Doc so thu tu file tiep theo
		fread(&count, sizeof(int), 1, infile);
		count2++;
	}
	fclose(infile);
	return true;
}
