#include "LZW.h"


vector<int> LZW::Compress(string data)
{
		// Create dictionary
		map<string, int> dic;

		string str;
		for (int i = 0; i < 256; i++)
		{
				str = "";
				str += char(i);

				dic.insert(pair<string, int>(str, i));
		}

		vector<int> code;

		string buffer = "", nextChar = "";
		buffer += data[0];

		int pos = 256;

		for (int i = 1; i < data.length(); i++)
		{
				nextChar += data[i];

				if (dic.find(buffer + nextChar) != dic.end())
				{
						buffer += nextChar;
				}
				else
				{
						code.push_back(dic[buffer]);
						dic.insert(pair<string, int>(buffer + nextChar, pos++));

						buffer = nextChar;
				}

				nextChar = "";
		}

		code.push_back(dic[buffer]);

		return code;
}

string LZW::Decompress(vector<int> code)
{
		// Create dictionary
		map<int, string> dic;

		string str;
		for (int i = 0; i < 256; i++)
		{
				str = "";
				str += char(i);

				dic.insert(pair<int, string>(i, str));
		}

		string result = "";

		int oldCode = code[0];

		string buffer = dic[oldCode];
		string c = "";
		c += buffer[0];

		result += buffer;

		int pos = 256;

		for (int i = 1; i < code.size(); i++)
		{
				int newCode = code[i];

				if (dic.find(newCode) != dic.end())
				{
						buffer = dic[newCode];
				}
				else
				{
						buffer = dic[oldCode];
						buffer += c;
				}

				result += buffer;
				c = "";
				c += buffer[0];
				dic.insert(pair<int, string>(pos++, dic[oldCode]+ c));

				oldCode = newCode;
		}

		for (int i = 0; i < dic.size(); i++)
		{
				cout << dic[i] << endl;
		}

		return result;
}
