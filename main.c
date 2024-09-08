#include <stdio.h>

#define FMT_IMPL
#include "fmt.h"

int main() {
    // fmt("%s\n", "Hello, Sailor!");
    int b = 69;
    fmt("%i =\n= %x = %X = %#x = %#X =\n= %o = %#o =\n= %b = %#b\n", b,b,b,b,b,b,b,b,b);

    // printf("%.10f\n", 123.456);
    return 0;
}
