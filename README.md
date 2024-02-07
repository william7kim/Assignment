# Assignment
Pagerank/InvertedIndex/Search Engine project in C

This project heavily uses file i/o in C.

Pagerank.c : Basically calculates the pagerank in a DFS traversal from source.

invertedIndex.c : Essentially parses the txt files in Collection.txt and goes to the 
specific url.txt file and parses from there. After which normalises the word and stores
it inside a binary search tree with each nodes branches out into a linked list all of 
which is setup in a way for an AVL tree.

Search Engine: Once done with the both invertedIndex.c and Pagerank.c and created there
respective .txt files we then go through the information and rank the url pages. We are
given pagerank, number of outgoing links, and the name of the url. alongside what url is the
specific word that showws up.
