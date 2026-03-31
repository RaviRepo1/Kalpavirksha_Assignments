#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* decodeString(char* s) {
    char *strStack[1000], *numStack[1000];
    int strTop = -1, numTop = -1;

    char *curr = (char*)calloc(2000, sizeof(char));  
    int num = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];

        if (isdigit(c)) {
            num = num * 10 + (c - '0');  
        }

        else if (c == '[') {
            numStack[++numTop] = (char*)(long)num;  
            strStack[++strTop] = curr;
            curr = (char*)calloc(2000, sizeof(char));
            num = 0;
        }

        else if (c == ']') {
            int repeat = (int)(long)numStack[numTop--];  
            char *prev = strStack[strTop--];

            char *temp = (char*)calloc(2000, sizeof(char));
            strcpy(temp, prev);

            while (repeat--) strcat(temp, curr);

            free(curr);
            curr = temp;
        }

        else {
            int len = strlen(curr);
            curr[len] = c;
            curr[len + 1] = '\0';
        }
    }

    return curr;  
}

int main() {
    char s[1000];
    scanf("%s", s);

    char* result = decodeString(s);
    printf("%s\n", result);

    free(result);
    return 0;
}
