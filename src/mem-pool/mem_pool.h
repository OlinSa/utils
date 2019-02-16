#ifndef UTILS_SRC_MEM_POOL_H
#define UTILS_SRC_MEM_POOL_H

//https://blog.csdn.net/u010183728/article/details/81531392

/*
| struct buffer | mempool_struct| memory chunk pool | memory |
*/

#define MIN_UNIT_SIZE 64
#define ADDR_ALIGN 8
#define SIZE_ALIGN MIN_UNIT_SIZE

struct memory_chunk;

typedef struct memory_block{
    size_t count;
    size_t start;
    struct memory_chunk *mem_chunk;
}memory_block_t;

//memory block which can be used
typedef struct memory_chunk{
    memory_block_t *free_mem_addr;
    memory_block_t *pre;
    memory_block_t *next;
}memory_chunk_t;

//memory pool struct 
typedef struct memory_pool{
    void *mem;
    size_t size;
    memory_block_t *mem_map;
    memory_chunk_t *free_mem_chunk;
    memory_chunk_t *free_mem_chunk_pool;
    size_t mem_used_size; //record the size of memory pool that has been allocated to the user
    size_t mem_map_pool_count;//record the number of unit element in buffer
    size_t free_mem_chunk_count;
    size_t mem_map_unit_count;
    size_t mem_block_count;
}memory_pool_t;

/**
 * @brief Based on the 'product_key' + 'device_name' + 'device_secret' produce an MQTT connection username and password.
 *
 * @param [in] product_key: @n Apply for 'product_key' in the AliYun Console.
 * @param [in] device_name: @n Apply for 'device_name' in the AliYun Console.
 * @param [in] device_secret: @n Apply for 'device_secret' in the AliYun Console.
 * @param [out] info_ptr: @n return MQTT connection parameter.
 *
 * @retval -1 : Fail.
 * @retval  0 : Success.
 * @see None.
 */

/*
 * Create memory buffer
 * buf:set start addr of memory buffer
 * bufsize: the size of memory buffer
 * 
 * return: memory pool point
 */
memory_pool_t *CreateMemoryPool(void *buf, size_t bufsize);



#endif