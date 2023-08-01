#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static inline int getchar_unlocked() {
    return _getchar_nolock();
}

typedef struct treeNode {
    int life; // battery life
    struct treeNode *r;
    struct treeNode *l;
} car;

typedef struct listNode {
    //struct listNode *prev;
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
int* planPath(station *start, station *finish, int *numS);

// stations global pointer
station *stat = NULL;

int main() {

    int i = 0, lives[512], dist, num, life, start, finish, n;
    char c;

    char cmd[20];

    while(scanf("%s", cmd) != EOF){

        switch(cmd[0]) {
            case 'a':
                switch(cmd[9]){
                    case 's':
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
                        break;

                    case 'a':
                        if(scanf("%d %d", &dist, &life));
                        station *tmp = searchStation(stat, dist);
                        if(tmp != NULL){
                            tmp->root = insertCar(tmp->root, life);
                            printf("aggiunta\n");
                        } else {
                            printf("non aggiunta\n");
                        }
                        break;
                }
                break;

            case 'd':
                if(scanf("%d", &dist));
                if(searchStation(stat, dist) != NULL){
                    deleteStation(&stat, dist);
                    printf("demolita\n");
                } else {
                    printf("non demolita\n");
                }
                break;

            case 'r':
                if(scanf("%d %d", &dist, &life));
                station *tmp = searchStation(stat, dist);
                car *tmp1 = searchCar(tmp->root, life);
                if(tmp != NULL && tmp1 != NULL){
                    tmp->root = deleteCar(tmp->root, life);
                    printf("rottamata\n");
                } else {
                    printf("non rottamata\n");
                }
                break;

            case 'p':
                if(scanf("%d %d", &start, &finish));
                int *p = planPath(searchStation(stat, start), searchStation(stat, finish), &n);

                if(p){
                    for(int k = 0; k < n; k++)
                        printf("%d ", p[k]);
                    printf("\n");
                } else {
                    printf("nessun percorso\n");
                }
                break;
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

int* planPath(station *start, station *finish, int *numS) {
    *numS = 1;
    int *path = NULL;
    int *invPath = NULL;
    bool noPath = false;

    station *curr;
    station *tmp = finish;

    path = (int*)malloc((*numS) * sizeof(int));
    path[*numS - 1] = finish->dist;

    if(start->dist < finish->dist){
        while(tmp != start && !noPath){
            curr = start;
            if(curr->dist + max(curr->root) >= curr->next->dist){
                while(curr->dist + max(curr->root) < tmp->dist){
                    curr = curr->next;
                }
                (*numS)++;
                path = realloc(path, (*numS) * sizeof(int));
                path[(*numS) - 1] = curr->dist;
                tmp = curr;
            } else {
                noPath = true;
            }
        }
        if(noPath){
            free(path);
            return NULL;
        } else {
            invPath = (int*)malloc((*numS) * sizeof(int));
            for(int i = 0; i < *numS; i++)
                invPath[i] = path[(*numS) - i - 1];
            free(path);
            return invPath;
        }

    } else if(start->dist > finish->dist){
        while(tmp != start && !noPath){
            curr = start;
            while(tmp->dist - max(tmp->root) > curr->dist){
                curr = curr->next;
            }
            if(tmp->dist - max(tmp->root) > curr->dist){
                (*numS)++;
                path = realloc(path, (*numS) * sizeof(int));
                path[(*numS) - 1] = curr->dist;
                tmp = curr;
            } else {
                noPath = true;
            }
        }
        if(noPath){
            return NULL;
        } else {
            return path;
        }
    }

    return path;
}