#include <stdio.h>

#define FMT_IMPL
#include "fmt.h"

int main() {
    int b = 255;
    fmt("%u = %b\n", b, b);
    return 0;
}
