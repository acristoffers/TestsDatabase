#include <fstream>
#include <iostream>
#include <cstdlib>

#include "bzip/bzlib.h"

int main (int argc, char const *argv[])
{
    if ( argc < 1 )
        return 0;
    
    std::fstream file;
    file.open(argv[1], std::ios::binary | std::ios::in | std::ios::out | std::ios::ate );
    
    if ( file.is_open() ) {
        unsigned int srcLen = file.tellg();
        
        char* source = (char*)malloc(srcLen);
        
        file.seekg(0, std::ios::beg);
        file.read(source, srcLen);
        file.close();
        
        unsigned int destLen = ((unsigned int)(srcLen * 1.01)) + 600;
        char* dest = (char*)malloc(destLen + 8);
        
        dest += 8;
        
        BZ2_bzBuffToBuffCompress(dest, &destLen, source, srcLen, 9, 0, 30);
        
        dest -= 8;
        
        // compressed length
        dest[0] = (destLen & 0xff000000) >> 24;
        dest[1] = (destLen & 0x00ff0000) >> 16;
        dest[2] = (destLen & 0x0000ff00) >> 8;
        dest[3] = (destLen & 0x000000ff);
        // uncompressed length
        dest[4] = (srcLen & 0xff000000) >> 24;
        dest[5] = (srcLen & 0x00ff0000) >> 16;
        dest[6] = (srcLen & 0x0000ff00) >> 8;
        dest[7] = (srcLen & 0x000000ff);
        
        std::cout << std::string(dest, destLen+8);
    }
    
    return 0;
}
