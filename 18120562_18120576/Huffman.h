#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <filesystem>
#include <string>
using namespace std;


//Cau truc node cay huffman
typedef struct Node
{
	char c;
	int frequence;
	Node* left;
	Node* right;
};

//Cau truc 1 phan tu cua bang ma huffman
typedef struct HuffmanCode
{
	char c;
	string code;
	int frequence;
};


//Tao cay huffman voi bang tan so truyen vao
//character: cac ky tu trong bang tan so
//frequence: tan so tuong ung
//Bang tan so da duoc sap xep tang dan theo tan so
Node* createHuffmanTree(vector <char> character, vector <int> frequence);

//Tao bang ma huffman (Table) tu cay huffman (Tree), code la chuoi tam
void createHuffmanCodeTable(Node* Tree, vector <HuffmanCode> &Table, string code);

//Xoa cay
void DeleteTree(Node* Tree);