// COMP2521 Assignment 1: bst.c
//
// Essentially a AVL insert only with
// a binary search tree creation and freeing
// alongside a in file linked list impmentation
// that is unique to this file.
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bst.h"

struct list {
    char *url;
    struct list *next; 
};

struct node {
    char *searchWord;
    int numberUrls;
    int height; 
    struct list *urlList; 
    struct node *right;
    struct node *left;
};

typedef struct node *Tree;
typedef struct list *List;

Tree createTree(char *searchWord) {
    Tree new = malloc(sizeof(struct node));
    if (new == NULL) {
        fprintf(stderr,"Memory Allocation Failed!");
        exit(EXIT_FAILURE);
    }
    new->searchWord = strdup(searchWord);
    new->numberUrls = 0;
    new->height = 0;
    new->urlList = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

// Insert at a leaf node;
Tree treeInsert(Tree tree, char *word) {
    if (tree == NULL) {
        return createTree(word);
    } else if (strcmp(word, tree->searchWord) < 0) {
        tree->left = treeInsert(tree->left, word);
    } else if (strcmp(word, tree->searchWord) > 0) {
        tree->right = treeInsert(tree->right, word);
    } else {
        tree->numberUrls++;
    }
    return tree;
}

Tree insertAVL(Tree tree, char *word) {
    if (tree == NULL) {
        Tree newNode = createTree(word);
        newNode->height = 0;
        return newNode;
    }
    if (strcmp(word, tree->searchWord) < 0) {
        tree->left = insertAVL(tree->left, word);
    } else if (strcmp(word, tree->searchWord) > 0) {
        tree->right = insertAVL(tree->right, word);
    } else {
        return tree;
    }
    updateHeight(tree);
    int balance = height(tree->left) - height(tree->right);
    // Left left case
    if (balance > 1 && strcmp(word, tree->left->searchWord) < 0) {
        return rotateRight(tree);
    }
    // Right right case
    if (balance < -1 && strcmp(word, tree->right->searchWord) > 0) {
        return rotateLeft(tree);
    }
    // Left right case
    if (balance > 1 && strcmp(word, tree->left->searchWord) > 0) {
        tree->left = rotateLeft(tree->left);
        return rotateRight(tree);
    }
    // Right left case
    if (balance < -1 && strcmp(word, tree->right->searchWord) < 0) {
        tree->right = rotateRight(tree->right);
        return rotateLeft(tree);
    }
    return tree;
}

Tree rotateRight(Tree root) {
    Tree newRoot = root->left;
    if (root == NULL || newRoot == NULL)
        return root;
    root->left = newRoot->right;
    newRoot->right = root;
    // Update heights
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
}

// Function to left rotate the subtree rooted with 'root'
Tree rotateLeft(Tree root) {
    Tree newRoot = root->right;
    if (root == NULL || newRoot == NULL)
        return root;
    root->right = newRoot->left;
    newRoot->left = root;
    // Update heights
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
}

void printInOrder(Tree tree) {
    if (tree == NULL) {
        return;
    }
    // Traverse the left subtree
    printInOrder(tree->left);
    // Print the current node's data
    printf("%s", tree->searchWord);
    int i = 0;
    for (List list = tree->urlList; list != NULL && i < tree->numberUrls; list = list->next) {
        printf(" %s", list->url);
        i++;
    }
    // Traverse the right subtree
    printf("\n");
    printInOrder(tree->right);
}


void freeTree(Tree tree) {
    if (tree == NULL) 
        return;
    // In order traversal
    freeTree(tree->left);
    freeTree(tree->right);
    List curr = tree->urlList;
    while (curr != NULL) {
        List tmp = curr;
        curr = curr->next; 
        free(tmp->url);
        free(tmp);
    }
    free(tree->searchWord);
    free(tree);
}

Tree searchNode(Tree tree, char *word) {
    if (tree == NULL) {
        return NULL;
    } else if (strcmp(word, tree->searchWord) < 0) {
        return searchNode(tree->left, word);
    } else if (strcmp(word, tree->searchWord) > 0) {
        return searchNode(tree->right, word);
    } else {
        return tree;
    }
}

int height(Tree tree) {
    if (tree == NULL)
        return -1;
    return tree->height;
}

int balance(Tree tree) {
    if (tree == NULL)
        return 0;
    return abs(height(tree->left) - height(tree->right));
}

void updateHeight(Tree tree) {
    if (tree == NULL)
        return;
    int leftHeight = -1;
    int rightHeight = -1;
    if (tree->left != NULL) {
        leftHeight = tree->left->height;
    }
    if (tree->right != NULL) {
        rightHeight = tree->right->height;
    }
    if (leftHeight > rightHeight) {
        tree->height = 1 + leftHeight;
    } else {
        tree->height = 1 + rightHeight;
    }
}

void newBstListNode(Tree tree, char *searchWord, char *fileName) {
    Tree node = searchNode(tree, searchWord);
    if (node == NULL) {
        return;
    }
    if (node->urlList == NULL) {
        node->urlList = createTreeList(fileName);
        node->numberUrls = 1;
    } else {
        List curr = node->urlList;
        while (curr != NULL) {
            if (strcmp(curr->url, fileName) == 0) {
                return;
            }
            if (curr->next == NULL) {
                curr->next = createTreeList(fileName);
                node->numberUrls++;
                return;
            }
            curr = curr->next;
        }
    }
}

void inOrderWrite(FILE *fileName, Tree tree) {
    if (tree == NULL)
        return;
    // First, visit the left subtree
    inOrderWrite(fileName, tree->left);
    // Then, do the work at the current node
    fprintf(fileName, "%s", tree->searchWord);
    for (List curr = tree->urlList; curr != NULL; curr = curr->next) {
        fprintf(fileName, " %s", curr->url);
    }
    // New line for each search word
    fprintf(fileName, "\n"); 
    inOrderWrite(fileName, tree->right);
}

/*
    Linked List 
*/

List createTreeList(char *url) {
    List list = malloc(sizeof(struct node));
	if (list == NULL) {
		fprintf(stderr, "Memory Allocation Failed!\n");
		exit(EXIT_FAILURE);
	}
    list->url = strdup(url);
    list->next = NULL;
    return list;
}

void listFree(List list) {
    if (list == NULL)
        return;
    while (list != NULL) {
        List temp = list;
        list = list->next;
        free(temp->url);
        free(temp);      
    }
}

List insertList(List list, char *url) {
    if (list == NULL) {
        fprintf(stderr, "List does not exist\n");
        exit(EXIT_FAILURE);
    }
    List newNode = createTreeList(url);
    if (list->next == NULL) {
        list->next = newNode;
    } else {
        // Traverse the list to find the last node
        List current = list;
        while (current->next != NULL) {
            current = current->next;
        }
        // Insert the new node at the end
        current->next = newNode;
    }
    return list;
}

int countList(List list) {
    int count = 0;
    for (List curr = list; curr != NULL; curr = curr->next) {
		count++;
	}
    return count;
}