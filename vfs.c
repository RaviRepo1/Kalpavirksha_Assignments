#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

char virtualMemoryDisk[MAX_NUM_BLOCKS][MAX_SIZE_BLOCK];
FreeBlock *freeBlockListHead = NULL;

FileNode *root = NULL;
FileNode *cwd = NULL;


void setupFreeBlocks()
{
    FreeBlock *previous = NULL;
    for (int index = 0; index < MAX_NUM_BLOCKS; index++)
    {
        FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
        block->index = index;
        block->next = NULL;
        block->prev = previous;
        if (previous)
            previous->next = block;
        else
            freeBlockListHead = block;
        previous = block;
    }
}


void initializeFileSystem()
{
    setupFreeBlocks();

    root = (FileNode *)malloc(sizeof(FileNode));
    root->isDirectory = 1;
    root->parent = NULL;
    root->child = NULL;
    strcpy(root->name, "/");
    root->next = root;
    root->contentSize = 0;
    root->numOfBlocks = 0;
    cwd = root;

    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}


int allocateFreeBlock()
{
    if (!freeBlockListHead)
    {
        printf("No memory left.\n");
        return -1;
    }
    FreeBlock *allocatedBlock = freeBlockListHead;
    int index = allocatedBlock->index;
    freeBlockListHead = freeBlockListHead->next;
    if (freeBlockListHead)
        freeBlockListHead->prev = NULL;
    free(allocatedBlock);
    return index;
}


void freeFileBlocks(FileNode *file)
{
    for (int i = 0; i < file->numOfBlocks; i++)
    {
        int blockIndex = file->blockPointers[i];
        if (blockIndex >= 0)
        {
            FreeBlock *newFree = malloc(sizeof(FreeBlock));
            newFree->index = blockIndex;
            newFree->next = freeBlockListHead;
            if (freeBlockListHead)
                freeBlockListHead->prev = newFree;
            newFree->prev = NULL;
            freeBlockListHead = newFree;
            file->blockPointers[i] = -1;
        }
    }
    file->contentSize = 0;
    file->numOfBlocks = 0;
}


void mkdir(char *name)
{
    FileNode *node = cwd->child;

    if (node)
    {
        do
        {
            if (strcmp(node->name, name) == 0)
            {
                printf("Directory with name %s already exists.\n", name);
                return;
            }
            node = node->next;
        } while (node != cwd->child);
    }

    FileNode *newDir = malloc(sizeof(FileNode));
    strcpy(newDir->name, name);
    newDir->isDirectory = 1;
    newDir->parent = cwd;
    newDir->child = NULL;
    newDir->contentSize = 0;
    newDir->numOfBlocks = 0;

    if (!cwd->child)
    {
        cwd->child = newDir;
        newDir->next = newDir;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
            temp = temp->next;
        newDir->next = temp->next;
        temp->next = newDir;
    }
    printf("Directory '%s' created successfully.\n", name);
}


void ls()
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("(empty)\n");
        return;
    }
    do
    {
        printf("%s", temp->name);
        if (temp->isDirectory)
            printf("/");
        printf("\n");
        temp = temp->next;
    } while (temp != cwd->child);
}


void pwd()
{
    FileNode *temp = cwd;
    char path[PATH_SIZE] = "";
    char tempPath[PATH_SIZE] = "";

    while (temp != root)
    {
        snprintf(tempPath, sizeof(tempPath), "%s/%s", temp->name, path);
        strncpy(path, tempPath, sizeof(path));
        temp = temp->parent;
    }
    if (strlen(path) == 0)
    {
        printf("/\n");
        return;
    }
    printf("/%s\n", path);
}

void cdParent()
{
    if (cwd == root)
    {
        printf("Already on root.\n");
        return;
    }
    cwd = cwd->parent;
    printf("Moved to ");
    pwd();
}


