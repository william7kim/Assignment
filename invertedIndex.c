// COMP2521 Assignment 1: invertedIndex.c
//
// Utilize a AVL tree to store information 
// collected from collection.txt and then goes
// to that url.txt file and collects from section 2.
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024


#include <stdlib.h>
#include <stdio.h>

#include "file.h"
#include "bst.h"

int main(void) {
    Tree tree = collectWords();
    FILE *invertedIndex = fopen("invertedIndex.txt", "w"); 
    if (invertedIndex == NULL) {
        fprintf(stderr, "Failed to open invertedIndex.txt!\n");
        exit(EXIT_FAILURE);
    }
    inOrderWrite(invertedIndex, tree);
    fclose(invertedIndex);
    freeTree(tree);
    return 0;
}
