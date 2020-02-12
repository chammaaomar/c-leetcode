/*

Design and implement a data structure for Least Recently Used (LRU) cache.
It should support the following operations: get and put.
get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.

put(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity,
it should invalidate the least recently used item before inserting a new item

The cache is initialized with a positive capacity.

Runtime: 72 ms, faster than 95.83% of C online submissions for LRU Cache.
Memory Usage: 26.7 MB, less than 100.00% of C online submissions for LRU Cache.
*/

// I have implemented both 'put' and 'get' operations to be O(1) in time
// using a hash-table and doubly-linked list to store the use history
// and evict the LRU element

#include <stdio.h>
#include <stdlib.h>
typedef struct DLLNode {
    // doubly-linked list; makes the operation of insertion and removal easy
    struct DLLNode* next;
    struct DLLNode* prev;
    int key;
} DLLNode;

typedef struct SLLNode {
    // singly-linked list. This is to handle hash collisions.
    struct SLLNode* next;
    // each element in the hash table will have a reference to the
    // node, keeping track of how recently it has been used (either read or written to)
    // this is so that insertions into the cache can be handled in O(1); otherwise, we would
    // need to traverse a doubly-linked list to update the cache and use history, which is O(n)
    // and generally non-local, thus very slow. So we keep a reference to immediately change
    // its position.
    DLLNode* cacheNode;
    int value;
    int key;
} SLLNode;

typedef struct LRUCache {
    int capacity;
    int size; // current number of elements
    SLLNode** data;
    DLLNode* LRU;
    DLLNode* MRU; // (M)ost (R)ecently (U)sed element
} LRUCache;

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* cache = (LRUCache*) malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->data = (SLLNode**) calloc(capacity, sizeof(SLLNode*));
    cache->size = 0;
    cache->LRU = NULL;
    cache->MRU = NULL;

    return cache;
}

int lRUCacheGet(LRUCache* cache, int key) {
    // if the key is present, return the value, otherwise return -1
    // we don't have a hash function since the keys are integers; just
    // reduce modulo the capacity to bring it into the array range / bounds
    int hash = key % cache->capacity;
    SLLNode* dataNode = (cache->data)[hash];
    int value = -1;
    if (dataNode == NULL) {
        return -1;
    }
    while (dataNode) {
        if (dataNode->key == key) {
            // found the key in the hash bucket
            value = dataNode->value;
            break;
        }
        else if (dataNode->next)
            dataNode = dataNode->next;
        else break;
        }
    if (value == -1)
        // value not found
        return -1;
    // update the cache; different scenarios depending on whether the read
    // element is the LRU or not. Here it becomes clear why we use a doubly-linked
    // list.
    if (dataNode->cacheNode == cache->LRU) {
        DLLNode* newMRU = cache->LRU;
        cache->LRU = (cache->LRU)->next;
        (cache->LRU)->prev = NULL;
        (cache->MRU)->next = newMRU;
        newMRU->prev = cache->MRU;
        newMRU->next = NULL;
        cache->MRU = newMRU;
        // p -> == (*p). involves dereferencing, so I'm not modifying a copy
        // of the pointer I'm modifying the object actually stored at that
        // memory address but DO NOTE that dataNode is an independent copy
        // of (cache->data)[hash] and if I assign it to something else
        // it doesn't affect the original
        dataNode->cacheNode = cache->MRU;
    }
    else if (dataNode->cacheNode == cache-> MRU) {/* nothing to do*/}
    // else if (cache ->capacity == 1) {}
    else if (cache->size > 2) {
        ((dataNode->cacheNode)->prev)->next = (dataNode->cacheNode)->next;
        ((dataNode->cacheNode)->next)->prev = (dataNode->cacheNode)->prev;
        (cache->MRU)->next = dataNode->cacheNode;
        (dataNode->cacheNode)->prev = cache->MRU;
        (dataNode->cacheNode)->next = NULL;
        cache->MRU = dataNode->cacheNode;
    }
        return value;
}

