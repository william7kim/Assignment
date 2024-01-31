#ifndef FILE_H
#define FILE_H

#include "bst.h"
#include "graph.h"
#include "qsort.h"

Graph collectOutgoingURLs();

Graph collectUrlTxt(char **url, int urlCount);

Tree collectWords();

//debugging avl tree
Tree insertAvlTrees(char **url, int urlCount);

Tree collectTreeTxt(char **url, int urlCount);

char **split(char *string, char *seperators, int *count);

void lowerCase(char *s) ;

char *removePunc(char *s);

char *removePunctuation(char *word);


#endif

