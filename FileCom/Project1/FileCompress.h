#include"HuffmanTree.h"

class FileCompress
{
public:
	FileCompress()
	{}
	bool Compress(const char * filename)
	{
		static int chCountBuff[256] = { 0 };
		memset(chCountBuff, 0, 256);

		//次数统计
		InitCountArr(filename, chCountBuff);

		//EOF获取
		HuffmanTreeNode* pEOF = _ht.SetHuffmanTree(chCountBuff);
		//保存有效编码
		_EOF = _ht.GetCode(pEOF);
		for (int i = 0; i < _ht._leafVec.size(); ++i)
		{
			_code[_ht._leafVec[i]->_data] = _ht.GetCode(_ht._leafVec[i]);
		}

		//配置文件保存
		string configName = filename;
		configName += "config";
		FILE* ofsConfig = fopen(configName.c_str(), "wb");
		for (int i = 0; i < _ht._leafVec.size(); ++i)
		{
			fputc(_ht._leafVec[i]->_data, ofsConfig);
			fputc(',', ofsConfig);
			fprintf(ofsConfig, "%d\n", (chCountBuff[_ht._leafVec[i]->_data]));
		}
		fclose(ofsConfig);

		//压缩文件开始
		string outFile = filename;
		outFile += ".Compress";

		unsigned char codeBuff = 0;
		FILE* ofs = fopen(outFile.c_str(), "wb");


		FILE* ifs = fopen(filename, "rb");
		unsigned char c = fgetc(ifs);
		
		int bytes = 8;
		while (!feof(ifs)) {
			///编码压入
			for (int i = 0; i < _code[c].size(); ++i)
			{
				--bytes;
				if (_code[c][i] == '1')
					codeBuff |= (1<<bytes);

				if (bytes == 0)
				{
					//写入codeBuff
					fputc(codeBuff, ofs);
					codeBuff = 0;
					bytes = 8;
				}
			}
			c = fgetc(ifs);
		}
		//写入EOF
		for (int i = 0; i < _EOF.size(); ++i)
		{
			--bytes;
			if (_EOF[i] == '1')
				codeBuff |= (1 << bytes);

			if (bytes == 0)
			{
				//写入codeBuff
				fputc(codeBuff, ofs);
				codeBuff = 0;
				bytes = 8;
			}
		}
		fputc(codeBuff, ofs);

//		Print();
//		_ht.Show();
		_ht.clear();
		_ht._leafVec.resize(0);
		fclose(ifs);
		fclose(ofs);
		return true;
	}

	static bool UnCompress(const char *filename)
	{
		static int chCountBuff[256] = { 0 };
		memset(chCountBuff, 0, 256);

		//读取文件，重建HuffmanTree
		string fileConfig = filename;
		fileConfig += "config";
		ifstream ifsConfig(fileConfig, ios::in);
		unsigned char chIn = ifsConfig.get();
		while (ifsConfig.good())
		{
			unsigned char c = ifsConfig.get();
			ifsConfig >> chCountBuff[chIn];
			c = ifsConfig.get();
			chIn = ifsConfig.get();
		}
		ifsConfig.close();
		//EOF获取
		HuffmanTreeNode* pEOF = _ht.SetHuffmanTree(chCountBuff);
		for (int i = 0; i < _ht._leafVec.size(); ++i)
		{
			_code[_ht._leafVec[i]->_data] = _ht.GetCode(_ht._leafVec[i]);
		}

	//	Print();
	//	_ht.Show();
		//解码
		string UnCompressFile = filename;
		UnCompressFile += ".Compress";

		FILE* ifs = fopen(UnCompressFile.c_str(), "rb");

		//

		string finallyFile = filename;
		finallyFile += "final";
		//ofstream ofs(finallyFile, ios::out|ios::binary);
		FILE* ofs = fopen(finallyFile.c_str(), "wb");
		
		unsigned char codeBuff = 0;
		//读取一个字符
		//codeBuff = ifs.get();
		codeBuff = fgetc(ifs);
		int bytes = 8;

		HuffmanTreeNode* cur = _ht.getRoot();

		while (1) 
		{
			--bytes;

			//如果是叶子结点
			if (cur->_left == cur->_right&& cur->_left == NULL)
			{
				if (cur == pEOF)
				{
					break;
				}
				fputc(cur->_data, ofs);
			//	cout << cur->_data;
				cur = _ht.getRoot();
			}

			if ((codeBuff&(1 << bytes)) == 0)
			{
				//左移
				cur = cur->_left;
			}
			else
			{
				//右
				cur = cur->_right;
			}

			if (bytes == 0)
			{
				//	codeBuff = 0;
				codeBuff = fgetc(ifs);
				bytes = 8;
			}
		}

		_ht._leafVec.resize(0);
		_ht.clear();
		_code.clear();

		fclose(ifs);
		fclose(ofs);
		return true;
	}

protected:

	//文件中字符次数统计
	static bool InitCountArr(const char* filename, int *chCount)
	{
		//记录次数
		FILE* ifs = fopen(filename, "rb");
		unsigned char c = fgetc(ifs);
		while (!feof(ifs)) {
			chCount[c]++;
			c = fgetc(ifs);
		}
		fclose(ifs);
		return true;
	}


	static void Print()
	{
		for (int i = 0; i < _ht._leafVec.size(); ++i)
		{
			cout << "[" << _ht._leafVec[i]->_data << "]" << _code[_ht._leafVec[i]->_data].c_str() << endl;
		}
		cout << "[EOF]" << _EOF.c_str() << endl;
	}
private:
	static HuffmanTree _ht;
	static hash_map<unsigned char, string> _code;
	static string _EOF;
};
HuffmanTree FileCompress::_ht = HuffmanTree();
hash_map<unsigned char, string> FileCompress::_code = hash_map<unsigned char, string>();
string FileCompress::_EOF = string();