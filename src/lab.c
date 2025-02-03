/**
 * lab.c
 * 
 * @author Vladyslav (Vlad) Maliutin
 */

#include "lab.h"
#include <stdlib.h>

/**
 * @brief Initializes a new circular linked list.
 * 
 * This function creates a new list with a sentinel node and initializes the
 * destroy_data and compare_to function pointers.
 * 
 * @param destroy_data Function to free memory allocated for data.
 * @param compare_to Function to compare two data elements.
 * @return A pointer to the newly created list, or NULL if allocation fails.
 */
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) return NULL;

    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;

    list->head = (node_t *)malloc(sizeof(node_t));
    if (!list->head) {
        free(list);
        return NULL;
    }

    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;

    return list;
}

/**
 * @brief Destroys the circular linked list and frees all allocated memory.
 * 
 * This function traverses the list, freeing each node's data using the
 * destroy_data function pointer, and then frees the nodes and the list itself.
 * 
 * @param list A double pointer to the list to be destroyed.
 */
void list_destroy(list_t **list) {
    if (!list || !*list) return;

    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *temp = current;
        current = current->next;
        (*list)->destroy_data(temp->data);
        free(temp);
    }

    free((*list)->head);
    free(*list);
    *list = NULL;
}

/**
 * @brief Adds a new element to the front of the circular linked list.
 * 
 * This function inserts a new node containing the provided data at the front
 * of the list, just after the sentinel node.
 * 
 * @param list A pointer to the list.
 * @param data A pointer to the data to be added.
 * @return The list pointer, or NULL if allocation fails.
 */
list_t *list_add(list_t *list, void *data) {
    if (!list || !data) return NULL;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) return NULL;

    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;

    list->head->next->prev = new_node;
    list->head->next = new_node;

    list->size++;

    return list;
}

/**
 * @brief Removes an element from the specified index in the list.
 * 
 * This function removes the node at the given index and returns its data.
 * If the index is invalid, the function returns NULL.
 * 
 * @param list A pointer to the list.
 * @param index The zero-based index of the element to remove.
 * @return A pointer to the removed data, or NULL if the index is invalid.
 */
void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size) return NULL;

    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    void *data = current->data;

    current->prev->next = current->next;
    current->next->prev = current->prev;

    free(current);
    list->size--;

    return data;
}

/**
 * @brief Searches for the first occurrence of the specified data in the list.
 * 
 * This function traverses the list and uses the compare_to function to find
 * the index of the first node containing the specified data.
 * 
 * @param list A pointer to the list.
 * @param data A pointer to the data to search for.
 * @return The zero-based index of the data, or -1 if not found.
 */
int list_indexof(list_t *list, void *data) {
    if (!list || !data) return -1;

    node_t *current = list->head->next;
    size_t index = 0;

    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}