//
//  treemain.h
//  BPlusTree
//
//  Created by Kimleng Hor on 4/16/23.
//

#ifndef tree_h
#define tree_h

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define MIN_SEARCH_KEY 100000
#define MAX_SEARCH_KEY 200000
#define NUM_SEARCH_KEYS 10000

typedef struct tree_node {
    int length;
    struct tree_node *next;
    bool leaf_node;
    void **p;
    int *keys;
    struct tree_node *parent;
} tree_node;

int *search_keys;
tree_node *linked_list = NULL;
int tree_order;

int contains(int* arr, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return 1;
        }
    }
    return 0;
}

void generateData(void) {
    
    search_keys = (int*) malloc(NUM_SEARCH_KEYS * sizeof(int));
    
    int num_keys = 0;
    while (num_keys < NUM_SEARCH_KEYS) {
        int randomKey = rand() % (MAX_SEARCH_KEY - MIN_SEARCH_KEY + 1) + MIN_SEARCH_KEY;
        if (!contains(search_keys, num_keys, randomKey)) {
            search_keys[num_keys] = randomKey;
            num_keys++;
        }
    }
}

void setupSparseTree(int number) {
    tree_order = (number / 2) + 1;
}

void setupDenseTree(int number) {
    tree_order = number + 1;
}

tree_node *addToParent(tree_node *tree, int key, tree_node *left_node, tree_node *right_node) {
    int index = 0;
    int split_point = (int) ceil((double) tree_order / 2);
    int split_key;
    
    tree_node *new_tree;
    tree_node *child;
    
    int *parent_keys;
    tree_node **parent_pointers;
    tree_node *parent;
    
    int i = 0;
    int j = 0;
    
    parent_pointers = malloc((tree_order + 1) * sizeof(tree_node *));
    parent_keys = malloc(tree_order * sizeof(int));

    parent = left_node->parent;

    if (parent == NULL) {
        new_tree = malloc(sizeof(tree_node));
        new_tree->keys = malloc((tree_order - 1) * sizeof(int));
        new_tree->p = malloc(tree_order * sizeof(void *));
        new_tree->leaf_node = false;
        new_tree->keys[0] = key;
        new_tree->p[0] = left_node;
        new_tree->p[1] = right_node;
        new_tree->length++;
        new_tree->parent = NULL;
        left_node->parent = new_tree;
        right_node->parent = new_tree;
        return new_tree;
    }

    for (; index <= parent->length && parent->p[index] != left_node; index++) {}

    if (parent->length >= tree_order - 1) {
        while (i < parent->length + 1) {
            if (j == index + 1) {
                j++;
            }
            parent_pointers[j] = parent->p[i];
            i++;
            j++;
        }

        i = 0;
        j = 0;
        while (i < parent->length) {
            if (j == index) {
                j++;
            }
            parent_keys[j] = parent->keys[i];
            i++;
            j++;
        }

        parent_pointers[index + 1] = right_node;
        parent_keys[index] = key;
        
        new_tree = malloc(sizeof(tree_node));
        new_tree->keys = malloc((tree_order - 1) * sizeof(int));
        new_tree->p = malloc(tree_order * sizeof(void *));
        new_tree->leaf_node = false;
        new_tree->length = 0;
        new_tree->parent = NULL;
        new_tree->next = NULL;
        
        parent->length = 0;
        
        i = 0;
        while (i < split_point - 1) {
            parent->p[i] = parent_pointers[i];
            parent->keys[i] = parent_keys[i];
            parent->length++;
            i++;
        }
        
        parent->p[i] = parent_pointers[i];
        split_key = parent_keys[split_point - 1];
        
        j = 0;
        i++;
        while (i < tree_order) {
            new_tree->p[j] = parent_pointers[i];
            new_tree->keys[j] = parent_keys[i];
            new_tree->length++;
            i++;
            j++;
        }
        
        new_tree->p[j] = parent_pointers[i];
        new_tree->parent = parent->parent;
        
        i = 0;
        while (i <= new_tree->length) {
            child = new_tree->p[i];
            child->parent = new_tree;
            i++;
        }
        
        return addToParent(tree, split_key, parent, new_tree);
    } else {
        int i = parent->length;
        while (i > index) {
            parent->p[i + 1] = parent->p[i];
            parent->keys[i] = parent->keys[i - 1];
            i--;
        }
        parent->p[index + 1] = right_node;
        parent->keys[index] = key;
        parent->length++;
        
        return tree;
    }
}

//search for the value
tree_node *search(tree_node *const tree, int key, bool isSearching) {
    bool is_found = false;
    tree_node *node = tree;
  
    for (int i = 0; node->leaf_node == false; node = (tree_node *)node->p[i]) {
        for ( i = 0; i < node->length && key >= node->keys[i]; i++) {}
    }
  
    if (isSearching) {
        printf("\n");
        for (int i = 0; i < node->length; i++) {
            if (node->keys[i] == key) {
                is_found = true;
                printf("%d exists in this tree", key);
                break;
            }
        }
        
        if (is_found == false) {
            printf("%d does not exist in this tree\n", key);
        }
    }
    
    return node;
}

