#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//static inline int getchar_unlocked() { return _getchar_nolock(); }

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
void deleteAllCars(car *root);
car* min(car *node);
int maxLife(car *node);

// (STATIONS) List functions declaration
station* createStation(int distance, int carsNumber, int lives[]);
station* insertStation(station *s);
void deleteStation(int dist);
station* searchStation(int dist);

// Path planning function(s) declaration
void planPath(station *start, station *finish);

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
                        if(searchStation(dist) == NULL){
                            stat = insertStation(createStation(dist, num, lives));
                            printf("aggiunta\n");
                        } else {
                            printf("non aggiunta\n");
                        }
                        break;

                    case 'a':
                        if(scanf("%d %d", &dist, &life));
                        station *tmp = searchStation(dist);
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
                if(searchStation(dist) != NULL){
                    deleteStation(dist);
                    printf("demolita\n");
                } else {
                    printf("non demolita\n");
                }
                break;

            case 'r':
                if(scanf("%d %d", &dist, &life));
                station *tmp = searchStation(dist);
                if(tmp != NULL){
                    car *tmp1 = searchCar(tmp->root, life);
                    if(tmp1 != NULL) {
                        tmp->root = deleteCar(tmp->root, life);
                        printf("rottamata\n");
                    } else {
                        printf("non rottamata\n");
                    }
                } else {
                    printf("non rottamata\n");
                }
                break;

            case 'p':
                /*
                if(scanf("%d %d", &start, &finish));
                int *p = planPath(searchStation(start), searchStation(finish), &n);

                if(p){
                    for(int k = n - 1; k >= 0; k--)
                        printf("%d ", p[k]);
                    printf("\n");
                } else {
                    printf("nessun percorso\n");
                }
                */
                if(scanf("%d %d", &start, &finish));
                planPath(searchStation(start), searchStation(finish));
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

void deleteAllCars(car *root){
    if(root == NULL)
        return;

    deleteAllCars(root->l);
    deleteAllCars(root->r);

    free(root);
}

car* min(car *node) {
    car *current = node;
    while(current && current->l != NULL)
        current = current->l;
    return current;
}

int maxLife(car *node) {
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

station* insertStation(station *s) {
    if(stat == NULL){
        stat = s;
        s->next = NULL;
        s->prev = NULL;
        return stat;
    }

    if(s->dist < stat->dist){
        s->next = stat;
        s->prev = NULL;
        stat->prev = s;
        stat = s;
        return stat;
    }

    station *curr = stat;

    while(curr->next && curr->next->dist < s->dist){
        curr = curr->next;
    }

    s->prev = curr;
    s->next = curr->next;
    curr->next = s;

    if(s->next)
        s->next->prev = s;

    return stat;
}

void deleteStation(int dist) {
    station *temp = searchStation(dist);

    if(stat == NULL || temp == NULL)
        return;
    if(stat == temp)
        stat = temp->next;
    if(temp->next != NULL)
        temp->next->prev = temp->prev;
    if(temp->prev != NULL)
        temp->prev->next = temp->next;
    deleteAllCars(temp->root);
    free(temp);
}

station* searchStation(int dist) {
    station *current = stat;
    while (current != NULL) {
        if (current->dist == dist)
            return current;
        current = current->next;
    }
    return current;
}

void planPath(station *start, station *finish) {
    int num = 1;
    int *path = NULL;
    path = (int*)malloc(num * sizeof(int));

    station *curr, *tmp;

    if(start->dist < finish->dist){
        path[num - 1] = finish->dist;

        curr = start;
        tmp = finish;

        while(tmp != start){
            while(curr->dist + maxLife(curr->root) < tmp->dist){
                curr = curr->next;
            }
            if(curr != tmp){
                num++;
                path = realloc(path, num * sizeof(int));
                path[num - 1] = curr->dist;
                tmp = curr;
                curr = start;
            } else {
                free(path);
                printf("nessun percorso\n");
                return;
            }
        }

        for(int a = num - 1; a >= 0; a++)
            printf("%d ", path[a]);
        printf("\n");

        free(path);

        return;

    } else if(start->dist > finish->dist){
        path[num - 1] = start->dist;
        int size = 0, pos;
        station ***nodes = (station***)malloc(size * sizeof(station**));
        int *len = (int*)malloc(size * sizeof(int));

        curr = finish;
        tmp = curr->next;

        while(curr != start && tmp != start){
            size++;
            nodes = realloc(nodes, size * sizeof(station**));
            len = realloc(len, size * sizeof(int));
            pos = 0;
            nodes[size - 1] = (station**)malloc(pos * sizeof(station*));
            while(tmp != start){
                if(tmp->dist - maxLife(tmp->root) <= curr->dist){
                    pos++;
                    nodes[size - 1] = realloc(nodes[size - 1], pos * sizeof(station*));
                    nodes[size - 1][pos - 1] = tmp;
                    len[size - 1] = pos;
                }
                tmp = tmp->next;
            }
            if(!pos){
                free(path);

                /*
                for(int i = size - 1; i >= 0; i--){
                    for(int j = 0; j < len[i]; j++)
                        free(nodes[i][j]);
                    free(nodes[i]);
                }
                free(nodes[size]);
                free(nodes);
                */

                printf("nessun percorso\n");
                return;
            }
            curr = nodes[size - 1][pos - 1];
            tmp = curr->next;
        }

        size++;
        nodes = realloc(nodes, size * sizeof(station**));
        len = realloc(len, size * sizeof(int));
        nodes[size - 1] = (station**)malloc(sizeof(station*));
        nodes[size - 1][0] = start;
        len[size - 1] = 1;

        for(int i = size - 1, j = i - 1, found; i > 0; i--, j--, found = false){
            for(int k = 0; k < len[j] && !found; k++){
                for(int h = 0; h < len[i] && !found; h++){
                    if(nodes[i][h]->dist == path[num - 1] && nodes[i][h]->dist - maxLife(nodes[i][h]->root) <= nodes[j][k]->dist){
                        num++;
                        path = realloc(path, (num) * sizeof(int));
                        path[num - 1] = nodes[j][k]->dist;
                        found = true;
                    }
                }
            }
        }

        if(path[num - 1] - maxLife(searchStation(path[num - 1])->root) <= finish->dist){
            num++;
            path = realloc(path, (num) * sizeof(int));
            path[num - 1] = finish->dist;

            for(int a = 0; a < num; a++)
                printf("%d ", path[a]);
            printf("\n");

            free(path);

            /*
            for(int i = size - 1; i >= 0; i--){
                for(int j = 0; j < len[i]; j++)
                    free(nodes[i][j]);
                free(nodes[i]);
            }
            free(nodes[size]);
            free(nodes);
            */

            return;

        } else {

            free(path);

            /*
            for(int i = size - 1; i >= 0; i--){
                for(int j = 0; j < len[i]; j++)
                    free(nodes[i][j]);
                free(nodes[i]);
            }
            free(nodes[size]);
            free(nodes);
            */

            printf("nessun percorso\n");
            return;
        }

    } else {
        //base case
        free(path);
        printf("%d\n", start->dist);
        return;
    }
}