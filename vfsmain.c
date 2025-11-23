#include <stdio.h>
#include "vfs.h"
#include "vfs.c"
int main() {
    FileSystem *fsState = initializeFileSystem();
    if (!fsState) return 1;

    while (1) {
        takeInput(fsState);
    }
    return 0;
}
