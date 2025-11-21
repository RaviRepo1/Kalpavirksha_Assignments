#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vfs.h"

statusCode setupFreeBlocks(FileSystem *fsState)
{
    if (!fsState)
        return INVALID_INPUT;

    FreeBlock *previous = NULL;
    fsState->freeBlockListHead = NULL;

    for (int index = 0; index < MAX_NUM_BLOCKS; index++)
    {
        FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
        if (!block)
            return DISK_IS_FULL;
        block->index = index;
        block->next = NULL;
        block->prev = previous;
        if (previous)
            previous->next = block;
        else
            fsState->freeBlockListHead = block;
        previous = block;
    }
    return SUCCESS;
}

FileSystem *initializeFileSystem()
{
    FileSystem *fsState = (FileSystem *)malloc(sizeof(FileSystem));
    if (!fsState)
    {
        printf("Failed to allocate memory for filesystem.\n");
        return NULL;
    }

    fsState->root = NULL;
    fsState->cwd = NULL;
    fsState->freeBlockListHead = NULL;

    if (setupFreeBlocks(fsState) != SUCCESS)
    {
        free(fsState);
        printf("Failed to setup free blocks.\n");
        return NULL;
    }

    FileNode *root = (FileNode *)malloc(sizeof(FileNode));
    if (!root)
    {
        freeFreeBlocks(fsState);
        free(fsState);
        printf("Failed to create root.\n");
        return NULL;
    }

    root->isDirectory = true;
    root->parent = NULL;
    root->child = NULL;
    strcpy(root->name, "/");
    root->next = root;
    root->contentSize = 0;
    root->numOfBlocks = 0;
    for (int i = 0; i < BLOCKS_PER_FILE; i++)
        root->blockPointers[i] = -1;

    fsState->root = root;
    fsState->cwd = root;

    printf("Compact VFS - ready. Type 'exit' to quit.\n");
    return fsState;
}

statusCode freeFileBlocks(FileSystem *fsState, FileNode *file)
{
    if (!fsState || !file)
        return INVALID_INPUT;

    for (int i = 0; i < file->numOfBlocks; i++)
    {
        int blockIndex = file->blockPointers[i];
        if (blockIndex >= 0)
        {

            memset(fsState->virtualMemoryDisk[blockIndex], 0, MAX_SIZE_BLOCK);

            FreeBlock *newFree = malloc(sizeof(FreeBlock));
            if (!newFree)
                return INVALID_INPUT;
            newFree->index = blockIndex;
            newFree->next = fsState->freeBlockListHead;
            if (fsState->freeBlockListHead)
                fsState->freeBlockListHead->prev = newFree;
            newFree->prev = NULL;
            fsState->freeBlockListHead = newFree;
            file->blockPointers[i] = -1;
        }
    }
    file->contentSize = 0;
    file->numOfBlocks = 0;
    return SUCCESS;
}

statusCode mkdir_vfs(FileSystem *fsState, char *name)
{
    if (!fsState || !name || !*name)
        return INVALID_INPUT;

    FileNode *cwd = fsState->cwd;
    FileNode *node = cwd->child;

    if (node)
    {
        do
        {
            if (strcmp(node->name, name) == 0)
            {
                printf("Directory with name %s already exists.\n", name);
                return FILENODE_EXISTS;
            }
            node = node->next;
        } while (node != cwd->child);
    }

    FileNode *newDir = malloc(sizeof(FileNode));
    if (!newDir)
        return INVALID_INPUT;
    strcpy(newDir->name, name);
    newDir->isDirectory = true;
    newDir->parent = cwd;
    newDir->child = NULL;
    newDir->contentSize = 0;
    newDir->numOfBlocks = 0;
    for (int i = 0; i < BLOCKS_PER_FILE; i++)
        newDir->blockPointers[i] = -1;

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
    return SUCCESS;
}

int allocateFreeBlock(FileSystem *fsState)
{
    if (!fsState)
        return -1;
    if (!fsState->freeBlockListHead)
    {
        printf("No memory left.\n");
        return -1;
    }
    FreeBlock *allocatedBlock = fsState->freeBlockListHead;
    int index = allocatedBlock->index;
    fsState->freeBlockListHead = fsState->freeBlockListHead->next;
    if (fsState->freeBlockListHead)
        fsState->freeBlockListHead->prev = NULL;
    free(allocatedBlock);
    return index;
}

