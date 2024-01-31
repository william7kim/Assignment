// COMP2521 Assignment 1: qsort.c
//
// Stdlib has a qsort() function that takes in
// an array, the length of array indices, how big/type
// are the bytes of the array, and special function that 
// you need to create. In this file stcompare compares by 
// what is returned by strcmp.
// My guess is that this is a quicksort impmentation that
// is built into the standard libaries for C
// I first saw it being implmented by Kevin Browne and then iterated
// upon to use strcmp, struct array sort and etc the qsort special function
// not mentioned here have static void in them to prevent them from being used
// in other c files.
//
// Youtube video of how to use qsort() 
// Author of which is Kevin Browne from Portfolio Courses
// https://www.youtube.com/watch?v=rHoOWG6Ihs4&t=8s&pp=ygUFcXNvcnQ%3D
// Website: https://portfoliocourses.com/
//
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// qsort(a, length, sizeof(char *), compare);
// use in tandem with files.c. 

int strcompare(const void *x_void, const void *y_void) {
    // Cast the void pointers to pointers to const char
    const char *x = *(const char **)x_void;
    const char *y = *(const char **)y_void;
    // Compare the strings
    return strcmp(x, y);
}
