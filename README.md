# Base64
One header base64 convertor

# How to use
```cpp
#include <string>
#include <iostream>

// This is the only header you need
#include "Base64.hpp"

int main() {
    std::string src = "Hello World!";
    
    // Convert plain string to Base64 string
    std::string b64 = Base64::to(src);
    
    // Convert Base64 string to plain string
    std::string _src = Base64::from(b64);
    
    std::cout << "src: " << src << "\nb64: " << b64 << "\n_src: " << _src << std::endl;
    
    return 0;
}
```
