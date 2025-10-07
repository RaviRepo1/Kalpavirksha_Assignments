#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 100

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
} User;

// Functions
void createFile();
int isValidName(const char *name);
int inputUserId(const char *prompt);
User inputUser();
void addUser();
void displayUsers();
int updateUser();
int deleteUser();
int getValidChoice();

// Create an empty file /if not present
void createFile() {
    FILE *filePtr = fopen("users.txt", "a");
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


int inputUserId(const char *prompt) {
    int id;
    char input[20];
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) && sscanf(input, "%d", &id) == 1) {
            return id;
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }
}


User inputUser() {
    User user;
    char input[NAME_LEN];

    user.id = inputUserId("Enter User ID: ");

    while (1) {
        printf("Enter User Name: ");
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = '\0'; 
            if (isValidName(input)) {
                strncpy(user.name, input, NAME_LEN);
                break;
            } else {
                printf("Invalid name. Only alphabetic characters and spaces are allowed.\n");
            }
        }
    }

    user.age = inputUserId("Enter User Age: ");
    return user;
}

// Add a new user to the file
void addUser() {
    FILE *filePtr = fopen("users.txt", "a");
    if (!filePtr) {
        printf("Error opening file!\n");
        exit(1);
    }

    User user = inputUser();
    fprintf(filePtr, "%d %s %d\n", user.id, user.name, user.age);
    fclose(filePtr);
    printf("User added successfully!\n");
}

// Display all users
void displayUsers() {
    FILE *filePtr = fopen("users.txt", "r");
    if (filePtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    User user;
    char tempName[NAME_LEN];
    printf("\nID\tName\t\tAge\n");
    printf("------------------------------\n");
    while (fscanf(filePtr, "%d %s %d", &user.id, tempName, &user.age) == 3) {
        printf("%d\t%-15s%d\n", user.id, tempName, user.age);
    }
    fclose(filePtr);
}

// Update an existing user 
int updateUser() {
    FILE *filePtr = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!filePtr || !tempFile) {
        printf("Error opening file.\n");
        return 0;
    }

    User user;
    char tempName[NAME_LEN];
    int targetId = inputUserId("Enter User ID to update: ");
    int found = 0;

    while (fscanf(filePtr, "%d %s %d", &user.id, tempName, &user.age) == 3) {
        if (user.id == targetId) {
            found = 1;
            printf("Enter updated details:\n");
            User updatedUser = inputUser();
            fprintf(tempFile, "%d %s %d\n", updatedUser.id, updatedUser.name, updatedUser.age);
        } else {
            fprintf(tempFile, "%d %s %d\n", user.id, tempName, user.age);
        }
    }

    fclose(filePtr);
    fclose(tempFile);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User updated successfully!\n");
        return 1;
    } else {
        remove("temp.txt");
        printf("User not found.\n");
        return 0;
    }
}


int deleteUser() {
    FILE *filePtr = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!filePtr || !tempFile) {
        printf("Error opening file.\n");
        return 0;
    }

    User user;
    char tempName[NAME_LEN];
    int targetId = inputUserId("Enter User ID to delete: ");
    int found = 0;

    while (fscanf(filePtr, "%d %s %d", &user.id, tempName, &user.age) == 3) {
        if (user.id != targetId) {
            fprintf(tempFile, "%d %s %d\n", user.id, tempName, user.age);
        } else {
            found = 1;
        }
    }

    fclose(filePtr);
    fclose(tempFile);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User deleted successfully!\n");
        return 1;
    } else {
        remove("temp.txt");
        printf("User not found.\n");
        return 0;
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
        printf("\n==== User Management System ====\n");
        printf("1. Add User\n");
        printf("2. Display Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");

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
                printf("Exiting program. Goodbye!\n");
                break;
        }
    } while (choice != 5);

    return 0;
}

