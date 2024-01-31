// COMP2521 Assignment 1: file.c
//
// A file to abstract away parsing/collecting, 
// creating graphs and binary search trees
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "graph.h"
#include "file.h"
#include "bst.h"
#include "qsort.h"

#define SIZE 1000

Graph collectOutgoingURLs() {
    FILE *collection = fopen("collection.txt", "r");
    if (collection == NULL) {
        fprintf(stderr, "Error reading file 1!\n");
        exit(EXIT_FAILURE);
    }
    char line[SIZE];
    int urlCount = 0;
    int capacity = SIZE;
    char **url = malloc(sizeof(char *) * capacity);
    if (url == NULL) {
        fprintf(stderr, "Memory Allocation Failed!\n");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, SIZE, collection) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        int count = 0;
        char **collection = split(line, " ", &count);
        for (int i = 0; i < count; i++) {
            if (urlCount == capacity) {
                capacity *= 2;
                char **newUrl = realloc(url, sizeof(char *) * capacity);
                if (newUrl == NULL) {
                    fprintf(stderr, "Memory Reallocation Failed!\n");
                    exit(EXIT_FAILURE);
                }
                url = newUrl;
            }
            if (collection[i][0] != 0 && collection[i][0] != '\n' && collection[i][0] != '\t') {
                url[urlCount] = strdup(collection[i]);
                if (url[urlCount] == NULL) {
                    fprintf(stderr, "Str Duplication Failed!\n");
                    exit(EXIT_FAILURE);
                }
                urlCount++;
            }
        }
        for (int i = 0; i < count; i++) {
            free(collection[i]);
        }
        free(collection);
    }
    fclose(collection);
    Graph new = collectUrlTxt(url, urlCount);
    for (int j = 0; j < urlCount; j++) {
        free(url[j]);
    }
    free(url);
    return new;
}

Graph collectUrlTxt(char **url, int urlCount) {
    Graph graph = createGraph(urlCount, url);
    for (int i = 0; i < graph->numURL; i++) {
        char *fileName = strdup(url[i]);
        fileName = realloc(fileName, strlen(url[i]) + strlen(".txt") + 1);
        strcat(fileName, ".txt");
        //open .txt file and start extracting
        FILE *urlTXT;
        urlTXT = fopen(fileName, "r");
        if (urlTXT == NULL) {
            fprintf(stderr,"Error reading file 2!\n");
            exit(EXIT_FAILURE);
        }
        //skip #start Section 1
        fscanf(urlTXT, "%*[^\n]\n");
        char out[SIZE];
        fscanf(urlTXT, "%s", out);
        while (strcmp(out, "#end") != 0) { 
            Outgoing newlink = createList(out);
            addEdge(graph, i, newlink);
            fscanf(urlTXT, "%s", out);
        }
        fclose(urlTXT);
        free(fileName);
    }
    return graph;
}

