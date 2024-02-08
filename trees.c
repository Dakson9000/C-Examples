#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
  File: server.c
  Author: Mason Miles
	Description: This file contains a simple implementation of a binary search tree
  including functions to insert, delete, and search for values in the tree. There is
  main function to test the tree implementation.
  Usage: ./trees
*/

// Define a structure for a binary tree node
typedef struct node {
  int value;
  struct node *left;  // Pointer to the left child node
  struct node *right; // Pointer to the right child node
} * Tree;             // Tree is a pointer to a node

//  print the tree in-order
void print_tree(Tree t) {
  if (t == NULL) {
    return; // Base case: if tree is empty, return
  }
  // Recursively print left subtree, then current node's value, then right
  // subtree
  print_tree(t->left);
  printf(" %d ", t->value);
  print_tree(t->right);
}

//  Create a new tree node with given value, left, and right subtrees
Tree make_node(int value, Tree left, Tree right) {
  Tree t = (Tree)malloc(sizeof(struct node)); // Allocate memory for new node
  t->value = value;                           // Set node's value and children
  t->left = left;
  t->right = right;
  return t; // Return the new node
}

//  check if a value exists in the tree
bool lookup(int value, Tree t) {
  if (t == NULL) {
    return false; // Base case: if tree is empty, value not found
  } else if (value < t->value) {
    return lookup(value, t->left); // Recursively search left subtree
  } else if (value > t->value) {
    return lookup(value, t->right); // Recursively search right subtree
  } else {
    return true; // Value found
  }
}

//  find the smallest value in a tree
int find_smallest(Tree t) {
  if (t->left == NULL) {
    return t->value; // Base case: if no left child, current node is smallest
  }
  return find_smallest(t->left); // Recursively search left subtree
}

//  delete a value from the tree
void delete_value(int value, Tree *pt) {
  if (*pt == NULL) {
    return; // Base case: if tree is empty, do nothing
  } else if (value < (*pt)->value) {
    delete_value(value, &(*pt)->left); // Recursively delete from left subtree
  } else if (value > (*pt)->value) {
    delete_value(value, &(*pt)->right); // Recursively delete from right subtree
  } else {
    // Current node must be deleted
    Tree temp = *pt;
    if ((*pt)->left == NULL) {
      *pt = (*pt)->right; // Replace current node with its right child
    } else if ((*pt)->right == NULL) {
      *pt = (*pt)->left; // Replace current node with its left child
    } else {             // If node has both left and right children
      int smallest =
          find_smallest((*pt)->right); // Find smallest value in right subtree
      delete_value(smallest,
                   &(*pt)->right); // Delete smallest value from right subtree
      *pt = make_node(smallest, (*pt)->left,
                      (*pt)->right); // Replace current node with smallest value
    }
    free(temp); // Free memory for deleted node
  }
}

//  insert a value into the tree
void insert_value(int value, Tree *pt) {
  if (*pt == NULL) {
    *pt =
        make_node(value, NULL, NULL); // If tree is empty, create new root node
  } else if (value < (*pt)->value) {
    insert_value(value, &((*pt)->left)); // Recursively insert into left subtree
  } else {
    insert_value(value,
                 &((*pt)->right)); // Recursively insert into right subtree
  }
}

//  delete the entire tree and free memory
void delete_tree(Tree *t) {
  if (*t == NULL) {
    return; // Base case: if tree is empty, do nothing
  }
  // Recursively delete right and left subtrees, then free memory for current
  // node
  delete_tree(&(*t)->right);
  delete_tree(&(*t)->left);
  free(*t);
  *t = NULL; // Set pointer to NULL after freeing memory
}

int main() {
  // Create an empty tree
  Tree t = NULL;

  // Test insert_value
  printf("Inserting values into the tree...\n");
  insert_value(42, &t);
  insert_value(13, &t);
  insert_value(15, &t);
  insert_value(60, &t);
  printf("Tree after insertion:\n");
  print_tree(t);
  printf("\n\n");

  // Test lookup
  printf("Looking up values in the tree...\n");
  int value_to_lookup = 15;
  if (lookup(value_to_lookup, t)) {
    printf("%d is in the tree.\n", value_to_lookup);
  } else {
    printf("%d is not in the tree.\n", value_to_lookup);
  }
  value_to_lookup = 100;
  if (lookup(value_to_lookup, t)) {
    printf("%d is in the tree.\n", value_to_lookup);
  } else {
    printf("%d is not in the tree.\n", value_to_lookup);
  }
  printf("\n");

  // Test delete_value
  printf("Deleting values from the tree...\n");
  int value_to_delete = 42;
  printf("Deleting %d from the tree...\n", value_to_delete);
  delete_value(value_to_delete, &t);
  printf("Tree after deletion:\n");
  print_tree(t);
  printf("\n");

  // Test delete_tree
  printf("Deleting the entire tree...\n");
  delete_tree(&t);
  printf("Tree after deletion:\n");
  print_tree(t); // Should print nothing as the tree is empty

  return 0;
}
