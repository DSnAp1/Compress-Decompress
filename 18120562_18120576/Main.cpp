#include "Huffman.h"
#include "CompressAndDecompress.h";
int main()

{
	
	
	CompressStringFile("in.txt","out.th");
	DecompressStringFile("out.th", "Out.txt");
	//cout << CompressBinaryFile("in.txt", "out.b");
	//DecompressBinaryFile("out.b", "out.txt");
	//string str = ReadFileBinary("18120562_18120576.exe");
	//cout << str;
	system("pause");
	return 0;
}