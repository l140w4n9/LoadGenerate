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
{