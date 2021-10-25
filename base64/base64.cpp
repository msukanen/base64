// base64.cpp : Defines the entry point for the application.
//

#include "base64.h"

#include <iostream>
#include <cstring>

#define SZ_T(x) static_cast<std::vector<char, std::allocator<char>>::size_type>(x)

using namespace std;

static const char* BASE64_SET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const string BASE64_STR = string(BASE64_SET);

vector<char> base64_encode(vector<char> data) {
    vector<char> pad;
    switch (data.size() % 3) {
        case 1: pad.push_back('=');
        case 2: pad.push_back('=');
    }

    for (char x : pad)
        data.push_back('\0');

    vector<char> out;
    for (int i = 0, s = data.size(); i < s; i += 3) {
        uint32_t n
            = ((0x00FF & (uint32_t)data[i]) << 16)
            + ((0x00FF & (uint32_t)data[SZ_T(i) + 1]) << 8)
            + ((0x00FF & (uint32_t)data[SZ_T(i) + 2]));
        out.push_back(BASE64_SET[(n >> 18) & 0x3F]);
        out.push_back(BASE64_SET[(n >> 12) & 0x3F]);
        out.push_back(BASE64_SET[(n >> 6) & 0x3F]);
        out.push_back(BASE64_SET[n & 0x3F]);
    }

    for (char x : pad)
        out.pop_back();
    for (char x : pad)
        out.push_back(x);
    
    return out;
}

vector<char> base64_decode(const vector<char>& src) {
    if (src.size() % 4 != 0)
        throw 42;//IllegalArgumentException("Data ${ src } does not comply with BASE64 length requirement.")
    
    vector<char> data;
    int padlen = 0;
    for (char x : src) {
        if (strchr(BASE64_SET, x))
            data.push_back(x);
        else if (x == '=') {
            padlen++;
            data.push_back('A');
        }
    }
    vector<char> out;
    for (int i = 0, s = data.size(); i < s; i += 4) {
        uint32_t v
            = (BASE64_STR.find(data[i]) << 18)
            + (BASE64_STR.find(data[SZ_T(i) + 1]) << 12)
            + (BASE64_STR.find(data[SZ_T(i) + 2]) << 6)
            + BASE64_STR.find(data[SZ_T(i) + 3]);
        out.push_back(0xFF & (v >> 16));
        out.push_back(0xFF & (v >> 8));
        out.push_back(0xFF & v);
    }
    for (int i = 0; i < padlen; i++)
        out.pop_back();
    
    return out;
}

int main()
{
    string test = "This is a brown fox. What does the fox say? Bip-blibi-bibi-blibibi-bli!";
	cout << "nobase: \"" << test << "\"" << endl;
    
    vector<char> base64 = base64_encode(vector<char>(test.begin(), test.end()));
    cout << "base64: \"" << string(base64.begin(), base64.end()) << "\"" << endl;
    
    vector<char> deco64 = base64_decode(base64);
    cout << "64base: \"" << string(deco64.begin(), deco64.end()) << "\"" << endl;
	return 0;
}
