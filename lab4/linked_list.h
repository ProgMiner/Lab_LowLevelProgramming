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

/* list_foreach :: [Int] -> (Int -> IO ()) -> IO () */
void list_foreach(const list_t * list, void (* f)(int));

/* list_map :: (Int -> Int) -> [Int] -> [Int] */
list_t * list_map(int (* f)(int), const list_t * list);

/* list_map_mut :: (Int -> Int) -> State [Int] */
void list_map_mut(int (* f)(int), list_t * list);

/* list_foldl :: Int -> (Int -> Int -> Int) -> [Int] -> Int */
void * list_foldl(void * acc, void * (* f)(int, void *), const list_t * list);

/* iterate :: Int -> Int -> (Int -> Int) -> [Int] */
list_t * list_iterate(int value, unsigned int length, int (* f)(int));