void cd(char *name)
{
    if (!strcmp(name, ".."))
    {
        cdParent();
        return;
    }
    FileNode *temp = cwd->child;

    if (!temp)
    {
        printf("%s is empty.\n", cwd->name);
        return;
    }
    do
    {
        if (!strcmp(temp->name, name) && temp->isDirectory)
        {
            cwd = temp;
            printf("Moved to ");
            pwd();
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("No folder found with name %s\n", name);
}


void create(char *name)
{
    FileNode *node = cwd->child;

    if (node)
    {
        do
        {
            if (!strcmp(node->name, name))
            {
                printf("File with name %s already exists.\n", name);
                return;
            }
            node = node->next;
        } while (node != cwd->child);
    }

    FileNode *newFile = malloc(sizeof(FileNode));
    strcpy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->child = NULL;
    newFile->parent = cwd;
    newFile->contentSize = 0;
    newFile->numOfBlocks = 0;

    for (int i = 0; i < BLOCKS_PER_FILE; i++)
        newFile->blockPointers[i] = -1;

    if (!cwd->child)
    {
        cwd->child = newFile;
        newFile->next = newFile;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
            temp = temp->next;
        newFile->next = temp->next;
        temp->next = newFile;
    }

    printf("File '%s' created successfully.\n", name);
}

void writeData(FileNode *file, char *data)
{
    if (!file || file->isDirectory)
    {
        printf("Invalid file.\n");
        return;
    }

    if (file->numOfBlocks > 0)
        freeFileBlocks(file);

    int len = strlen(data);
    int needed = (len + MAX_SIZE_BLOCK - 1) / MAX_SIZE_BLOCK;
    file->contentSize = len;
    file->numOfBlocks = 0;

    int start = 0;
    for (int i = 0; i < needed; i++)
    {
        int blockIndex = allocateFreeBlock();
        if (blockIndex == -1)
            return;

        file->blockPointers[file->numOfBlocks++] = blockIndex;
        strncpy(virtualMemoryDisk[blockIndex], data + start, MAX_SIZE_BLOCK);
        start += MAX_SIZE_BLOCK;
    }
    printf("Data written successfully(size = %d bytes)\n", file->contentSize);
}

void write(char *filename, char *data)
{
    if (data[0] != '"' || data[strlen(data) - 1] != '"')
    {
        printf("Syntax error.\n");
        return;
    }

    data[strlen(data) - 1] = '\0';
    data++;

    for (int i = 0, j = 0; data[i]; i++, j++)
    {
        if (data[i] == '\\' && data[i + 1] == 'n')
        {
            data[j] = '\n';
            i++;
        }
        else
            data[j] = data[i];
    }

    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("No file found.\n");
        return;
    }
    do
    {
        if (!strcmp(temp->name, filename))
        {
            writeData(temp, data);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("No file found with name %s.\n", filename);
}


void readData(FileNode *file)
{
    if (file->numOfBlocks == 0)
    {
        printf("File is empty.\n");
        return;
    }
    for (int i = 0; i < file->numOfBlocks; i++)
        printf("%s", virtualMemoryDisk[file->blockPointers[i]]);
    printf("\n");
}

void read(char *filename)
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("No file found.\n");
        return;
    }
    do
    {
        if (!strcmp(temp->name, filename) && !temp->isDirectory)
        {
            readData(temp);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("No file found with name %s.\n", filename);
}


void deleteFile(FileNode *file)
{
    freeFileBlocks(file);

    if (file->next == file)
    {
        cwd->child = NULL;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != file && temp->next != cwd->child)
            temp = temp->next;
        temp->next = file->next;
        if (file == cwd->child)
            cwd->child = file->next;
    }
    free(file);
    printf("File deleted successfully.\n");
}

void delete(char *filename)
{
    if (!cwd->child)
    {
        printf("No file found.\n");
        return;
    }
    FileNode *temp = cwd->child;
    do
    {
        if (!strcmp(temp->name, filename) && !temp->isDirectory)
        {
            deleteFile(temp);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("No file found with name %s.\n", filename);
}


void rmdir(char *dirname)
{
    if (!cwd->child)
    {
        printf("No directory found.\n");
        return;
    }
    FileNode *temp = cwd->child;
    do
    {
        if (!strcmp(temp->name, dirname))
        {
            if (!temp->isDirectory)
            {
                printf("%s is not a directory.\n", dirname);
                return;
            }
            if (temp->child)
            {
                printf("Directory not empty. Remove files first.\n");
                return;
            }

            if (temp->next == temp)
                cwd->child = NULL;
            else
            {
                FileNode *node = cwd->child;
                while (node->next != temp && node->next != cwd->child)
                    node = node->next;
                node->next = temp->next;
                if (temp == cwd->child)
                    cwd->child = temp->next;
            }
            free(temp);
            printf("Directory removed successfully.\n");
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("No directory found with name %s.\n", dirname);
}


void df()
{
    int freeBlocks = 0;
    FreeBlock *temp = freeBlockListHead;

    while (temp)
    {
        freeBlocks++;
        temp = temp->next;
    }
    printf("Total blocks: %d\n", MAX_NUM_BLOCKS);
    printf("Used blocks: %d\n", MAX_NUM_BLOCKS - freeBlocks);
    printf("Free blocks: %d\n", freeBlocks);
    printf("Disk usage: %.2f%%\n",
           ((float)(MAX_NUM_BLOCKS - freeBlocks) * 100.0) / MAX_NUM_BLOCKS);
}


void freeFreeBlocks()
{
    FreeBlock *temp = freeBlockListHead;
    while (temp)
    {
        FreeBlock *next = temp->next;
        free(temp);
        temp = next;
    }
    freeBlockListHead = NULL;
}


void freeFileNodes(FileNode *node)
{
    if (!node)
        return;
    FileNode *child = node->child;

    if (child)
    {
        FileNode *start = child;
        do
        {
            FileNode *nextChild = child->next;
            freeFileNodes(child);
            child = (nextChild == start) ? NULL : nextChild;
        } while (child);
    }
    if (!node->isDirectory)
        freeFileBlocks(node);
    free(node);
}

void exit_system()
{
    freeFileNodes(root);
    root = NULL;
    cwd = NULL;
    freeFreeBlocks();
    printf("Memory released. Exiting program...\n");
}



void splitInput(char *line, char *cmd, char *arg1, char *arg2)
{
    int i = 0, j = 0;

    
    while (line[i] == ' ') i++;

    
    while (line[i] != ' ' && line[i] != '\0')
        cmd[j++] = line[i++];
    cmd[j] = '\0';

   
    while (line[i] == ' ') i++;

 
    j = 0;
    while (line[i] != ' ' && line[i] != '\0')
        arg1[j++] = line[i++];
    arg1[j] = '\0';

    
    while (line[i] == ' ') i++;


    j = 0;
    while (line[i] != '\0')
        arg2[j++] = line[i++];
    arg2[j] = '\0';
}


int commandToID(char *cmd)
{
    if (!strcmp(cmd, "mkdir"))  return 1;
    if (!strcmp(cmd, "cd"))     return 2;
    if (!strcmp(cmd, "cd.."))   return 3;
    if (!strcmp(cmd, "ls"))     return 4;
    if (!strcmp(cmd, "create")) return 5;
    if (!strcmp(cmd, "write"))  return 6;
    if (!strcmp(cmd, "read"))   return 7;
    if (!strcmp(cmd, "delete")) return 8;
    if (!strcmp(cmd, "rmdir"))  return 9;
    if (!strcmp(cmd, "pwd"))    return 10;
    if (!strcmp(cmd, "df"))     return 11;
    if (!strcmp(cmd, "exit"))   return 12;
    return -1;
}


void takeInput()
{
    printf("%s > ", cwd->name);

    char line[LINE_LENGTH];
    fgets(line, LINE_LENGTH, stdin);
    line[strcspn(line, "\n")] = '\0';

    char cmd[50] = "", arg1[100] = "", arg2[300] = "";
    splitInput(line, cmd, arg1, arg2);

    int cmdID = commandToID(cmd);

    switch (cmdID)
    {
        case 1:     
            if (!arg1[0]) printf("Write name.\n");
            else mkdir(arg1);
            break;

        case 2:     
            if (!arg1[0]) printf("Write name.\n");
            else cd(arg1);
            break;

        case 3:   
            cdParent();
            break;

        case 4:    
            ls();
            break;

        case 5:     
            if (!arg1[0]) printf("Write filename.\n");
            else create(arg1);
            break;

        case 6:
            if (!arg1[0] || !arg2[0])
            {
                printf("Syntax: write filename \"data\"\n");
                break;
            }
            write(arg1, arg2);
            break;

        case 7:     
            if (!arg1[0]) printf("Write name.\n");
            else read(arg1);
            break;

        case 8:    
            if (!arg1[0]) printf("Write name.\n");
            else delete(arg1);
            break;

        case 9:    
            if (!arg1[0]) printf("Write name.\n");
            else rmdir(arg1);
            break;

        case 10:    
            pwd();
            break;

        case 11:   
            df();
            break;

        case 12:    
            exit_system();
            exit(0);
            break;

        default:
            printf("Invalid command.\n");
    }
}


