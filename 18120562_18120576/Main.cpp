#include "Huffman.h"
#include "CompressAndDecompress.h"

int main()

{
	cout << CompressFolder("E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\18120562_18120576\\temp",
		"E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\18120562_18120576\\temp.folderzip");
	cout << DecompressFolder("E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\18120562_18120576\\temp.folderzip",
		"E:\\Nam 2\\TH CTDL\\GitHub\\Compress-Decompress\\18120562_18120576\\temp2");
	system("pause");
	return 0;
}