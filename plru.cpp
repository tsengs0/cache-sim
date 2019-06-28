#include "plru.h"

#define ll long long


    /*
    Tree with setAssociativity-1 nodes to keep track of least recently accessed element in cache.
    A tree which would look like
    
                    1
                  /   \
                 2     3
                / \   / \
               4   5 6   7
    
    is represented in the array as 
        _____________________________
        | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
        
    for an 8-way setAssociativity
    */
    PLRU::PLRU(ll cacheSize, ll blockSize, ll setAssociativity) : 
        Cache(cacheSize, blockSize, setAssociativity){
            bool *tree = (bool *)calloc(numberOfSets * (setAssociativity - 1), sizeof(bool));
        }

    ll PLRU::getBlockToReplace(ll address){
        
        int node = 1;
        ll index = getIndex(address);
        while(node < setAssociativity){
            if(tree[index*setAssociativity + node - 1]){
                node = node*2 + 1;
            }
            else{
                node = node*2;
            }
        } //node can vary from setAssociativity to 2*setAssociativity-1
        return index*setAssociativity + node - setAssociativity;
    }

    void PLRU::update(ll blockToReplace, int status){
        ll node = (blockToReplace % setAssociativity) + setAssociativity;
        ll setNumber = blockToReplace / setAssociativity;
        while(node>1){
            if(node%2){
                cacheBlocks[setNumber*(setAssociativity-1) + node/2] = false;
            }
            else{
                cacheBlocks[setNumber*(setAssociativity-1) + node/2] = true;
            }
            node /= 2;
        }
    }

    PLRU::~PLRU(){}