void lRUCachePut(LRUCache* cache, int key, int value) {
    int replace = 0; // to indicate whether the key is already present and we're just updating the value
    int hash = key % cache->capacity;
    SLLNode* dataNode; // this is the SLLNode where we will end up putting the key value pair
   if ((cache->data)[hash] == NULL) {
       // the hash bucket is empty; create a new linked list there
       (cache->data)[hash] = (SLLNode*) malloc(sizeof(SLLNode));
       (cache->data)[hash]->next = NULL;
       (cache->data)[hash]->key = key;
       (cache->data)[hash]->value = value;
    //    (cache->data)[hash]->cacheNode = cache->MRU;
       dataNode = (cache->data)[hash]; // just a reference
}
   else {
       // there is already a linked list at the hash bucket;
       // dataNode is a pointer to the first node in the hash bucket
       // later it will point to the node where we store our key-value pair
       dataNode = (cache->data)[hash];
       while (dataNode){
           if (dataNode->key == key) {
               // the key is already present; just set the value instead of inserting
               dataNode->value = value;
               replace = 1;
               break;
           }
           // if the key isn't already present, dataNode is the last present node
           // in the same hash bucket; so we will place the key value pair at the
           // next of this node.
           if (dataNode->next) dataNode = dataNode->next;
           else break;
       }
       if (!replace) {
           SLLNode* newDataNode = malloc(sizeof(SLLNode));
           newDataNode->next = NULL;
           newDataNode->key = key;
           newDataNode->value = value;
        //    newDataNode->cacheNode = cache->MRU;
           dataNode->next = newDataNode;
           // just a reference; this is redirecting a pointer to another address in memory
           dataNode = newDataNode; 
       }
   }
    
   if (replace) {
       // the key was already present and we set its value
       // thus, we need to take the cache node and move it to the top
       // of the LRU history, and there are different cases depending
       // on whether it was the LRU or not
       if (dataNode->cacheNode == cache->MRU) {}
       else if (dataNode->cacheNode == cache->LRU) {
           DLLNode* tmp = cache->LRU;
           cache->LRU = (cache->LRU)->next;
           (cache->LRU)->prev = NULL;
           tmp->prev = cache->MRU;
           (cache->MRU)->next = tmp;
           cache->MRU = tmp;
           (cache->MRU)->next = NULL;
       }
       else {
           DLLNode* tmp = dataNode->cacheNode;
           (tmp->prev)->next = tmp->next;
           (tmp->next)->prev = tmp->prev;
           tmp->prev = cache->MRU;
           (cache->MRU)->next = tmp;
           cache->MRU = tmp;
           (cache->MRU)->next = NULL;
       }
   }
   // no replacement happened; the key wasn't already present; new key inserted
   else if (cache->size == 0) {
       // The LRU is not actually defined for an LRUCache of single element;
       cache->MRU = (DLLNode*) malloc(sizeof(DLLNode));
       (cache->MRU)->next = NULL;
       (cache->MRU)->prev = NULL;
       (cache->MRU)->key = key;
   }
   else if (cache->size == 1) {
       // I only define the LRU once we have added two elements;
       cache->LRU = cache->MRU;
       cache->MRU = (DLLNode*) malloc(sizeof(DLLNode));
       (cache->LRU)->next = cache->MRU;
       (cache->MRU)->prev = cache->LRU;
       (cache->MRU)->next = NULL;
       (cache->MRU)->key = key;
   }
   else {
       DLLNode* cacheNode = (DLLNode*) malloc(sizeof(DLLNode));
       cacheNode->prev = cache->MRU;
       cacheNode->next = NULL;
       cacheNode->key = key;
       (cache->MRU)->next = cacheNode;
       cache->MRU = cacheNode;
   }
   dataNode->cacheNode = cache->MRU; 
   // if a replacement happened we won't remove an element from the cache
   // since we just set the value of a key, we didn't insert anything
   if (replace) {}
   else if (cache->size < cache->capacity) {
       (cache->size)++;
   }
    else {
       // this is implementing the logic of removing the LRU element from both the recent use list
       // and the hashtable
       DLLNode* tmp = cache->LRU;
       int hash = tmp->key % cache->capacity;
       SLLNode* LRUHash = (cache->data)[hash];
       if (LRUHash->key == tmp->key) {
           // remove the data node; in this case its pointer is the first
           // in the hash bucket
           (cache->data)[hash] = LRUHash->next;
           free(LRUHash);
       }
       else {
           // we want to remove the data node, but we need to find the previous
           // node, so we can set its next to be the next of the node we want to remove
           while ((LRUHash->next)->key != tmp->key) {
               LRUHash = LRUHash->next;
           }
           // LRUHash is the previous node to the LRU Node.
           SLLNode* newNext = (LRUHash->next)->next;
           free(LRUHash->next);
           LRUHash->next = newNext;
       }
        if (cache->capacity > 1){
            cache->LRU = (cache->LRU)->next;
            (cache->LRU)->prev = NULL;
            free(tmp);   
        }
        else {
            (cache->MRU)->prev = NULL;
            free(cache->LRU);
        }
   }
}

void lRUCacheFree(LRUCache* cache) {
    for (int i = 0; i < cache->capacity; i++) {
        SLLNode* dataNode = (cache->data)[i];
        if (dataNode == NULL)
            continue;
        if (dataNode->next == NULL) {
            free(dataNode->cacheNode);
            free(dataNode);
        }
        else {
            while (dataNode->next) {
                SLLNode* tmp = dataNode;
                dataNode = dataNode->next;
                free(tmp->cacheNode);
                free(tmp);
            }
        }
    }
    free(cache->data);
    free(cache);
}


/**
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);
 
 * lRUCachePut(obj, key, value);
 
 * lRUCacheFree(obj);
*/  
