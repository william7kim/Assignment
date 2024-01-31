#ifndef BST_H
#define BST_H

typedef struct node *Tree;
typedef struct list *List;

Tree createTree(char *url);

Tree treeInsert(Tree tree, char *word);

Tree insertAVL(Tree tree, char *word);

Tree rotateRight(Tree root);

Tree rotateLeft(Tree root);

void print(Tree t, int depth);

void printInOrder(Tree tree);

void freeTree(Tree tree);

Tree searchNode(Tree tree, char *word);

int height(Tree tree);

int balance(Tree tree);

void updateHeight(Tree tree);

// insert in the linked list
void newBstListNode(Tree tree, char *searchWord, char *fileName);

void inOrderWrite(FILE *fileName, Tree tree);

/*
    Linked List 
*/
List createTreeList(char *url);

void listFree(List list);

List insertList(List list, char *url);

int countList(List l);



#endif
