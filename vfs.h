#ifndef VFS_H
#define VFS_H

#include <stdio.h>

#define BLOCKS_PER_FILE 100
#define LINE_LENGTH 80
#define MAX_SIZE_BLOCK 512
#define MAX_NAME_LENGTH 51
#define MAX_NUM_BLOCKS 1024
#define PATH_SIZE 1000

typedef struct FreeBlock{
    int index;
    struct FreeBlock* prev;
    struct FreeBlock* next;
} FreeBlock;

typedef struct FileNode{
    char name[MAX_NAME_LENGTH];
    int isDirectory;
    struct FileNode* parent;
    struct FileNode* next;
    struct FileNode* child;
    int blockPointers[BLOCKS_PER_FILE];
    int contentSize;
    int numOfBlocks;
} FileNode;


void setupFreeBlocks();
void initializeFileSystem();

int allocateFreeBlock();
void freeFileBlocks(FileNode *file);
void freeFreeBlocks();

void mkdir(char *name);
void ls();
void pwd();
void cdParent();
void cd(char *name);
void create(char *name);
void deleteFile(FileNode *file);
void delete(char *filename);
void rmdir(char *dirname);


void writeData(FileNode *file, char *data);
void write(char *filename, char *data);
void readData(FileNode *file);
void read(char *filename);


void df();
void freeFileNodes(FileNode *node);
void exit_system();

void takeInput();
void splitInput(char *line, char *cmd, char *arg1, char *arg2);
int commandToID(char *cmd);

#endif
