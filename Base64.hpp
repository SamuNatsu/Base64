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
            for (size_t i = 0; i < src.length(); i += 3) {
                tmp = ((unsigned int)src[i] & 0xFF) << 16;
                tmp |= (i + 1 >= src.length() ? 0 : ((unsigned int)src[i + 1] & 0xFF) << 8);
                tmp |= (i + 2 >= src.length() ? 0 : (unsigned int)src[i + 2] & 0xFF);
                rtn += s_Map[tmp >> 18 & 0x3F];
                rtn += s_Map[tmp >> 12 & 0x3F];
                rtn += s_Map[tmp >> 6 & 0x3F];
                rtn += s_Map[tmp & 0x3F];
            }
            if (src.length() % 3 == 1)
                rtn[rtn.length() - 1] = rtn[rtn.length() - 2] = '=';
            else if (src.length() % 3 == 2)
                rtn[rtn.length() - 1] = '=';
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
                    rtn += (unsigned char)(tmp >> 16 & 0xFF);
                    rtn += (unsigned char)(tmp >> 8 & 0xFF);
                    rtn += (unsigned char)(tmp & 0xFF);
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
            if (i == src.length()) {
                if (src.length() % 4)
                    return {-2, ""};
                rtn += (unsigned char)(tmp >> 16 & 0xFF);
                rtn += (unsigned char)(tmp >> 8 & 0xFF);
                rtn += (unsigned char)(tmp & 0xFF);
            }
            else {
                if (i % 4 == 0) {
                    rtn += (unsigned char)(tmp >> 16 & 0xFF);
                    rtn += (unsigned char)(tmp >> 8 & 0xFF);
                    rtn += (unsigned char)(tmp & 0xFF);
                }
                else if (i % 4 == 1)
                    return {-2, ""};
                else if (i % 4 == 2) {
                    if (i + 1 >= src.length() || src[i] != '=' || src[i + 1] != '=')
                        return {-2, ""};
                    rtn += (unsigned char)(tmp >> 4 & 0xFF);
                }
                else if (i % 4 == 3) {
                    if (src[i] != '=')
                        return {-2, ""};
                    rtn += (unsigned char)(tmp >> 10 & 0xFF);
                    rtn += (unsigned char)(tmp >> 2 & 0xFF);
                }
            }
            return {0, rtn};
        }
};

#endif
