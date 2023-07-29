#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *addS = "aggiungi-stazione";
static const char *addC = "aggiungi-auto";
static const char *destroyS = "demolisci-stazione";
static const char *destroyC = "rottama-auto";
static const char *plan = "pianifica-percorso";

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

// (CARS) BST functions declaration
car* newCar(int key);
car* insertCar(car *node, int key);
car* searchCar(car *root, int key);
car* deleteCar(car *root, int key);

// (STATIONS) List functions declaration
void insertStation(station *prevNode, int data, car *cars);
void deleteStation(station **head, int key);
station* searchStation(station *head, int data);

int main() {

    int i = 0, lives[512], dist, num, life;

    char cmd[20];

    //cmd[i] = getchar_unlocked();

    station *stat = NULL;

    while(scanf("%s", cmd) != EOF){

        /*
        while((cmd[i] >= 'a' && cmd[i] <= 'z') || cmd[i] == '-') {
            cmd[i] = getchar_unlocked();
            i++;
        }


        cmd[i] = '\0';

        */

        if(!strcmp(cmd, addS)){
            if(scanf("%d %d", &dist, &num));
            car *cars = NULL;
            for(int j = 0; j < num; j++){
                if(scanf("%d", &lives[j]) != '\n');
                cars = insertCar(cars, lives[j]);
            }
            insertStation(stat, dist, cars);

        } else if(!strcmp(cmd, plan)){

        } else if(!strcmp(cmd, destroyS)){
            if(scanf("%d", &dist));
            if(searchStation(stat, dist) != NULL){
                deleteStation(&stat, dist);
                printf("demolita\n");
            } else {
                printf("non demolita\n");
            }
        } else if(!strcmp(cmd, destroyC)){
            if(scanf("%d %d", &dist, &life));
            station *tmp = searchStation(stat, dist);
            if(tmp != NULL){
                deleteCar(tmp->root, life);
                printf("rottamata\n");
            } else {
                printf("non rottamata\n");
            }
        } else if(!strcmp(cmd, addC)){
            if(scanf("%d %d", &dist, &life));
            if(searchStation(stat, dist) != NULL){
                stat->root = insertCar(stat->root, life);
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }
        }
}



    return 0;
}

car* newCar(int key) {
    car* temp = (car*)malloc(sizeof(car));
    temp->life = key;
    temp->l = temp->r = NULL;
    return temp;
}

car* insertCar(car *node, int key) {
    if (node == NULL)
        return newCar(key);

    if (key < node->life)
        node->l = insertCar(node->l, key);
    else if (key > node->life)
        node->r = insertCar(node->r, key);

    return node;
}

car* searchCar(car *root, int key) {
    if (root == NULL || root->life == key)
        return root;

    if (root->life < key)
        return searchCar(root->r, key);

    return searchCar(root->l, key);
}

car* deleteCar(car *root, int key) {
    // Base case
    if (root == NULL)
        return root;

    // Recursive calls for ancestors of
    // node to be deleted
    if (root->life > key) {
        root->l = deleteCar(root->l, key);
        return root;
    } else if (root->life < key) {
        root->r = deleteCar(root->r, key);
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

void insertStation(station *prevNode, int data, car *cars) {
    if(prevNode == NULL) {
        //printf("the given previous node cannot be NULL");
        return;
    }

    station *newNode = (station*)malloc(sizeof(station));

    newNode->dist = data;
    newNode->root = cars;
    newNode->next = prevNode->next;
    prevNode->next = newNode;
}

void deleteStation(station **head, int key) {
    // Store head node
    station *temp = *head, *prev;

    // If head node itself holds the key to be deleted
    if(temp != NULL && temp->dist == key){
        *head = temp->next; // Changed head
        free(temp); // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while(temp != NULL && temp->dist != key){
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if(temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
}

station* searchStation(station *head, int data) {
    station *current = head;
    while (current != NULL) {
        if (current->dist == data)
            return current;
        current = current->next;
    }
    return current;
}