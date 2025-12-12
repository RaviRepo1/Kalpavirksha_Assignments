#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char item[50];
    int qty;
} Item;

void addItem(Item *arr[], int *size, char name[], int q) {
    for (int i = 0; i < *size; i++) {
        if (strcmp(arr[i]->item, name) == 0) {
            arr[i]->qty += q;
            return;
        }
    }

    arr[*size] = (Item *)malloc(sizeof(Item));
    strcpy(arr[*size]->item, name);
    arr[*size]->qty = q;
    (*size)++;
}

void updateItem(Item *arr[], int size, char name[], int q) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->item, name) == 0) {
            arr[i]->qty = q;
            return;
        }
    }
}

void queryItem(Item *arr[], int size, char name[]) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->item, name) == 0) {
            printf("%d\n", arr[i]->qty);
            return;
        }
    }
    printf("Item not found\n");
}

int main() {
    int n;
    scanf("%d", &n);

    Item *arr[100];
    int size = 0;

    while (n--) {
        char cmd[20], name[50];
        scanf("%s", cmd);

        if (strcmp(cmd, "ADD") == 0) {
            int q;
            scanf("%s %d", name, &q);
            addItem(arr, &size, name, q);
        }
        else if (strcmp(cmd, "UPDATE") == 0) {
            int q;
            scanf("%s %d", name, &q);
            updateItem(arr, size, name, q);
        }
        else if (strcmp(cmd, "QUERY") == 0) {
            scanf("%s", name);
            queryItem(arr, size, name);
        }
    }
    return 0;
}
