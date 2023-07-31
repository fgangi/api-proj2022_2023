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
car* newCar(int life);
car* insertCar(car *node, int life);
car* searchCar(car *root, int life);
car* deleteCar(car *root, int life);
car* min(car *node);


// (STATIONS) List functions declaration
station* createStation(int distance, int carsNumber, int lives[]);
station* insertStation(station *head, station *s);
void deleteStation(station **head, int dist);
station* searchStation(station *head, int dist);

// Path planning functions declaration
void checkPath(int start, int finish);
void planPath(station *start, station *finish);

// stations global pointer
station *stat = NULL;

int main() {

    int i = 0, lives[512], dist, num, life, start, finish;

    char cmd[20];

    //cmd[i] = getchar_unlocked();

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
            for(int j = 0; j < num; j++){
                if(scanf("%d", &lives[j]) != '\n');
            }
            if(searchStation(stat, dist) == NULL){
                station *s = createStation(dist, num, lives);
                stat = insertStation(stat, s);
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }

        } else if(!strcmp(cmd, plan)){
            if(scanf("%d %d", &start, &finish));
            checkPath(start, finish);

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
            car *tmp1 = searchCar(tmp->root, life);
            if(tmp != NULL && tmp1 != NULL){
                tmp->root = deleteCar(tmp->root, life);
                printf("rottamata\n");
            } else {
                printf("non rottamata\n");
            }
        } else if(!strcmp(cmd, addC)){
            if(scanf("%d %d", &dist, &life));
            station *tmp = searchStation(stat, dist);
            if(tmp != NULL){
                tmp->root = insertCar(tmp->root, life);
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }
        }
    }

    return 0;
}

car* newCar(int life) {
    car* temp = (car*)malloc(sizeof(car));
    temp->life = life;
    temp->l = temp->r = NULL;
    return temp;
}

car* insertCar(car *node, int life) {
    if(node == NULL)
        return newCar(life);

    if(life <= node->life)
        node->l = insertCar(node->l, life);
    else if(life > node->life)
        node->r = insertCar(node->r, life);

    return node;
}

car* searchCar(car *root, int life) {
    if(root == NULL || root->life == life)
        return root;

    if(root->life < life)
        return searchCar(root->r, life);

    return searchCar(root->l, life);
}

car* deleteCar(car *root, int life) {
    if(root == NULL)
        return root;

    if(life < root->life)
        root->l = deleteCar(root->l, life);

    else if(life > root->life)
        root->r = deleteCar(root->r, life);

    else {
        if(root->l == NULL) {
            car *temp = root->r;
            free(root);
            return temp;
        }
        else if(root->r == NULL) {
            car *temp = root->l;
            free(root);
            return temp;
        }

        car *temp = min(root->r);

        root->life = temp->life;

        root->r = deleteCar(root->r, temp->life);
    }
    return root;
}

car* min(car *node) {
    car *current = node;
    while(current && current->l != NULL)
        current = current->l;
    return current;
}

int max(car *node) {
    car* current = node;
    while (current && current->r != NULL)
        current = current->r;
    return (current->life);
}

station* createStation(int distance, int carsNumber, int lives[]) {
    station *s = (station*)malloc(sizeof(station));
    car *cars = NULL;
    s->dist = distance;
    for(int i = 0; i < carsNumber; i++){
        cars = insertCar(cars, lives[i]);
    }
    s->root = cars;
    return s;
}

station* insertStation(station *head, station *s) {
    if(head && head->dist <= s->dist) {
        if(head->dist < s->dist)
            head->next = insertStation(head->next, s);
        return head;
    }

    station* tmp = (station*)malloc(sizeof(station));
    tmp->dist = s->dist;
    tmp->next = head;
    tmp->root = s->root;
    return tmp;
}

void deleteStation(station **head, int dist) {
    station *temp = *head, *prev;

    if(temp != NULL && temp->dist == dist){
        *head = temp->next;
        free(temp);
        return;
    }

    while(temp != NULL && temp->dist != dist){
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL)
        return;

    prev->next = temp->next;

    free(temp);
}

station* searchStation(station *head, int dist) {
    station *current = head;
    while (current != NULL) {
        if (current->dist == dist)
            return current;
        current = current->next;
    }
    return current;
}

void checkPath(int start, int finish){
    if(start == finish){
        printf("%d\n", start);
    } else {
        planPath(searchStation(stat, start), searchStation(stat, finish));
    }
}

void planPath(station *start, station *finish) {
    station *curr = start;
    int m = max(curr->root);
    if(start->dist < finish->dist){
        station **nodes = (station**)malloc(sizeof(station*));
        station *tmp = curr;
        for(int i = curr->dist; i <= curr->dist + m && tmp->next->dist <= curr->dist + m; i += tmp->next->dist, tmp = tmp->next){
            *nodes = insertStation(*nodes, tmp->next);
        }


    }
}