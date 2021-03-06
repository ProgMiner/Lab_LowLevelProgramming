#pragma once

/* [Int] */
struct list;
typedef struct list list_t;

/* list_create :: Int -> [Int] */
list_t * list_create(int value);

/* list_node_at :: [Int] -> Int -> [Int] */
list_t * list_node_at(list_t * list, unsigned int index);

/* list_free :: [Int] -> IO () */
void list_free(list_t * list);

/* list_length :: [Int] -> Int */
unsigned int list_length(const list_t * list);

/* list_get :: [Int] -> Int -> Int */
int list_get(const list_t * list, unsigned int index);

/* list_sum :: [Int] -> Int */
long list_sum(const list_t * list);

/* list_add_front :: Int -> [Int] -> [Int] */
void list_add_front(int value, list_t ** list);

/* list_add_back :: Int -> [Int] -> [Int] */
void list_add_back(int value, list_t ** list);
