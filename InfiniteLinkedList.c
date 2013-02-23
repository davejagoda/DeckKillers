#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  int data;
  struct node *next;
} nodeType;

nodeType *insertFirst(nodeType *head, int data)
{
  nodeType *newNode = (nodeType *)malloc(sizeof(struct node));
  newNode->data = data;
  newNode->next = head;
  return newNode;
}

int main(int argc, char **argv)
{
  nodeType *head = NULL;
  int i = 0;
  printf("each dot represents a million nodes added to the list\n");
  while (1)
    {
      head = insertFirst(head, ++i);
      if (i % 1000000 == 0)
	{
	  printf(".");
	}
    }
  return 0;
}
