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

string ReadFileString(string filename)
{
	ifstream infile;
	infile.open(filename, ios::in);
	if (infile.fail())
	{
		return "";
	}
	string temp;
	stringstream ss;

	while (!infile.eof())
	{
		getline(infile, temp);
		ss << temp << "\n";
	}
	temp = ss.str();
	temp.erase(temp.length() - 1);
	infile.close();
	return temp;
}

bool createFrequenceTable(string str, vector <char> &character, vector <int> &frequence)
{
	if (str.empty())
		return false;
	for (int i = 0; i < str.length(); i++)
	{
		if (findElementOfVector(character, str[i]) == -1)
		{
			character.push_back(str[i]);
			frequence.push_back(1);
		}
		else
			frequence[findElementOfVector(character, str[i])]++;
	}

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

bool CompressStringFile(string infilename, string outfilename)
{
	//Bang tan so
	vector <char> character; //Cac ky tu trong bang 
	vector <int> frequence; //Tan so tuong ung

	string str = ReadFileString(infilename);
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

		for (int i = 0; i < CodeTable.size(); i++)
		{
			cout << CodeTable[i].c << ":" << CodeTable[i].code << endl;
		}
		
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

bool DecompressStringFile(string infilename, string outfilename)
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
	fopen_s(&outfile, outfilename.c_str(), "w");
	if (outfile == NULL)
		return false;

	//Duyet cac ma nhi phan tren cay de tim ky tu duoc ma hoa va dua vao file giai nen
	int i = 0;
	Node *p = HuffmanTree;
	while (i <= length_str_code)
	{
		
		if (p->left == NULL && p->right == NULL)
		{
			fprintf(outfile, "%c", p->c);
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


string ReadFileBinary(string filename)
{
	FILE* f = NULL;
	fopen_s(&f, filename.c_str(), "rb");
	
	if (f == NULL)
		return "";
	string temp = "";
	char c;
	while (!feof(f))
	{
		fread(&c, sizeof(char), 1, f);
		temp += c;
	}
	return temp;
}

bool CompressBinaryFile(string infilename, string outfilename)
{
	//Bang tan so
	vector <char> character; //Cac ky tu trong bang 
	vector <int> frequence; //Tan so tuong ung

	string str = ReadFileBinary(infilename);
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
		/*
		for (int i = 0; i < CodeTable.size(); i++)
		{
			cout << CodeTable[i].c << ":" << CodeTable[i].code << endl;
		}*/

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

bool DecompressBinaryFile(string infilename, string outfilename)
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

