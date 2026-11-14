/**
 * @file message_center.h
 * @author 
 * @brief 
 * @version 0.1
 * @date 2022-11-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PUB_SUB_H
#define PUB_SUB_H

#include "stdint.h"

#define MAX_TOPIC_NAME_LEN 32 
#define MAX_TOPIC_COUNT 12    
#define QUEUE_SIZE 1

typedef struct mqt
{
    
    void *queue[QUEUE_SIZE];
    uint8_t data_len;
    uint8_t front_idx;
    uint8_t back_idx;
    uint8_t temp_size; 
    struct mqt *next_subs_queue; 
} Subscriber_t;

typedef struct ent
{
    char topic_name[MAX_TOPIC_NAME_LEN + 1]; 
    uint8_t data_len;                        
    Subscriber_t *first_subs;
    struct ent *next_topic_node;
    uint8_t pub_registered_flag; 
} Publisher_t;

static void CheckName(char *name);
static void CheckLen(uint8_t len1, uint8_t len2);

Subscriber_t *SubRegister(char *name, uint8_t data_len);

Publisher_t *PubRegister(char *name, uint8_t data_len);

uint8_t SubGetMessage(Subscriber_t *sub, void *data_ptr);

uint8_t PubPushMessage(Publisher_t *pub, void *data_ptr);

#endif // !PUBSUB_H
