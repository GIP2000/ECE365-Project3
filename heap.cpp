#include "heap.h"
#include "hash.h"
#include <vector>
#include <bits/stdc++.h>
#include <iostream> 

using namespace std;
heap::heap(int capacity){
    this->capacity = capacity;
    this->data = vector<heap::heapItem>(capacity+1);
    this->hashmap = hashTable(capacity); 
}
int heap::insert(const std::string &id, int key, void *pv){
    if(this->capacity == this->lastIndex) return 1; 
    if(this->hashmap.contains(id)) return 2; 

    for(int i = this->lastIndex + 1; i > 0; i /= 2){
        if(checkIfValid(i,key) == 0){
            this->data[i].key = key;
            this->data[i].id = id; 
            this->data[i].pv = pv; 
            this->data[i].index = i; 
            this->hashmap.insert(id,&data[i]); 
            this->lastIndex++; 
            return 0;
        }
        data[i] = data[i/2];
        data[i].index = i; 
        this->hashmap.setPointer(data[i].id,&data[i]);

    }
    return -1; // unexpected error should never occur; 

}
int heap::setKey(const std::string &id, int key){
    bool found = false; 
    heapItem* item = (heapItem*)this->hashmap.getPointer(id,&found);
    if(!found) return 1; 

    int i = item->index; 
    void* pv = item->pv;

    while(true){
        switch(checkIfValid(i,key)){
            case 0: 
                this->data[i].id = id; 
                this->data[i].key = key;  
                this->data[i].index = i; 
                this->data[i].pv = pv; 
                return 0;
            case 1:
                this->data[i] = data[i/2];
                this->data[i].index = i;
                this->hashmap.setPointer(data[i].id,&data[i]); 
                i/=2; 
                break;
            case 2: 
                this->data[i] = this->data[i*2];
                this->data[i].index = i; 
                this->hashmap.setPointer(data[i].id,&data[i]);
                i*=2; 
                break;
            case 3:
                this->data[i] = this->data[i*2+1];
                this->data[i].index = i;
                this->hashmap.setPointer(data[i].id,&data[i]); 
                i = i*2 +1; 
                break;
        }
    }


}
int heap::deleteMin(std::string *pId, int *pKey , void **ppData ){
    if(this->lastIndex == 0) return 1; 

    heapItem min = this->data[1]; 
    if(pId != nullptr) *pId = min.id; 
    if(pKey != nullptr) *pKey = min.key;
    if(ppData != nullptr) *ppData = min.pv;
    this->hashmap.remove(min.id); 

    heapItem bot = this->data[this->lastIndex]; 
    const int key = bot.key; 
    bool addOne = false; 
    this->lastIndex--; 
    for(int i = 1; i<this->lastIndex+1; i*=2){
        if(addOne){
            i++;
            addOne = false;
        } 
        switch(checkIfValid(i,key)){
            case 0: 
                this->data[i] = bot; 
                this->data[i].index = i; 
                this->hashmap.setPointer(data[i].id,&data[i]);
                return 0;
            case 2: 
                this->data[i] = this->data[i*2];
                this->data[i].index = i; 
                this->hashmap.setPointer(data[i].id,&data[i]);
                break;
            case 3:
                this->data[i] = this->data[i*2+1];
                this->data[i].index = i; 
                this->hashmap.setPointer(data[i].id,&data[i]);
                addOne = true;
                break;
        }

    }
    return 0; // should only happen if the array has only 1 item in it
    

}
int heap::remove(const std::string &id, int *pKey, void *ppData){\
    bool found = false; 
    heapItem* item = (heapItem*)this->hashmap.getPointer(id,&found);
    if(!found) return 1;
    if(pKey != nullptr) *pKey = item->key; 
    if(ppData != nullptr) ppData = item->pv; 
    this->setKey(id,this->data[1].key-1); 
    return this->deleteMin(nullptr);  
}

int heap::checkIfValid(int index, int key){
    int parentKey = this->data[index/2 > 0 ? index/2 : 0].key;
    int leftChild = this->data[index*2 <= this->lastIndex ? index*2 : 0].key;
    int rightChild = this->data[index*2 +1 <= this->lastIndex ? index*2 +1 : 0].key;

    // if the parent index is valid and the key is less than the parent
    if(index/2 > 0 && key < parentKey) return 1;
    
    // if the left child index is valid and the key is less than the left child and if the right child index is valid the left child is also smaller than the right child
    if((index*2) <= this->lastIndex && key > leftChild && leftChild < ((index*2 + 1) <= this->lastIndex ? rightChild:leftChild+1) ) return 2;

    // if the right child index is valid and the key is less than the right child
    if((index*2 + 1) <= this->lastIndex && key > rightChild) return 3;
    return 0; 
}


int heap::getKey(const string& id, bool * found){
    heapItem* item = ((heapItem*)this->hashmap.getPointer(id,found));
    if(!(*found)) return -1;
    return item->key;
     
}

void heap::setPointer(const string& id, void * pv){
    bool found = false; 
    heapItem* item = (heapItem*)this->hashmap.getPointer(id,&found);
    if(!found) return;
    item->pv = pv; 
}

void * heap::getPointer(const string& id, bool * found){
    heapItem* item = (heapItem*)this->hashmap.getPointer(id,found);
    return item == nullptr ? nullptr : item->pv; 
}