#include <stdio.h>
#include "vfs.h"
#include "vfs.c"

int main() {
    initializeFileSystem();
    while (1) {
        takeInput();
    }
    return 0;
}
