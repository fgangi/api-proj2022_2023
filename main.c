#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct treeNode {
    int life; // battery life
    struct treeNode *r;
    struct treeNode *l;
} car;

typedef struct listNode {
    struct listNode *next;
    struct treeNode *root;
    int dist; // distance
    struct listNode *l;
    struct listNode *r;
} station;


// Cars BST functions
car* newCar(int life);
car* insertCar(car *node, int life);
car* searchCar(car *root, int life);
car* deleteCar(car *root, int life);
void deleteAllCars(car *root);
car* minCar(car *node);
int maxLife(car *node);

// Stations list functions
station* createStation(int distance, int carsNumber, int lives[]);
station* insertStation(station *s);
void deleteStation(int dist);
station* searchStation(station *root, int dist);

// Stations BST functions
station* insertStationBST(station *root, station *s);
station* deleteStationBST(station *root, int dist);
station* minStation(station *node);

// Path planning function
int* planPath(station *start, station *finish, int *num);

// Stations global pointers
station *stat = NULL;
station *statTree = NULL;

int main() {

    int lives[512], dist, num, life, start, finish, n, *p;
    char cmd[20];
    station *s, *tmp;
    car *tmp1;

    while(scanf("%s", cmd) != EOF){

        switch(cmd[0]) {
            case 'a':
                switch(cmd[9]){
                    case 's':
                        assert(scanf("%d %d", &dist, &num));
                        for(int j = 0; j < num; j++){
                            assert(scanf("%d", &lives[j]) != '\n');
                        }
                        if(searchStation(statTree, dist) == NULL){
                            s = createStation(dist, num, lives);
                            stat = insertStation(s);
                            statTree = insertStationBST(statTree, s);
                            printf("aggiunta\n");
                        } else {
                            printf("non aggiunta\n");
                        }
                        break;

                    case 'a':
                        assert(scanf("%d %d", &dist, &life));
                        tmp = searchStation(statTree, dist);
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
                assert(scanf("%d", &dist));
                if(searchStation(statTree, dist) != NULL){
                    statTree = deleteStationBST(statTree, dist);
                    deleteStation(dist);
                    printf("demolita\n");
                } else {
                    printf("non demolita\n");
                }
                break;

            case 'r':
                assert(scanf("%d %d", &dist, &life));
                tmp = searchStation(statTree, dist);
                if(tmp != NULL){
                    tmp1 = searchCar(tmp->root, life);
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
                assert(scanf("%d %d", &start, &finish));
                p = planPath(searchStation(statTree, start), searchStation(statTree, finish), &n);

                if(p){
                    for(int a = n - 1; a >= 0; a--)
                        printf("%d ", p[a]);
                    printf("\n");
                    free(p);
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

        car *temp = minCar(root->r);

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

car* minCar(car *node) {
    car *current = node;
    while(current && current->l != NULL)
        current = current->l;
    return current;
}

int maxLife(car *node) {
    car* current = node;
    while(current && current->r != NULL)
        current = current->r;
    if(current == NULL)
        return 0;
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
    s->r = s->l = NULL;
    return s;
}

station* insertStation(station *s) {
    if(stat == NULL){
        stat = s;
        s->next = NULL;
        return stat;
    }

    if(s->dist < stat->dist){
        s->next = stat;
        stat = s;
        return stat;
    }

    station *curr = stat;

    while(curr->next && curr->next->dist < s->dist){
        curr = curr->next;
    }

    s->next = curr->next;
    curr->next = s;

    return stat;
}

void deleteStation(int dist) {
    station *temp = stat, *prev;

    if(temp != NULL && temp->dist == dist){
        stat = temp->next;
        deleteAllCars(temp->root);
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
    deleteAllCars(temp->root);
    free(temp);
}

station* insertStationBST(station *root, station *s) {
    if(root == NULL)
        return s;

    if(s->dist < root->dist)
        root->l = insertStationBST(root->l, s);
    else if(s->dist > root->dist)
        root->r = insertStationBST(root->r, s);

    return root;
}

station* searchStation(station *root, int dist) {
    if(root == NULL || root->dist == dist)
        return root;

    if(dist < root->dist)
        return searchStation(root->l, dist);

    return searchStation(root->r, dist);
}

station* deleteStationBST(station *root, int dist) {
    if(root == NULL)
        return root;

    if(dist < root->dist)
        root->l = deleteStationBST(root->l, dist);

    else if(dist > root->dist)
        root->r = deleteStationBST(root->r, dist);

    else {
        if(root->l == NULL) {
            station *temp = root->r;
            return temp;
        }
        else if(root->r == NULL) {
            station *temp = root->l;
            return temp;
        }

        station *temp = minStation(root->r);

        root->dist = temp->dist;
        root->root = temp->root;
        root->next = temp->next;

        root->r = deleteStationBST(root->r, temp->dist);
    }
    return root;
}

station* minStation(station *node) {
    station *current = node;
    while(current && current->l != NULL)
        current = current->l;
    return current;
}

int* planPath(station *start, station *finish, int *num) {
    *num = 0;
    int *path = (int*)malloc((*num) * sizeof(int));

    station *curr, *tmp;

    if(start->dist < finish->dist){
        (*num)++;
        path = realloc(path, (*num) * sizeof(int));
        path[*num - 1] = finish->dist;

        curr = start;
        tmp = finish;

        while(tmp != start){
            while(curr->dist + maxLife(curr->root) < tmp->dist){
                curr = curr->next;
            }
            if(curr != tmp){
                (*num)++;
                path = realloc(path, (*num) * sizeof(int));
                path[*num - 1] = curr->dist;
                tmp = curr;
                curr = start;
            } else {
                free(path);
                return NULL;
            }
        }

        return path;

    } else if(start->dist > finish->dist){
        int size = 1, pos = 0;
        station ***nodes = (station***)malloc(size * sizeof(station**));
        int *len = (int*)malloc(size * sizeof(int));
        nodes[size - 1] = (station**)malloc(pos * sizeof(station*));

        pos++;
        nodes[size - 1] = realloc(nodes[size - 1], pos * sizeof(station*));
        nodes[size - 1][pos - 1] = finish;
        len[size - 1] = pos;

        curr = finish;
        tmp = curr->next;

        while(curr != start && tmp != start){
            size++;
            nodes = realloc(nodes, size * sizeof(station**));
            len = realloc(len, size * sizeof(int));
            pos = 0;
            nodes[size - 1] = (station**)malloc(pos * sizeof(station*));
            while(tmp != start->next){
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

                for(int i = size - 1; i >= 0; i--)
                    free(nodes[i]);
                free(nodes);

                free(len);

                return NULL;
            }
            curr = nodes[size - 1][pos - 1];
            tmp = curr->next;
        }


        if(nodes[size - 1][len[size - 1] - 1] == start){
            if(len[size - 1] > 1){
                len[size - 1] = 1;
                nodes[size - 1] = realloc(nodes[size - 1], sizeof(station*));
                nodes[size - 1][0] = start;
            } else {
                if(start->dist - maxLife(start->root) <= nodes[size - 1][len[size - 1] - 1]->dist){
                    size++;
                    nodes = realloc(nodes, size * sizeof(station**));
                    len = realloc(len, size * sizeof(int));
                    nodes[size - 1] = (station**)malloc(sizeof(station*));
                    nodes[size - 1][0] = start;
                    len[size - 1] = 1;
                } else {
                    free(path);

                    for(int i = size - 1; i >= 0; i--)
                        free(nodes[i]);
                    free(nodes);

                    free(len);

                    return NULL;
                }
            }
        } else {
            if(start->dist - maxLife(start->root) <= nodes[size - 1][len[size - 1] - 1]->dist){
                size++;
                nodes = realloc(nodes, size * sizeof(station**));
                len = realloc(len, size * sizeof(int));
                nodes[size - 1] = (station**)malloc(sizeof(station*));
                nodes[size - 1][0] = start;
                len[size - 1] = 1;
            } else {
                free(path);

                for(int i = size - 1; i >= 0; i--)
                    free(nodes[i]);
                free(nodes);

                free(len);

                return NULL;
            }
        }


        int max, temp;

        for(int i = size - 1; i > 1; i--){
            for(int j = 0; j < len[i - 1]; j++){
                max = nodes[i][0]->dist - maxLife(nodes[i][0]->root);
                for(int k = 0; k < len[i]; k++){
                    temp = nodes[i][k]->dist - maxLife(nodes[i][k]->root);
                    if(temp < max)
                        max = temp;
                }
                if(nodes[i - 1][j]->dist < max){
                    len[i - 1]--;
                    for(int k = 0; k < len[i - 1]; k++){
                        nodes[i - 1][k] = nodes[i - 1][k + 1];
                    }
                    nodes[i - 1] = realloc(nodes[i - 1], len[i - 1] * sizeof(station*));
                    j--;
                } else {
                    break;
                }
            }
        }


        (*num)++;
        path = realloc(path, (*num) * sizeof(int));
        path[*num - 1] = finish->dist;

        bool found = false;
        for(int i = 0; i < size - 1; i++, found = false){
            for(int j = 0; j < len[i] && !found && nodes[i][j] != start; j++){
                for(int k = 0; k < len[i + 1] && !found; k++){
                    if(nodes[i][j]->dist == path[*num - 1] && nodes[i + 1][k]->dist - maxLife(nodes[i + 1][k]->root) <= nodes[i][j]->dist){
                        (*num)++;
                        path = realloc(path, (*num) * sizeof(int));
                        path[*num - 1] = nodes[i + 1][k]->dist;
                        found = true;
                    }
                }
            }
        }


        if(path[*num - 1] == start->dist){

            for(int i = size - 1; i >= 0; i--)
                free(nodes[i]);
            free(nodes);

            free(len);

            return path;

        } else {

            free(path);

            for(int i = size - 1; i >= 0; i--)
                free(nodes[i]);
            free(nodes);

            free(len);

            return NULL;
        }

    } else {
        //base case
        (*num)++;
        path = realloc(path, (*num) * sizeof(int));
        path[*num - 1] = start->dist;
        return path;
    }
}