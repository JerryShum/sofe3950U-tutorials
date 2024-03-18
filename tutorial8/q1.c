#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256 

struct proc {
    char parent[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    int priority;
    int memory;
};

struct proc_tree {
    struct proc process;
    struct proc_tree *left;
    struct proc_tree *right;
};

struct proc_tree *tree;

void add_node(struct proc_tree **root, struct proc_tree *node) {
    if (*root == NULL) {
        *root = node;
    } else {
        struct proc_tree *current = *root;
        do {
            if (strcmp(node->process.name, current->process.name) < 0) {
                if (current->left == NULL) {
                    current->left = node;
                    break;
                } else {
                    current = current->left;
                }
            } else {
                if (current->right == NULL) {
                    current->right = node;
                    break;
                } else {
                    current = current->right;
                }
            }
        } while(root != NULL);
    }
}

void populate_proc_tree() {
    FILE *file = fopen("processes_tree.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open file\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        struct proc *process = malloc(sizeof(struct proc));
        char *token = strtok(buffer, " ");
        strcpy(process->parent, token);
        token = strtok(NULL, " ");
        strcpy(process->name, token);
        token = strtok(NULL, " ");
        process->priority = atoi(token);
        token = strtok(NULL, " ");
        process->memory = atoi(token);

        struct proc_tree *node = malloc(sizeof(struct proc_tree));
        node->process = *process;
        node->left = NULL;
        node->right = NULL;

        add_node(&tree, node);
    }

    fclose(file);
}

void print_proc_tree_preorder(struct proc_tree *node) {
    if (node == NULL) {
        return;
    }
    printf("Parent: %s Name: %s Priority: %d, Memory: %d\n", node->process.parent, node->process.name, node->process.priority, node->process.memory);
    print_proc_tree_preorder(node->left);
    print_proc_tree_preorder(node->right);
}

int main(int argc, char **argv) {
    populate_proc_tree();
    print_proc_tree_preorder(tree);
    return 0;
}