tree_node *insertion(tree_node *tree, int key) {
    
    tree_node *new_node;
    new_node = malloc(sizeof(tree_node));
    new_node->p = malloc(tree_order * sizeof(void *));
    new_node->keys = malloc((tree_order - 1) * sizeof(int));
    new_node->leaf_node = true;
    
    int *parent_keys;
    void **parent_pointers;
    
    int index = 0;
    int split = (int) ceil((double) (tree_order - 1) / 2);
    int new_key;
    int i;
    int j;
    
    parent_keys = malloc(tree_order * sizeof(int));
    parent_pointers = malloc(tree_order * sizeof(void *));
    
    if (tree == NULL) {
        new_node->length++;
        new_node->leaf_node = true;
        new_node->keys[0] = key;
        return new_node;
    }
    
    tree_node *leaf = search(tree, key, false);

    if (leaf->length >= tree_order - 1) {
        for (; index < tree_order - 1 && leaf->keys[index] < key; index++);

        i = 0;
        j = 0;
        while (i < leaf->length) {
            if (j == index) {
                j++;
            }
            parent_keys[j] = leaf->keys[i];
            parent_pointers[j] = leaf->p[i];
            i++;
            j++;
        }

        parent_keys[index] = key;
        leaf->length = 0;

        i = 0;
        while (i < split) {
            leaf->p[i] = parent_pointers[i];
            leaf->keys[i] = parent_keys[i];
            leaf->length++;
            i++;
        }

        i = split;
        j = 0;
        while (i < tree_order) {
            new_node->p[j] = parent_pointers[i];
            new_node->keys[j] = parent_keys[i];
            new_node->length++;
            i++;
            j++;
        }

        new_node->p[tree_order - 1] = leaf->p[tree_order - 1];
        new_node->parent = leaf->parent;
        leaf->p[tree_order - 1] = new_node;
        new_key = new_node->keys[0];

        return addToParent(tree, new_key, leaf, new_node);
    } else {
        for (index = 0; index < leaf->length && leaf->keys[index] < key; index++) {}

        i = leaf->length;
        while (i > index) {
            leaf->keys[i] = leaf->keys[i - 1];
            leaf->p[i] = leaf->p[i - 1];
            i--;
        }
        
        leaf->keys[index] = key;
        leaf->length++;
        
        return tree;
    }
}

void rangeSearch(tree_node *const tree, int begin, int end) {
    int numbers = 0;
    int keys_found[end - begin + 1];
    int order = tree_order - 1;
    
    tree_node *node = search(tree, begin, false);
  
    while (node) {
        for (int i = 0; i < node->length; i++) {
            if (node->keys[i] >= begin && node->keys[i] <= end) {
                keys_found[numbers++] = node->keys[i];
            }
        }
        node = node->p[order];
    }
    
    printf("\nKeys that are found in this range: ");
    for (int i = 0; i < numbers; i++) {
        printf("%d, ", keys_found[i]);
    }
    printf("\n");
}

tree_node *deletion(tree_node *tree, int key) {
    tree_node *leaf = NULL;
    leaf = search(tree, key, false);
    for (int i = 0; i < leaf->length; i++) {
        if (leaf->keys[i] == key) {
            leaf->keys[i] = leaf->keys[i + 1];
            leaf->length--;
            break;
        }
    }
    
    return tree;
}

//add the new node into the linked list
void addToTail(tree_node *new_node) {
    tree_node *tree;
    if (linked_list != NULL) {
        tree = linked_list;
        for (; tree->next != NULL; tree = tree->next) {}
        tree->next = new_node;
        new_node->next = NULL;
    } else {
        linked_list = new_node;
    }
}

//show tree in the console
void showBTree(tree_node *tree) {
    int level = 0;
    int next_level = 0;
    tree_node *node = NULL;

    linked_list = NULL;
    addToTail(tree);
    
    printf("--> ");
    while (linked_list != NULL) {
        next_level = 0;
        
        //remove from the head
        tree_node *temp_node = linked_list;
        linked_list = linked_list->next;
        node = temp_node;
        
        if (node->parent != NULL) {
            tree_node *child = node;
            while (child != tree) {
                child = child->parent;
                next_level++;
            }
            
            if (next_level != level) {
                level = next_level;
                printf("\n--> ");
            }
        }
        
        int i = 0;
        printf("[ ");
        while (i < node->length) {
            printf("%d ", node->keys[i]);
            i++;
        }
        
        if (node->leaf_node == false) {
            int j = 0;
            while (j <= node->length) {
                addToTail(node->p[j]);
                j++;
            }
        }

        printf("] ");
    }
}

#endif /* tree_h */
