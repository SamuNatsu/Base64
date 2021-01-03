/**
 * MIT License
 * 
 * Copyright (c) 2020 SamuNatsu
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

#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED

#include <string>

namespace Base64 {

std::string StringToBase64(const std::string&);
std::string StringToBase64(std::string&&);

std::string Base64ToString(const std::string&);
std::string Base64ToString(std::string&&);

}

#endif

#ifdef BASE64_IMPLEMENTATION

namespace Base64 {

std::string StringToBase64(const std::string& str) {
    static const char mapping[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
        'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', '+', '/'};
    std::string _rtn;
    unsigned int _t = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        if (i && i % 3 == 0) {
            _rtn += mapping[_t >> 18];
            _rtn += mapping[(_t & 0x3FFFF) >> 12];
            _rtn += mapping[(_t & 0xFFF) >> 6];
            _rtn += mapping[_t & 0x3F];
            _t = 0;
        }
        _t = _t << 8 | str[i];
    }
    if (_t) {
        for (size_t i = str.length(); i % 3; ++i)
            _t <<= 8;
        _rtn += mapping[_t >> 18];
        _rtn += mapping[(_t & 0x3FFFF) >> 12];
        _rtn += ((_t & 0xFFF) >> 6) || (_t & 0x3F) ? mapping[(_t & 0xFFF) >> 6] : '=';
        _rtn += (_t & 0x3F) ? mapping[_t & 0x3F] : '=';
    }
    return _rtn;
}
std::string StringToBase64(std::string&& str) {
    return StringToBase64(str);
}

std::string Base64ToString(const std::string& str) {
    std::string _rtn;
    size_t i;
    unsigned int _t = 0;
    for (i = 0; str[i] != '=' && i < str.length(); ++i) {
        if (i && i % 4 == 0) {
            _rtn += (unsigned char)(_t >> 16);
            _rtn += (unsigned char)((_t & 0xFFFF) >> 8);
            _rtn += (unsigned char)(_t & 0xFF);
            _t = 0;
        }
        _t = _t << 6 | [](char tmp)->unsigned int {
            if (isupper(tmp)) return tmp - 65;
            if (islower(tmp)) return tmp - 71;
            if (isdigit(tmp)) return tmp + 4;
            if (tmp == '+') return 62;
            if (tmp == '/') return 63;
            return 0;}(str[i]);
    }
    if (_t) {
        for (; i % 4; ++i)
            _t <<= 6;
        _rtn += (unsigned char)(_t >> 16);
        _rtn += (unsigned char)((_t & 0xFFFF) >> 8);
        _rtn += (unsigned char)(_t & 0xFF);
    }
    return _rtn;
}
std::string Base64ToString(std::string&& str) {
    return Base64ToString(str);
}

}

#endif
