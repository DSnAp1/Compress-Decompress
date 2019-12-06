#include "Huffman.h"


Node* createHuffmanTree(vector <char> character, vector <int> frequence)
{
	vector <Node*> Tree; //Danh sach cac node la trong cay
		
	if (character.size() == 0)
			return NULL;

	//Tao cac node tuong ung cac ky tu 
	for (int i = 0; i < character.size(); i++)
	{
		Node *p = new Node;
		p->c = character[i];
		p->frequence = frequence[i];
		p->left = NULL;
		p->right = NULL;
		Tree.push_back(p);
	}

	
	while (Tree.size() > 1)
	{
		//Ket hop 2 node co tan so thap nhat (2 phan tu dau tien cua bang tan so) thanh node moi co 2 con la 2 node cu
		Node *p = new Node;
		p->c = ' ';
		p->frequence = Tree[0]->frequence + Tree[1]->frequence;
		p->left = Tree[0];
		p->right = Tree[1];

		//Xoa 2 phan tu dau trong Tree (2 phan tu da duoc ket hop)
		Tree.erase(Tree.begin(), Tree.begin() + 2);

		//Chen node moi tao vao Tree sao cho Tree van theo thu tu tang dan theo tan so 
		Tree.push_back(p);
		for (int i = 0; i < Tree.size() - 1; i++)
		{
			int min = i;
			for (int j = i + 1; j < Tree.size(); j++)
				if (Tree[j]->frequence <= Tree[min]->frequence)
					min = j;
			Node* temp = Tree[i];
			Tree[i] = Tree[min];
			Tree[min] = temp;
		}
		//Tiep tuc cho den khi chi con 1 node trong Tree
		//Do la node goc cua cay huffman
	}
	
	return Tree[0];
}

void createHuffmanCodeTable(Node* Tree, vector <HuffmanCode> &Table, string code)
{
		if (Tree == NULL)
				return;

	if (Tree->left != NULL || Tree->right != NULL)
	{
		//Neu qua trai thi them 0 vao ma
		createHuffmanCodeTable(Tree->left, Table, code + "0");
		//Neu qua phai thi them 1 vao ma
		createHuffmanCodeTable(Tree->right, Table, code + "1");
	}
	else
	{
		//Neu la node la thi ghi ky tu cung ma tuong ung vao bang ma	
		HuffmanCode temp;
		temp.c = Tree->c;
		temp.code = code;
		temp.frequence = Tree->frequence;
		Table.push_back(temp);
	}
}

void DeleteTree(Node* Tree)
{
	if (Tree == NULL)
		return;
	DeleteTree(Tree->left);
	DeleteTree(Tree->right);
	delete Tree;
}

