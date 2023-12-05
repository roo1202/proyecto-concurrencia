
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
   list -> head = NULL;
   list -> size = 0;
   pthread_mutex_init(&list->list_mutex,NULL);
   return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
   pthread_mutex_lock(&list->list_mutex);
   struct int_node_t* current = list -> head;
   while(current != NULL)
   {
       struct int_node_t* temp = current;
       current = current->next;
       pthread_mutex_destroy(&temp->mutex);
       free(temp);
   }
   pthread_mutex_unlock(&list->list_mutex);
   pthread_mutex_destroy(&list->list_mutex);
   return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
   /*int count = 0;
   struct int_node_t* current = list->head;

   pthread_mutex_lock(&list->list_mutex);
   while(current != NULL)
   {
       struct int_node_t* next = current->next;
       pthread_mutex_unlock(&list->list_mutex);
       current = next;
       count++;
   }
   list -> size = count;
   pthread_mutex_unlock(&list->list_mutex);
   printf("tamanno %d \n",count);
   return count;*/
   pthread_mutex_lock(&list->list_mutex);
   int f = list->size;
   pthread_mutex_unlock(&list->list_mutex);
   return f;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    if(list == NULL) return 1;
    if(index < 0) {
        index = 0;
    }
    pthread_mutex_lock(&list->list_mutex);
    int size = list->size;
    if(index > size - 1 && size > 0)
    {
        index = size - 1;
    }
    int count = 0;
    struct int_node_t* current = list->head;
    while(current != NULL)
    {
        if(count == index)
        {
            *out_value = current->data;
            pthread_mutex_unlock(&list->list_mutex);
            return 0;
        }
        count ++;
        //struct int_node_t* next = current->next;
        //pthread_mutex_unlock(&list->list_mutex);
        current = current->next;
    }
    pthread_mutex_unlock(&list->list_mutex);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
   if(index < 0) {
       index = 0;
   }
   pthread_mutex_lock(&list->list_mutex);
   int size = list->size;
   if(index > size - 1 && size > 0)
   {
       index = size - 1;
   }
   struct int_node_t* current = list->head;
   struct int_node_t* previous = NULL;
   int count = 0;
   while (current != NULL) {
       if (count == index) {
           struct int_node_t* newNode = (struct int_node_t*)malloc(sizeof(struct int_node_t));
           newNode->data = value;
           newNode->next = current;
           if (previous == NULL) {
               list->head = newNode;
           } else {
               previous->next = newNode;
           }
           pthread_mutex_init(&newNode->mutex, NULL);
           list->size += 1;
           pthread_mutex_unlock(&list->list_mutex);
           return 0; 
       }
       count++;
       previous = current;
       current = current->next;
   }
   if (current == NULL && (index == 0 || index == size -1)) {
       struct int_node_t* newNode = (struct int_node_t*)malloc(sizeof(struct int_node_t));
       newNode->data = value;
       newNode->next = NULL;
       if(index == 0)
       list->head = newNode;
       if(index == size -1)
       {
        previous->next = newNode;
       }
       pthread_mutex_init(&newNode->mutex, NULL);
       list->size += 1;
       pthread_mutex_unlock(&list->list_mutex);
   }
   return 0; 
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    if(list == NULL) return 1;
    if(index < 0) {
        index = 0;
    }
    pthread_mutex_lock(&list->list_mutex);
    int size = list->size;
    if(index > size - 1 && size > 0)
    {
        index = size - 1;
    }
    struct int_node_t* current = list->head;
    struct int_node_t* previous = NULL;
    int count = 0;
    while (current != NULL) {
        if (count == index) {
            *out_value = current->data;
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }
            pthread_mutex_destroy(&current->mutex);
            free(current);
            list->size -= 1;
            pthread_mutex_unlock(&list->list_mutex);
            return 0; 
        }
        count++;
        previous = current;
        current = current->next;
    }
    return 0;
}
