

//
//  LRU_K.h
//  mimircache
//
//  Created by Juncheng on 6/2/16.
//  Copyright © 2016 Juncheng. All rights reserved.
//

#ifndef LRU_K_H
#define LRU_K_H


#ifdef __cplusplus
extern "C"
{
#endif


#define INITIAL_TS 0 


#include "cache.h"
#include "pqueue.h" 



struct LRU_K_params{
    GHashTable *cache_hashtable;        // label -> pq_node_pointer
    GHashTable *ghost_hashtable;        // label -> gqueue of size K
    pqueue_t *pq;                       
    guint K;
    guint maxK;
    guint64 ts;
};

struct LRU_K_init_params{
    int K;
    int maxK;
};



extern  void __LRU_K_insert_element(cache_t* LRU_K, request_t* cp);

extern  gboolean LRU_K_check_element(cache_t* cache, request_t* cp);

extern  void __LRU_K_update_element(cache_t* LRU_K, request_t* cp);

extern  void __LRU_K_evict_element(cache_t* LRU_K);

extern  gboolean LRU_K_add_element(cache_t* cache, request_t* cp);

extern  void LRU_K_destroy(cache_t* cache);
extern  void LRU_K_destroy_unique(cache_t* cache);


cache_t* LRU_K_init(guint64 size, char data_type, guint64 block_size, void* params);


#ifdef __cplusplus
}
#endif


#endif	/* LRU_K_H */
