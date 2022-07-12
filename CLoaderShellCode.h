#pragma once
#include <string>
#include <fstream>
#include <map>

using namespace std;

class CLoaderShellCode
{
public:
	CLoaderShellCode();		//构造
	~CLoaderShellCode();	//析构

	string& GetShellCode();							//获取shellcode
	bool FileToLoadCode(const char * szFileName);	//从文件加载shellcode

	bool CreateMAC();								//创建MAC加载器
	bool CreateUUID();								//创建UUID加载器
	bool CreateRMM();								//创建RMM加载器
	bool CreateREG();								//创建REG加载器

private:
	bool Isbase64(unsigned char c);					//判断是否是Base64编码
	bool IsLoad();									//判断是否再加shellcode
	bool CreateTmp(const string strTmpName);			//创建模板

	void Encode();									//xor_base编码
	void XOREncode();								//chellocode异或加密
	void XORDncode();								//chellocode异或解密
	void HexToString();								//shellocode变字符串
	void StringToHex();								//字符串变shellocode
	void Base64Encode();							//Base64编码shellcode
	void Base64Decode();							//Base64解码码shellcode
	void RestoreShellCode();							//Base64解码码shellcode

	map<int, string> UUIDEncode();					//UUID编码shellcode
	map<int, string> MACEncode();					//MAC编码shellcode

	string LoadTemplate();							//加载模板

private:
	string m_strShellCode;
	map<int, string> m_listTmp;
};

