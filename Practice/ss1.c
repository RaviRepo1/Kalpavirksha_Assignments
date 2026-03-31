#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
int arr[MAX_SIZE];
int top;
} Stack;

// Initialize stack
void initStack(Stack* s) {
s->top = -1;
}

// Push page onto stack
void push(Stack* s, int page) {
if(s->top < MAX_SIZE - 1) {
s->arr[++(s->top)] = page;
}
}

// Pop page from stack
void pop(Stack* s) {
if(s->top >= 0) {
s->top--;
}
}

// Print stack in ascending order
void list(Stack* s) {
if(s->top < 0) {
printf("No bookmarks\n");
return;
}


int temp[MAX_SIZE];
for(int i = 0; i <= s->top; i++) temp[i] = s->arr[i];

// Sort ascending
for(int i = 0; i <= s->top; i++) {
    for(int j = i + 1; j <= s->top; j++) {
        if(temp[i] > temp[j]) {
            int x = temp[i]; temp[i] = temp[j]; temp[j] = x;
        }
    }
}

for(int i = 0; i <= s->top; i++) printf("%d ", temp[i]);
printf("\n");


}

int main() {
int n;
scanf("%d", &n);


Stack s;
initStack(&s);

while(n--) {
    char cmd[20];
    scanf("%s", cmd);

    if(strcmp(cmd, "BOOKMARK") == 0) {
        int page;
        scanf("%d", &page);
        push(&s, page);
    }
    else if(strcmp(cmd, "DELETE") == 0) {
        pop(&s);
    }
    else if(strcmp(cmd, "LIST") == 0) {
        list(&s);
    }
}

return 0;


}