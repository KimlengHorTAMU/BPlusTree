//
//  main.c
//  BPlusTree
//
//  Created by Kimleng Hor on 4/16/23.
//
#include "treemain.h"

int main(int argc, char* arv[]) {
    
    //sparse
//    setupSparseTree(6);
    
    //dense
    setupDenseTree(4);
    
    //create a root and initialize to null
    tree_node *bTree = NULL;

    //insert into the tree
    bTree = insertion(bTree, 7);
    bTree = insertion(bTree, 10);
    bTree = insertion(bTree, 1);
    bTree = insertion(bTree, 23);
    bTree = insertion(bTree, 5);
    bTree = insertion(bTree, 15);
    bTree = insertion(bTree, 17);
    bTree = insertion(bTree, 9);
    bTree = insertion(bTree, 11);
    bTree = insertion(bTree, 39);
    bTree = insertion(bTree, 35);
    bTree = insertion(bTree, 8);
    bTree = insertion(bTree, 40);
    bTree = insertion(bTree, 25);
    
//    //generate the data
//    generateData();
//
//    //insert into b+tree
//    for (int i = 0; i < NUM_SEARCH_KEYS; i++) {
//        bTree = insertion(bTree, search_keys[i]);
//    }
    
    //show the tree in the console
    showBTree(bTree);
    
    //search
    search(bTree, 35, true);
    search(bTree, 1, true);
    search(bTree, 2, true);
    search(bTree, 5, true);
    search(bTree, 98, true);
    search(bTree, 99, true);
    
    rangeSearch(bTree, 1, 99);
    
//    delete nodes
    bTree = deletion(bTree, 1);
    bTree = deletion(bTree, 7);
    bTree = deletion(bTree, 11);
    bTree = deletion(bTree, 96);
    bTree = deletion(bTree, 40);
    showBTree(bTree);
    
    printf("\n");
}


