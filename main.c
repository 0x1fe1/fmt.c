#include <stdio.h>

#define FMT_IMPL
#include "fmt.h"

int main() {
    fmt("%s %d %.5d\n", "World", 34+35, 21 * 20);
    return 0;
}
