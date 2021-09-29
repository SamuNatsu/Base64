/**
 * MIT License
 * 
 * Copyright (c) 2021 SamuNatsu(https://github.com/SamuNatsu)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#ifndef BASE64_HPP_INCLUDED
#define BASE64_HPP_INCLUDED

#include <string>

class Base64 {
    public:
        Base64() = delete;
        Base64(const Base64&) = delete;
        ~Base64() = delete;

        Base64& operator=(const Base64&) = delete;

        // Convert a plain string to Base64 string
        static std::string to(const std::string& src) {
            static const char* s_Map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            unsigned int tmp = 0;
            std::string rtn;
            for (size_t i = 0; i < src.length(); ++i) {
                if (i && i % 3 == 0) {
                    for (int j = 18; j >= 0; j -= 6)
                        rtn += s_Map[(tmp >> j) & 0x3F];
                    tmp = 0;
                }
                tmp <<= 8;
                tmp |= src[i];
            }
            if (tmp) {
                for (size_t i = src.length(); i % 3; ++i)
                    tmp <<= 8;
                rtn += s_Map[tmp >> 18];
                rtn += s_Map[(tmp >> 12) & 0x3F];
                rtn += ((tmp >> 6) & 0x3F) || (tmp & 0x3F) ? s_Map[(tmp >> 6) & 0x3F] : '=';
                rtn += (tmp & 0x3F) ? s_Map[tmp & 0x3F] : '=';
            }
            return rtn;
        }

        // Convert a Base64 string to plain string, returns a pair(status code, result), 0 for success, -1 for invalid character, -2 for invalid end
        static std::pair<int, std::string> from(const std::string& src) {
            unsigned int tmp = 0;
            std::string rtn;
            size_t i = 0;
            for (; i < src.length() && src[i] != '='; ++i) {
                if (!isalpha(src[i]) && !isdigit(src[i]) && src[i] != '+' && src[i] != '/')
                    return {-1, ""};
                if (i && i % 4 == 0) {
                    for (int j = 16; j >= 0; j -= 8)
                        rtn += (unsigned char)((tmp >> j) & 0xFF);
                    tmp = 0;
                }
                tmp <<= 6;
                if (isupper(src[i]))
                    tmp |= src[i] - 65;
                else if (islower(src[i]))
                    tmp |= src[i] - 71;
                else if (isdigit(src[i]))
                    tmp |= src[i] + 4;
                else if (src[i] == '+')
                    tmp |= 62;
                else 
                    tmp |= 63;
            }
            if (tmp) {
                for (; i < src.length() && i % 4; ++i)
                    if (src[i] != '=')
                        return {-2, ""};
                    else 
                        tmp <<= 6;
                if (i % 4)
                    return {-2, ""};
                for (int j = 16; j >= 0; j -= 8)
                    rtn += (unsigned char)((tmp >> j) & 0xFF);
            }
            return {0, rtn};
        }
};

#endif
