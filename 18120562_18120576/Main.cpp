#include "Huffman.h"
#include "CompressAndDecompress.h";
int main()

{
	CompressStringFile("in.txt","out.th");
	DecompressStringFile("out.th", "Out.txt");
	system("pause");
	return 0;
}