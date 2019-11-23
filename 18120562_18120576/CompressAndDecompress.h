#pragma once
#include "Huffman.h"

//Tim va tra ve vi tri cua c trong character
//Neu khong tim thay tra ve -1
int findElementOfVector(vector <char> character, char c);

template <typename T>
//Hoan vi a va b
void mySwap(T& a, T&b);

//Tao bang tan so tu chuoi du lieu (str) doc tu file
//character: cac ky tu trong bang tan so
//frequence: tan so tuong ung
//Bang tan so duoc sap xep tang dan theo tan so
bool createFrequenceTable(string str, vector <char> &character, vector <int> &frequence);

//Chuyen doi chuoi nhi phan string (8 bit tuong ung 8 ky tu) thanh dang thap phan
unsigned char BinaryToDecimal(string str);

//Chuyen doi so thap phan c thanh dang chuoi nhi phan (8 bit)
string DecimalToBinary(unsigned char c);

//Doc du lieu tu file co ten la filename va tra ve chuoi ghi du lieu vua doc
string ReadFile(string filename);

//Nen file co ten infilename vao file co ten outfilename
//Tra ve: true neu nen thanh cong, false neu nguoc lai
bool CompressFile(string infilename, string outfilename);

//Giai nen file co ten infilename vao file co ten outfilename
//Tra ve: true neu giai nen thanh cong, false neu nguoc lai
bool DecompressFile(string infilename, string outfilename);

//Nen thu muc co duong dan (ten) la path vao file co ten (duong dan) la outfilename
//Tra ve: true neu nen thanh cong, false neu nguoc lai
bool CompressFolder(string path, string outfilename);

//Giai nen file (file nay dang nen thu muc) co ten la infilename vao thu muc co duong dan (ten) la path (thu muc nay chua duoc tao)
//Tra ve: true neu giai nen thanh cong, false neu nguoc lai
bool DecompressFolder(string infilename, string path);