//
//  LFU.h
//  mimircache
//
//  Created by Juncheng on 6/2/16.
//  Copyright © 2016 Juncheng. All rights reserved.
//

#ifndef LFU_h
#define LFU_h



#include "cache.h"
#include "pqueue.h"


#ifdef __cplusplus
extern "C"
{
#endif


struct LFU_params{
    GHashTable *hashtable;
    pqueue_t *pq;
};
typedef struct LFU_params LFU_params_t;





extern gboolean LFU_check_element(cache_t* cache, request_t* cp);
extern gboolean LFU_add_element(cache_t* cache, request_t* cp);


extern void     __LFU_insert_element(cache_t* LFU, request_t* cp);
extern void     __LFU_update_element(cache_t* LFU, request_t* cp);
extern void     __LFU_evict_element(cache_t* LFU, request_t* cp);
extern void*    __LFU__evict_with_return(cache_t* cache, request_t* cp);


extern void     LFU_destroy(cache_t* cache);
extern void     LFU_destroy_unique(cache_t* cache);

cache_t*   LFU_init(guint64 size, char data_type, guint64 block_size, void* params);


extern void     LFU_remove_element(cache_t* cache, void* data_to_remove);
extern guint64 LFU_get_size(cache_t* cache);


#ifdef __cplusplus
}
#endif


#endif	/* LFU_H */ 
