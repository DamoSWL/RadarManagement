#ifndef _BASE64_20150312_H_
#define _BASE64_20150312_H_

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif