#pragma once
#include "Huffman.h"

//Tim va tra ve vi tri cua c trong character
//Neu khong tim thay tra ve -1
int findElementOfVector(vector <char> character, char c);

template <typename T>
//Hoan vi a va b
void mySwap(T& a, T&b);

//Doc du lieu tu file chua chuoi co ten la filename va tra ve chuoi ghi du lieu vua doc
string ReadFileString(string filename);
bool createFrequenceTable(string str, vector <char> &character, vector <int> &frequence);


unsigned char BinaryToDecimal(string str);
string DecimalToBinary(unsigned char c);
bool CompressStringFile(string infilename, string outfilename);
bool DecompressStringFile(string infilename, string outfilename);

string ReadFileBinary(string filename);
bool CompressBinaryFile(string infilename, string outfilename);
bool DecompressBinaryFile(string infilename, string outfilename);
