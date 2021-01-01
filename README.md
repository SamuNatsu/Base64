# Base64
One header base64 convertor

# How to use
```cpp
#include <cstdio>

// You SHOULD define it ONLY ONCE in any source file before the header
#define BASE64_IMPLEMENTATION

// This is the only header you need
#include "Base64.hpp"

int main() {
    std::string src = "Hello World!";
    
    // Convert multichars to base64 string
    std::string b64 = Base64::Convertor::StringToBase64(src);
    
    // Convert base64 string to multichars
    std::string ans = Base64::Convertor::Base64ToString(b64);
    
    printf("%s\n\n%s\n", b64.c_str(), ans.c_str());
    
    return 0;
}
```
