#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* getChild(struct NODE* node, char* name) {
    struct NODE* workingNode = node->childPtr;
    if (workingNode == NULL) {
        return NULL;
    }

    while (strcmp(name, workingNode->name) != 0) {
        workingNode = workingNode->siblingPtr;
        if (workingNode == NULL) {
            return NULL;
        }
    }

    return workingNode;
}

//make directory
void mkdir(char pathName[]){
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[strlen(pathName) + 1];
    char dirName[strlen(pathName) + 1];

    struct NODE* parentDir = splitPath(pathName, baseName, dirName);
    if (parentDir == NULL) {
        return;
    }

    if (getChild(parentDir, baseName) != NULL) {
        printf("MKDIR ERROR: directory %s already exists\n", pathName);
        return;
    }

    struct NODE* new = (struct NODE*)malloc(sizeof(struct NODE));

    if (parentDir->childPtr == NULL) {
        parentDir->childPtr = new;
    } else {
        struct NODE* workingNode = parentDir->childPtr;
        while (workingNode->siblingPtr != NULL) {
            workingNode = workingNode->siblingPtr;
        }
        workingNode->siblingPtr = new;
    }

    new->fileType = 'D';
    new->parentPtr = parentDir;
    new->siblingPtr = NULL;
    new->childPtr = NULL;
    strcpy(new->name, baseName);

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    char path_temp[strlen(pathName) + 1];
    strcpy(path_temp, pathName);
    char* path = path_temp;

    if (strcmp(path, "/") == 0) {
        strcpy(dirName, path);
        strcpy(path, "");
        return NULL;
    }

    struct NODE* node;

    if (path[0] == '/') {
        node = root;
        strcpy(dirName, "/");
        path++;
    } else {
        node = cwd;
    }

    const char delim[2] = "/";
    char *token;
    char *nextToken;

    token = strtok(path, delim);
    nextToken = strtok(NULL, delim);

    
    while (nextToken != NULL) {
        strcat(dirName, token);
        strcat(dirName, "/");

        node = getChild(node, token);
        if (node == NULL) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        token = nextToken;
        nextToken = strtok(NULL, delim);
    
    }

    strcpy(baseName, token);
    if (strlen(dirName) > 0) {
        dirName[strlen(dirName)-1] = '\0';
    }
    
    return node;
}