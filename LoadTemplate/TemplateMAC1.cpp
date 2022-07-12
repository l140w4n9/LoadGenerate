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
{