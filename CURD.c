#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char *name;
    int age;
} User;


void createFile() {
    FILE *filePtr = fopen("users.txt", "w");
    if (filePtr == NULL) {
        printf("Error creating file!\n");
        exit(1);
    }
    fclose(filePtr);
}


int isValidName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0; 
        }
    }
    return 1;
}


void addUser() {
    FILE *filePtr = fopen("users.txt", "a");
    if (filePtr == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    User user;
    char nameInput[100];

    printf("Enter User ID: ");
    scanf("%d", &user.id);

    while (1) {
        printf("Enter User Name: ");
        scanf("%s", nameInput);
        if (isValidName(nameInput)) {
            user.name = malloc(strlen(nameInput) + 1);
            strcpy(user.name, nameInput);
            break;
        } else {
            printf("Invalid name. Please enter alphabetic characters only.\n");
        }
    }

    printf("Enter User Age: ");
    scanf("%d", &user.age);

    fprintf(filePtr, "%d %s %d\n", user.id, user.name, user.age);
    fclose(filePtr);
    free(user.name);
    printf("User added successfully!\n");
}


void displayUsers() {
    FILE *filePtr = fopen("users.txt", "r");
    if (filePtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    User user;
    char nameInput[100];
    printf("\nID\tName\tAge\n");
    printf("---------------------\n");
    while (fscanf(filePtr, "%d %s %d", &user.id, nameInput, &user.age) == 3) {
        printf("%d\t%s\t%d\n", user.id, nameInput, user.age);
    }
    fclose(filePtr);
}


void updateUser() {
    FILE *filePtr = fopen("users.txt", "r");
    FILE *tempf = fopen("temp.txt", "w");
    if (filePtr == NULL || tempf == NULL) {
        printf("Error opening file.\n");
        return;
    }

    User user;
    char nameInput[100];
    int Id, found = 0;
    printf("Enter User ID to update: ");
    scanf("%d", &Id);

    while (fscanf(filePtr, "%d %s %d", &user.id, nameInput, &user.age) == 3) {
        if (user.id == Id) {
            found = 1;
            printf("Enter updated User ID: ");
            scanf("%d", &user.id);

            while (1) {
                printf("Enter updated Name: ");
                scanf("%s", nameInput);
                if (isValidName(nameInput)) {
                    user.name = malloc(strlen(nameInput) + 1);
                    strcpy(user.name, nameInput);
                    break;
                } else {
                    printf("Invalid name. Please enter alphabetic characters only.\n");
                }
            }

            printf("Enter updated Age: ");
            scanf("%d", &user.age);
        }
        fprintf(tempf, "%d %s %d\n", user.id, nameInput, user.age);
    }

    fclose(filePtr);
    fclose(tempf);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User updated successfully!\n");
    } else {
        printf("User not found.\n");
        remove("temp.txt");
    }
}


void deleteUser() {
    FILE *filePtr = fopen("users.txt", "r");
    FILE *tempf = fopen("temp.txt", "w");
    if (filePtr == NULL || tempf == NULL) {
        printf("Error opening file.\n");
        return;
    }

    User user;
    char nameInput[100];
    int id, found = 0;
    printf("Enter User ID to delete: ");
    scanf("%d", &id);

    while (fscanf(filePtr, "%d %s %d", &user.id, nameInput, &user.age) == 3) {
        if (user.id != id) {
            fprintf(tempf, "%d %s %d\n", user.id, nameInput, user.age);
        } else {
            found = 1;
        }
    }

    fclose(filePtr);
    fclose(tempf);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User deleted successfully!\n");
    } else {
        printf("User not found.\n");
        remove("temp.txt");
    }
}


int getValidChoice() {
    int choice;
    char input[10];
    while (1) {
        printf("Enter your choice: ");
        if (fgets(input, sizeof(input), stdin)) {
            if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 5) {
                return choice;
            } else {
                printf("Please enter a valid choice (1-5).\n");
            }
        }
    }
}


int main() {
    int choice;
    createFile();

    do {
        printf("\n1. Add User\n2. Display Users\n3. Update User\n4. Delete User\n5. Exit\n");
        choice = getValidChoice();

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                displayUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}