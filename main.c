#include <stdio.h>

#define FMT_IMPL
#include "fmt.h"

int main() {
    long foo = 1;
    printf("%s %d %.5d %lu\n", "World", 34+35, 21 * 20, foo);
    ffmt(stdout, "%s %d %.5d %lu\n", "World", 34+35, 21 * 20, foo);
    return 0;
}
