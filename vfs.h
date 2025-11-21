#ifndef VFS_H
#define VFS_H

#include <stdio.h>
#include <stdbool.h>

#define BLOCKS_PER_FILE 100
#define LINE_LENGTH 512
#define MAX_SIZE_BLOCK 512
#define MAX_NAME_LENGTH 51
#define MAX_NUM_BLOCKS 1024
#define PATH_SIZE 1000

typedef enum
{
    FILENODE_EXISTS,
    FILE_NOT_FOUND,
    DIRECTORY_NOT_FOUND,
    DIRECTORY_NOT_EMPTY,
    EMPTY_DIRECTORY,
    DISK_IS_FULL,
    SUCCESS,
    INVALID_INPUT,
} statusCode;

typedef struct
{
    char cmd[50];
    char arg1[100];
    char arg2[400];
    int tokenCount;
} InputTokens;

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *prev;
    struct FreeBlock *next;
} FreeBlock;

typedef struct FileNode
{
    char name[MAX_NAME_LENGTH];
    bool isDirectory;
    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *child;

    int blockPointers[BLOCKS_PER_FILE];
    int contentSize;
    int numOfBlocks;
} FileNode;

typedef struct FileSystem
{
    FileNode *root;
    FileNode *cwd;
    FreeBlock *freeBlockListHead;
    char virtualMemoryDisk[MAX_NUM_BLOCKS][MAX_SIZE_BLOCK];
} FileSystem;

statusCode setupFreeBlocks(FileSystem *fsState);
FileSystem *initializeFileSystem();

int allocateFreeBlock(FileSystem *fsState);
statusCode freeFileBlocks(FileSystem *fsState, FileNode *file);
statusCode freeFreeBlocks(FileSystem *fsState);

statusCode mkdir_vfs(FileSystem *fsState, char *name);
void ls(FileSystem *fsState);
void pwd(FileSystem *fsState);
void cdParent(FileSystem *fsState);
statusCode cd(FileSystem *fsState, char *name);
statusCode create(FileSystem *fsState, char *name);
statusCode deleteFile(FileSystem *fsState, FileNode *file);
statusCode Delete(FileSystem *fsState, char *filename);
statusCode rmdir(FileSystem *fsState, char *dirname);

statusCode writeData(FileSystem *fsState, FileNode *file, char *data, int append);
statusCode write(FileSystem *fsState, char *arg1, char *arg2);
void readData(FileSystem *fsState, FileNode *file);
statusCode read(FileSystem *fsState, char *filename);

void df(FileSystem *fsState);
void freeFileNodes(FileNode *rootNode, FileSystem *fsState);
void exit_system(FileSystem *fsState);

void takeInput(FileSystem *fsState);
void splitInput(char *line, InputTokens *tokens);

int commandToID(char *cmd);
void printStatus(statusCode sc);

#endif
