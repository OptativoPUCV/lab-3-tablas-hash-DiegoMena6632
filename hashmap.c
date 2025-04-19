#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    int pocicion = hash(key, map->capacity);
    Pair * aux = map->buckets[pocicion];
    if(aux == NULL || aux->key == NULL) {
        map->buckets[pocicion] = createPair(key, value);
        map->size++;
        map->current = pocicion;
    }
    else{
        while(aux != NULL)
        {
            if(is_equal(aux->key,key))
            {
                aux->value = value;
                return;
            }
            pocicion++;
            if(pocicion >= map->capacity) pocicion = 0;
            aux = map->buckets[pocicion];
        }
        map->buckets[pocicion] = createPair(key, value);
        map->size++;
        map->current = pocicion;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) 
{
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
    for (int i = 0; i < capacity; i++) {
        map->buckets[i] = NULL;
    }
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {
    int pocicion = hash(key, map->capacity);
    Pair * aux = map->buckets[pocicion];
    if(aux == NULL) return;
    while(aux != NULL) {
        if(is_equal(aux->key,key)) {
            map->buckets[pocicion]->key = NULL;
            map->size--;
            return;
        }
        pocicion++;
        if(pocicion >= map->capacity) pocicion = 0;
        aux = map->buckets[pocicion];
    }
}

Pair * searchMap(HashMap * map,  char * key) {
    int pocicion = hash(key, map->capacity);
    Pair * aux = map->buckets[pocicion];
    while(aux != NULL) {
        if(is_equal(aux->key,key)) {
            map->current = pocicion;
            return aux;
        }
        pocicion++;
        if(pocicion >= map->capacity) pocicion = 0;
        aux = map->buckets[pocicion];
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    for (int i = 0; i < map->capacity; i++) {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if(map->current == -1) return NULL;
    for (int i = map->current + 1; i < map->capacity; i++) {
        if(map->buckets[i] != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
