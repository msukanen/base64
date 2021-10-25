// base64.h : Include file for standard system include files,
// or project specific include files.
#pragma once

#include <vector>

extern std::vector<char> base64_encode(std::vector<char> data);
extern std::vector<char> base64_decode(const std::vector<char>& src);