void ls(FileSystem *fsState)
{
    if (!fsState)
        return;
    FileNode *cwd = fsState->cwd;
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

void pwd(FileSystem *fsState)
{
    if (!fsState)
        return;
    FileNode *temp = fsState->cwd;
    char path[PATH_SIZE] = "";
    char tempPath[PATH_SIZE] = "";

    while (temp != fsState->root)
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

void cdParent(FileSystem *fsState)
{
    if (!fsState)
        return;
    if (fsState->cwd == fsState->root)
    {
        printf("Already on root.\n");
        return;
    }
    fsState->cwd = fsState->cwd->parent;
    printf("Moved to ");
    pwd(fsState);
}

statusCode cd(FileSystem *fsState, char *name)
{
    if (!fsState || !name)
        return INVALID_INPUT;

    if (!strcmp(name, ".."))
    {
        cdParent(fsState);
        return SUCCESS;
    }
    FileNode *temp = fsState->cwd->child;

    if (!temp)
    {
        printf("%s is empty.\n", fsState->cwd->name);
        return DIRECTORY_NOT_FOUND;
    }
    do
    {
        if (!strcmp(temp->name, name) && temp->isDirectory)
        {
            fsState->cwd = temp;
            printf("Moved to ");
            pwd(fsState);
            return SUCCESS;
        }
        temp = temp->next;
    } while (temp != fsState->cwd->child);

    printf("No folder found with name %s\n", name);
    return DIRECTORY_NOT_FOUND;
}

statusCode create(FileSystem *fsState, char *name)
{
    if (!fsState || !name || !*name)
        return INVALID_INPUT;

    FileNode *node = fsState->cwd->child;

    if (node)
    {
        do
        {
            if (!strcmp(node->name, name))
            {
                printf("File with name %s already exists.\n", name);
                return FILENODE_EXISTS;
            }
            node = node->next;
        } while (node != fsState->cwd->child);
    }

    FileNode *newFile = malloc(sizeof(FileNode));
    if (!newFile)
        return INVALID_INPUT;
    strcpy(newFile->name, name);
    newFile->isDirectory = false;
    newFile->child = NULL;
    newFile->parent = fsState->cwd;
    newFile->contentSize = 0;
    newFile->numOfBlocks = 0;

    for (int i = 0; i < BLOCKS_PER_FILE; i++)
        newFile->blockPointers[i] = -1;

    if (!fsState->cwd->child)
    {
        fsState->cwd->child = newFile;
        newFile->next = newFile;
    }
    else
    {
        FileNode *temp = fsState->cwd->child;
        while (temp->next != fsState->cwd->child)
            temp = temp->next;
        newFile->next = temp->next;
        temp->next = newFile;
    }

    printf("File '%s' created successfully.\n", name);
    return SUCCESS;
}

statusCode writeData(FileSystem *fsState, FileNode *file, char *data, int append)
{
    if (!fsState || !file || file->isDirectory)
    {
        printf("Invalid file.\n");
        return FILE_NOT_FOUND;
    }
    if (!data)
        return INVALID_INPUT;

    if (!append && file->numOfBlocks > 0)
    {
        freeFileBlocks(fsState, file);
    }

    int len = strlen(data);

    if (append && file->contentSize > 0)
    {
        int existingSize = file->contentSize;
        char *combined = malloc(existingSize + len + 1);
        if (!combined)
            return INVALID_INPUT;

        int pos = 0;

        int remaining = existingSize;
        for (int i = 0; i < file->numOfBlocks && remaining > 0; i++)
        {
            int b = file->blockPointers[i];
            if (b < 0)
                continue;
            int chunkLen = remaining < MAX_SIZE_BLOCK ? remaining : MAX_SIZE_BLOCK;
            memcpy(combined + pos, fsState->virtualMemoryDisk[b], chunkLen);
            pos += chunkLen;
            remaining -= chunkLen;
        }

        memcpy(combined + pos, data, len);
        pos += len;
        combined[pos] = '\0';

        freeFileBlocks(fsState, file);

        len = pos;
        data = combined;

        int needed = (len + MAX_SIZE_BLOCK - 1) / MAX_SIZE_BLOCK;
        file->contentSize = len;
        file->numOfBlocks = 0;
        int start = 0;
        for (int i = 0; i < needed; i++)
        {
            int blockIndex = allocateFreeBlock(fsState);
            if (blockIndex == -1)
            {
                free(data);
                return DISK_IS_FULL;
            }
            file->blockPointers[file->numOfBlocks++] = blockIndex;
            int copyLen = (len - start) < MAX_SIZE_BLOCK ? (len - start) : MAX_SIZE_BLOCK;
            memcpy(fsState->virtualMemoryDisk[blockIndex], data + start, copyLen);
            if (copyLen < MAX_SIZE_BLOCK)
                fsState->virtualMemoryDisk[blockIndex][copyLen] = '\0';
            else
                fsState->virtualMemoryDisk[blockIndex][MAX_SIZE_BLOCK - 1] = '\0';
            start += copyLen;
        }
        free(data);
        printf("Data written successfully(size = %d bytes)\n", file->contentSize);
        return SUCCESS;
    }
    else
    {

        int needed = (len + MAX_SIZE_BLOCK - 1) / MAX_SIZE_BLOCK;
        file->contentSize = len;
        file->numOfBlocks = 0;
        int start = 0;
        for (int i = 0; i < needed; i++)
        {
            int blockIndex = allocateFreeBlock(fsState);
            if (blockIndex == -1)
                return DISK_IS_FULL;

            file->blockPointers[file->numOfBlocks++] = blockIndex;
            int copyLen = (len - start) < MAX_SIZE_BLOCK ? (len - start) : MAX_SIZE_BLOCK;
            memcpy(fsState->virtualMemoryDisk[blockIndex], data + start, copyLen);
            if (copyLen < MAX_SIZE_BLOCK)
                fsState->virtualMemoryDisk[blockIndex][copyLen] = '\0';
            else
                fsState->virtualMemoryDisk[blockIndex][MAX_SIZE_BLOCK - 1] = '\0';
            start += copyLen;
        }
        printf("Data written successfully(size = %d bytes)\n", file->contentSize);
        return SUCCESS;
    }
}

statusCode write(FileSystem *fsState, char *arg1, char *arg2)
{
    if (!fsState)
        return INVALID_INPUT;
    if (!arg1 || !arg2)
        return INVALID_INPUT;

    char filename[100] = "";
    char dataBuf[1024] = "";

    strncpy(filename, arg1, sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = '\0';
    strncpy(dataBuf, arg2, sizeof(dataBuf) - 1);
    dataBuf[sizeof(dataBuf) - 1] = '\0';

    size_t dbLen = strlen(dataBuf);
    if (dbLen < 2 || dataBuf[0] != '"' || dataBuf[dbLen - 1] != '"')
    {
        printf("Syntax error. Data must be quoted: \"...\"\n");
        return INVALID_INPUT;
    }

    dataBuf[dbLen - 1] = '\0';
    char *dataRaw = dataBuf + 1;

    char processed[1024];
    int j = 0;
    for (size_t i = 0; dataRaw[i] && j < (int)sizeof(processed) - 1; i++)
    {
        if (dataRaw[i] == '\\' && dataRaw[i + 1] == 'n')
        {
            processed[j++] = '\n';
            i++;
        }
        else
        {
            processed[j++] = dataRaw[i];
        }
    }
    processed[j] = '\0';

    FileNode *temp = fsState->cwd->child;
    FileNode *found = NULL;

    if (!temp)
    {

        statusCode sc = create(fsState, filename);
        if (sc != SUCCESS)
            return sc;
        temp = fsState->cwd->child;
    }

    FileNode *start = temp;
    do
    {
        if (!strcmp(temp->name, filename) && !temp->isDirectory)
        {
            found = temp;
            break;
        }
        temp = temp->next;
    } while (temp != start);

    if (!found)
    {
        statusCode sc = create(fsState, filename);
        if (sc != SUCCESS)
            return sc;

        temp = fsState->cwd->child;
        start = temp;
        do
        {
            if (!strcmp(temp->name, filename) && !temp->isDirectory)
            {
                found = temp;
                break;
            }
            temp = temp->next;
        } while (temp != start);
    }

    if (!found)
    {
        printf("Unable to create or find file %s\n", filename);
        return INVALID_INPUT;
    }

    return writeData(fsState, found, processed, 1);
}

void readData(FileSystem *fsState, FileNode *file)
{
    if (!fsState || !file)
        return;

    if (file->numOfBlocks == 0)
    {
        printf("File is empty.\n");
        return;
    }

    int remaining = file->contentSize;
    for (int i = 0; i < file->numOfBlocks && remaining > 0; i++)
    {
        int b = file->blockPointers[i];
        if (b < 0)
            continue;

        int len = (remaining < MAX_SIZE_BLOCK) ? remaining : MAX_SIZE_BLOCK;
        fwrite(fsState->virtualMemoryDisk[b], 1, len, stdout);
        remaining -= len;
    }
    printf("\n");
}

statusCode read(FileSystem *fsState, char *filename)
{
    if (!fsState || !filename)
        return INVALID_INPUT;
    FileNode *temp = fsState->cwd->child;
    if (!temp)
    {
        printf("No file found.\n");
        return FILE_NOT_FOUND;
    }
    do
    {
        if (!strcmp(temp->name, filename) && !temp->isDirectory)
        {
            readData(fsState, temp);
            return SUCCESS;
        }
        temp = temp->next;
    } while (temp != fsState->cwd->child);

    printf("No file found with name %s.\n", filename);
    return FILE_NOT_FOUND;
}

statusCode deleteFile(FileSystem *fsState, FileNode *file)
{
    if (!fsState || !file)
        return INVALID_INPUT;

    freeFileBlocks(fsState, file);

    if (file->next == file)
    {
        fsState->cwd->child = NULL;
    }
    else
    {
        FileNode *temp = fsState->cwd->child;
        while (temp->next != file && temp->next != fsState->cwd->child)
            temp = temp->next;
        temp->next = file->next;
        if (file == fsState->cwd->child)
            fsState->cwd->child = file->next;
    }
    free(file);
    printf("File deleted successfully.\n");
    return SUCCESS;
}

statusCode Delete(FileSystem *fsState, char *filename)
{
    if (!fsState || !filename)
        return INVALID_INPUT;
    if (!fsState->cwd->child)
    {
        printf("No file found.\n");
        return FILE_NOT_FOUND;
    }
    FileNode *temp = fsState->cwd->child;
    do
    {
        if (!strcmp(temp->name, filename) && !temp->isDirectory)
        {
            return deleteFile(fsState, temp);
        }
        temp = temp->next;
    } while (temp != fsState->cwd->child);

    printf("No file found with name %s.\n", filename);
    return FILE_NOT_FOUND;
}

statusCode rmdir(FileSystem *fsState, char *dirname)
{
    if (!fsState || !dirname)
        return INVALID_INPUT;
    if (!fsState->cwd->child)
    {
        printf("No directory found.\n");
        return DIRECTORY_NOT_FOUND;
    }
    FileNode *temp = fsState->cwd->child;
    do
    {
        if (!strcmp(temp->name, dirname))
        {
            if (!temp->isDirectory)
            {
                printf("%s is not a directory.\n", dirname);
                return DIRECTORY_NOT_FOUND;
            }
            if (temp->child)
            {
                printf("Directory not empty. Remove files first.\n");
                return DIRECTORY_NOT_EMPTY;
            }

            if (temp->next == temp)
                fsState->cwd->child = NULL;
            else
            {
                FileNode *node = fsState->cwd->child;
                while (node->next != temp && node->next != fsState->cwd->child)
                    node = node->next;
                node->next = temp->next;
                if (temp == fsState->cwd->child)
                    fsState->cwd->child = temp->next;
            }
            free(temp);
            printf("Directory removed successfully.\n");
            return SUCCESS;
        }
        temp = temp->next;
    } while (temp != fsState->cwd->child);

    printf("No directory found with name %s.\n", dirname);
    return DIRECTORY_NOT_FOUND;
}

void df(FileSystem *fsState)
{
    if (!fsState)
        return;
    int freeBlocks = 0;
    FreeBlock *temp = fsState->freeBlockListHead;

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

statusCode freeFreeBlocks(FileSystem *fsState)
{
    if (!fsState)
        return INVALID_INPUT;
    FreeBlock *temp = fsState->freeBlockListHead;
    while (temp)
    {
        FreeBlock *next = temp->next;
        free(temp);
        temp = next;
    }
    fsState->freeBlockListHead = NULL;
    return SUCCESS;
}

void freeFileNodes(FileNode *rootNode, FileSystem *fsState)
{
    if (!rootNode)
        return;
    FileNode *child = rootNode->child;

    if (child)
    {
        FileNode *start = child;
        do
        {
            FileNode *nextChild = child->next;
            freeFileNodes(child, fsState);
            child = (nextChild == start) ? NULL : nextChild;
        } while (child);
    }
    if (!rootNode->isDirectory)
        freeFileBlocks(fsState, rootNode);
    free(rootNode);
}

void exit_system(FileSystem *fsState)
{
    if (!fsState)
        return;
    freeFileNodes(fsState->root, fsState);
    fsState->root = NULL;
    fsState->cwd = NULL;
    freeFreeBlocks(fsState);
    free(fsState);
    printf("Memory released. Exiting program...\n");
}

void splitInput(char *line, InputTokens *tokens)
{
    int i = 0, j = 0;

    tokens->cmd[0] = '\0';
    tokens->arg1[0] = '\0';
    tokens->arg2[0] = '\0';
    tokens->tokenCount = 0;

    while (line[i] == ' ')
        i++;

    while (line[i] != ' ' && line[i] != '\0')
        tokens->cmd[j++] = line[i++];
    tokens->cmd[j] = '\0';
    if (tokens->cmd[0] != '\0')
        tokens->tokenCount++;

    while (line[i] == ' ')
        i++;

    j = 0;
    while (line[i] != ' ' && line[i] != '\0')
        tokens->arg1[j++] = line[i++];
    tokens->arg1[j] = '\0';
    if (tokens->arg1[0] != '\0')
        tokens->tokenCount++;

    while (line[i] == ' ')
        i++;

    j = 0;
    while (line[i] != '\0')
        tokens->arg2[j++] = line[i++];
    tokens->arg2[j] = '\0';
    if (tokens->arg2[0] != '\0')
        tokens->tokenCount++;
}

int commandToID(char *cmd)
{
    if (!cmd)
        return -1;
    if (!strcmp(cmd, "mkdir"))
        return 1;
    if (!strcmp(cmd, "cd"))
        return 2;
    if (!strcmp(cmd, "ls"))
        return 3;
    if (!strcmp(cmd, "create"))
        return 4;
    if (!strcmp(cmd, "write"))
        return 5;
    if (!strcmp(cmd, "read"))
        return 6;
    if (!strcmp(cmd, "delete"))
        return 7;
    if (!strcmp(cmd, "rmdir"))
        return 8;
    if (!strcmp(cmd, "pwd"))
        return 9;
    if (!strcmp(cmd, "df"))
        return 10;
    if (!strcmp(cmd, "exit"))
        return 11;
    return -1;
}

void printStatus(statusCode sc)
{
    switch (sc)
    {
    case SUCCESS:
        printf("Operation successful.\n");
        break;
    case FILE_NOT_FOUND:
        printf("File not found.\n");
        break;
    case DIRECTORY_NOT_FOUND:
        printf("Directory not found.\n");
        break;
    case DIRECTORY_NOT_EMPTY:
        printf("Directory not empty.\n");
        break;
    case FILENODE_EXISTS:
        printf("File/Directory already exists.\n");
        break;
    case EMPTY_DIRECTORY:
        printf("Empty directory.\n");
        break;
    case DISK_IS_FULL:
        printf("Disk full.\n");
        break;
    case INVALID_INPUT:
        printf("Invalid input.\n");
        break;
    default:
        printf("Unknown status.\n");
    }
}

void takeInput(FileSystem *fsState)
{
    if (!fsState)
        return;
    printf("%s > ", fsState->cwd->name);

    char line[LINE_LENGTH];
    if (!fgets(line, LINE_LENGTH, stdin))
        return;
    line[strcspn(line, "\n")] = '\0';

    InputTokens tokens;
    splitInput(line, &tokens);

    int cmdID = commandToID(tokens.cmd);
    statusCode sc = INVALID_INPUT;

    switch (cmdID)
    {
    case 1:
        if (!tokens.arg1[0])
            printf("Write name.\n");
        else
            sc = mkdir_vfs(fsState, tokens.arg1);
        break;

    case 2:
        if (!tokens.arg1[0])
            printf("Write name.\n");
        else
            sc = cd(fsState, tokens.arg1);
        break;

    case 3:
        ls(fsState);
        sc = SUCCESS;
        break;

    case 4:
        if (!tokens.arg1[0])
            printf("Write filename.\n");
        else
            sc = create(fsState, tokens.arg1);
        break;

    case 5:
        if (!tokens.arg1[0] || !tokens.arg2[0])
        {
            printf("Syntax: write filename \"data\"\n");
        }
        else
        {
            sc = write(fsState, tokens.arg1, tokens.arg2);
        }
        break;

    case 6:
        if (!tokens.arg1[0])
            printf("Write name.\n");
        else
            sc = read(fsState, tokens.arg1);
        break;

    case 7:
        if (!tokens.arg1[0])
            printf("Write name.\n");
        else
            sc = Delete(fsState, tokens.arg1);
        break;

    case 8:
        if (!tokens.arg1[0])
            printf("Write name.\n");
        else
            sc = rmdir(fsState, tokens.arg1);
        break;

    case 9:
        pwd(fsState);
        sc = SUCCESS;
        break;

    case 10:
        df(fsState);
        sc = SUCCESS;
        break;

    case 11:
        exit_system(fsState);
        exit(0);

    default:
        printf("Invalid command.\n");
        sc = INVALID_INPUT;
    }

    printStatus(sc);
}
