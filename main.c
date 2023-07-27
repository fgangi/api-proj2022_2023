#include <stdio.h>
#include <stdlib.h>

static inline int getchar_unlocked() {
    return _getchar_nolock();
}

typedef struct treeNode {
    int life; // battery life
    struct treeNode *r;
    struct treeNode *l;
} car;

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    struct treeNode *root;
    int dist; // distance
} station;

// BST functions declaration
car* newTreeNode(int key);
car* insertBST(car *node, int key);
car* searchBST(car *root, int key);
car* deleteTreeNode(car *root, int key);

// List functions declaration
void insertList(station *prevNode, int data, car *cars);
void deleteListNode(station **head, station *temp);

int main() {

    int i = 0;

    char *cmd;

    cmd = malloc(sizeof(char)*20);

    if(cmd){
        while((cmd[i] = getchar_unlocked()) != '\n') {
            i++;
        }
        cmd[i] = '\0';
    }

    return 0;
}

car* newTreeNode(int key) {
    car* temp = (car*)malloc(sizeof(car));
    temp->life = key;
    temp->l = temp->r = NULL;
    return temp;
}

car* insertBST(car *node, int key) {
    if (node == NULL)
        return newTreeNode(key);

    if (key < node->life)
        node->l = insertBST(node->l, key);
    else if (key > node->life)
        node->r = insertBST(node->r, key);

    return node;
}

car* searchBST(car *root, int key) {
    if (root == NULL || root->life == key)
        return root;

    if (root->life < key)
        return searchBST(root->r, key);

    return searchBST(root->l, key);
}

car* deleteTreeNode(car *root, int key) {
    // Base case
    if (root == NULL)
        return root;

    // Recursive calls for ancestors of
    // node to be deleted
    if (root->life > key) {
        root->l = deleteTreeNode(root->l, key);
        return root;
    } else if (root->life < key) {
        root->r = deleteTreeNode(root->r, key);
        return root;
    }

    // We reach here when root is the node
    // to be deleted.

    // If one of the children is empty
    if (root->l == NULL) {
        car *temp = root->r;
        free(root);
        return temp;
    } else if (root->r == NULL) {
        car *temp = root->l;
        free(root);
        return temp;
    }

    // If both children exist
    else {

        car *succParent = root;

        // Find successor
        car *succ = root->r;
        while (succ->l != NULL) {
            succParent = succ;
            succ = succ->l;
        }

        // Delete successor.  Since successor
        // is always left child of its parent
        // we can safely make successor's right
        // right child as left of its parent.
        // If there is no succ, then assign
        // succ->right to succParent->right
        if (succParent != root)
            succParent->l = succ->r;
        else
            succParent->r = succ->r;

        // Copy Successor Data to root
        root->life = succ->life;

        // Delete Successor and return root
        free(succ);
        return root;
    }
}

void insertList(station *prevNode, int data, car *cars) {
    if(prevNode == NULL) {
        printf("the given previous node cannot be NULL");
        return;
    }

    station *newNode = (station*)malloc(sizeof(station));

    newNode->dist = data;
    newNode->root = cars;
    newNode->next = prevNode->next;
    prevNode->next = newNode;
}

void deleteListNode(station **head, station *temp) {
    if (*head == NULL || temp == NULL)
        return;
    if (*head == temp)
        *head = temp->next;
    if (temp->next != NULL)
        temp->next->prev = temp->prev;
    if (temp->prev != NULL)
        temp->prev->next = temp->next;
    free(temp);
    return;
}