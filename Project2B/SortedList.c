/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    SortedList.c
***/

#include "SortedList.h"
#include <string.h>
#include <sched.h>

int opt_yield = 0;

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
  if (list == NULL || element == NULL)
    return;
  SortedList_t *p = list->next;
  while (p != list && strcmp(p->key, element->key) < 0) {
    p = p->next;
  }
  SortedList_t *prev = p->prev;
  if (opt_yield & INSERT_YIELD) {
    sched_yield();
  }
  prev->next = element;
  element->prev = prev;
  element->next = p;
  p->prev = element;
}

int SortedList_delete(SortedListElement_t *element) {
  if (element == NULL)
    return -1;
  SortedList_t *next = element->next;
  SortedList_t *prev = element->prev;
  if (next->prev != element || prev->next != element) {
    return 1;
  }
  if (opt_yield & DELETE_YIELD) {
    sched_yield();
  }
  next->prev = prev;
  prev->next = next;
  return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
  if (list == NULL || key == NULL)
    return NULL;
  SortedList_t *p = list->next;
  for(;p != list; p = p->next) {
    if (opt_yield & LOOKUP_YIELD) {
      sched_yield();
    }
    if (strcmp(p->key, key) == 0)
      return p;
  }
  return NULL;
}

int SortedList_length(SortedList_t *list) {
  if (list == NULL)
    return -1;
  int count = 0;
  SortedList_t *p = list->next;
  for(;p != list; p = p->next) {
    SortedList_t *next = p->next;
    SortedList_t *prev = p->prev;
    if (opt_yield & LOOKUP_YIELD) {
      sched_yield();
    }
    if (next->prev != p || prev->next != p) {
      return -1;
    }
    count++;
  }
  return count;
}
