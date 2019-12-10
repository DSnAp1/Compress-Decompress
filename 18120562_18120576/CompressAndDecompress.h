#pragma once
#include "Huffman.h"
#include <thread>
namespace fs = std::experimental::filesystem;

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
void CreateFrequenceTable(string str, vector <char> &character, vector <int> &frequence);

//Chuyen doi chuoi nhi phan string (8 bit tuong ung 8 ky tu) thanh dang thap phan
unsigned char BinaryToDecimal(string str);

//Chuyen doi so thap phan c thanh dang chuoi nhi phan (8 bit)
string DecimalToBinary(unsigned char c);

//Doc du lieu tu file co duong dan la path va tra ve chuoi ghi du lieu vua doc
string ReadFile(string path);

//Nen cac file trong filePath vao file co ten compressFile
//Tra ve: true neu nen thanh cong, false neu nguoc lai
bool CompressFile(string compressPath, vector<string>filePath, int indexFileName);

//Giai nen file da ma hoa trong path
//Tra ve: true neu giai nen thanh cong, false neu nguoc lai
bool DecompressFile(string path, string decompressPath, int pos);

//Nen thu muc trong folderPath vao file compressPath
//Tra ve: true neu nen thanh cong, false neu nguoc lai
bool CompressFolder(string compressPath, vector<string>folderPath, int indexFileName);

//Giai nen cac thu muc trong path
//Tra ve: true neu giai nen thanh cong, false neu nguoc lai
int DecompressFolder(string path, string decompressPath);

// Lay tat ca cac duong dan bao gom folder va file
void GetAllPath(string path, vector<string>&filePath, vector<string>&folderPath);

// Lay ten file hoac folder trong duong dan, indexFileName la vi tri bat dau
string GetFileOrFolderName(string path, int indexFileName);

// Nen
bool Compress(string path, string compressPath);

// Giai nen
bool Decompress(string path, string decompressPath);

void FindFrequence(string str, vector<int> &hash);

void Serialize(string binaryCode, vector<unsigned char>&decimalCode);

void GetBinaryCode(string str, vector<string> hash, string& strCode);