Tree collectWords() {
    FILE *collection = fopen("collection.txt", "r");
    if (collection == NULL) {
        fprintf(stderr, "Error reading file 3!\n");
        exit(EXIT_FAILURE);
    }

    char line[SIZE];
    int urlCount = 0;
    int capacity = SIZE;
    char **url = malloc(sizeof(char *) * capacity);
    if (url == NULL) {
        fprintf(stderr, "Memory Allocation for char url Failed!\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, SIZE, collection) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        int count = 0;
        char **collection = split(line, " ", &count);
        for (int i = 0; i < count; i++) {
            // realloc the array url
            if (urlCount == capacity) {
                capacity *= 2;
                char **newUrl = realloc(url, sizeof(char *) * capacity);
                if (newUrl == NULL) {
                    fprintf(stderr, "Reallocation Failed!\n");
                    exit(EXIT_FAILURE);
                }
                url = newUrl;
            }
            // str duplicate the file
            if (collection[i][0] != 0 && collection[i][0] != '\n' && collection[i][0] != '\t') {
                url[urlCount] = strdup(collection[i]);
                if (url[urlCount] == NULL) {
                    fprintf(stderr, "Str Duplication Failed!\n");
                    exit(EXIT_FAILURE);
                }
                urlCount++;
            }
        }

        for (int i = 0; i < count; i++) {
            free(collection[i]);
        }
        free(collection);
    }
    fclose(collection);
    // quick sort algrotihm in the stdlib library
    // needs a function from my qsort.c file strcompare
    qsort(url, urlCount, sizeof(char *), strcompare);
    Tree tree = collectTreeTxt(url, urlCount);

    for (int j = 0; j < urlCount; j++) {
        free(url[j]);
    }
    free(url);
    return tree;
}

// for debugging purposes of the avl tree
Tree insertAvlTrees(char **url, int urlCount) {
    Tree tree = NULL;
    for (int i = 0; i < urlCount; i++) {
        lowerCase(url[i]);
        char *normalizedUrl = removePunc(url[i]);
        if (normalizedUrl == NULL) {
            continue;
        }

        char *urlToInsert = normalizedUrl;
        if (normalizedUrl != url[i]) {
            urlToInsert = normalizedUrl;
        } 

        if (tree == NULL) {
            tree = createTree(urlToInsert);
        } else {
            tree = insertAVL(tree, urlToInsert);
        }

        if (urlToInsert != url[i]) {
            free(normalizedUrl);
        }
    }
    return tree;
}

Tree collectTreeTxt(char **url, int urlCount) {
    Tree tree = NULL;
    int isTreeInitialized = 0;

    for (int i = 0; i < urlCount; i++) {
        char fileName[SIZE];
        sprintf(fileName, "%s.txt", url[i]);

        FILE *urlTXT = fopen(fileName, "r");
        if (urlTXT == NULL) {
            fprintf(stderr, "Failed to open file %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        // Skip #start Section 2
        fscanf(urlTXT, "%*[^\n]%*[^#]#%*[^#]#%*[^\n]\n");

        char line[SIZE];
        char section[] = "#end Section-2";
        while (fgets(line, SIZE, urlTXT) != NULL) {
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }

            // Check if the line is "#end Section-2"
            if (strncmp(line, section, strlen(section)) == 0) {
                break;
            }
            int count = 0;
            char **collection = split(line, " ", &count);
            for (int k = 0; k < count; k++) {
                lowerCase(collection[k]);
                char *normalizedWord = removePunc(collection[k]);
                if (normalizedWord && normalizedWord[0] != '\0' && normalizedWord[0] != '\n' && normalizedWord[0] != '\t') {
                    if (!isTreeInitialized) {
                        tree = createTree(normalizedWord);
                        newBstListNode(tree, normalizedWord, url[i]);
                        isTreeInitialized = 1;
                    } else {
                        tree = insertAVL(tree, normalizedWord);
                        newBstListNode(tree, normalizedWord, url[i]);
                    }
                }

                if (normalizedWord != collection[k]) {
                    free(normalizedWord);
                }
            }
            for (int k = 0; k < count; k++) {
                free(collection[k]);
            }
            free(collection);
        } 
        fclose(urlTXT);
    }
    return tree;
}

void lowerCase(char *s) {
    int length = strlen(s);
    for (int i = 0; i < length; i++) 
        s[i] = tolower(s[i]);
}

char *removePunc(char *s) {
    int len = strlen(s);
    int end = len;
    // Loop backwards to remove unwanted punctuation from the end.
    while (end > 0 && strchr(".,:;?*", s[end - 1]) != NULL) {
        end--;
    }
    // If the entire string is composed of punctuation or is empty, return an empty word.
    if (end == 0) {
       return NULL;
    }
    // + 1 for null terminator
    char *newWord = malloc(sizeof(char) * (end + 1)); 
    if (newWord == NULL) {
        return NULL;
    }
    for (int i = 0; i < end; i++) {
        newWord[i] = s[i];
    }
    newWord[end] = '\0'; 
    return newWord;
}

// grabs a single line and stores it inside a 2D array of pointer char
char **split(char *string, char *seperators, int *count) {
    int len = strlen(string);
    *count = 0;
    int i = 0;
    while (i < len) {
        while (i < len) {
            if (strchr(seperators, string[i]) == NULL)
                break;
            i++;
        }
        int old_i = i;
        while (i < len) {
            if (strchr(seperators, string[i]) != NULL)
                break;
            i++;
        }
        if (i > old_i) {
            *count = *count + 1;
        }
    }
    char **strings = malloc(sizeof(char *) * *count);
    i = 0;
    //16384
    char buffer[100000];
    int string_index = 0;
    while (i < len) {
        while (i < len) {
            if (strchr(seperators, string[i]) == NULL)
                break;
            i++;
        }
        int j = 0;
        while (i < len) {
            if (strchr(seperators, string[i]) != NULL)
                break;
            buffer[j] = string[i];
            i++;
            j++;
        }
        if (j > 0) {
            buffer[j] = '\0';
            int to_allocate = sizeof(char) * (strlen(buffer) + 1);
            strings[string_index] = malloc(to_allocate);
            strcpy(strings[string_index], buffer);
            string_index++;
        }
    }
    return strings;
}

