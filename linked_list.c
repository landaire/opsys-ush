//
// Created by Lander Brandt on 4/1/15.
//

#include "linked_list.h"

// TODO: Add to header when Stu tells us we can
void addAfter(LinkedList *theList, Node *after, Node *nn);

/**
 * Creates a new circular doubly-linked list with a dummy head node
 * The caller is responsible for freeing the list (obviously)
 */
LinkedList *linkedList() {
    LinkedList *returnList = calloc(1, sizeof(LinkedList));
    Node *dummyHead = calloc(1, sizeof(Node));
    // Circular!
    dummyHead->next = dummyHead;
    dummyHead->prev = dummyHead;

    // All linked lists have a dummy head node
    returnList->head = dummyHead;

    return returnList;
}

/**
 * Adds the given node to the end of the linked list
 */
void addLast(LinkedList * theList, Node * nn) {
    addAfter(theList, theList->head->prev, nn);
}

/**
 * Inserts the given node at the beginning of the linked list
 */
void addFirst(LinkedList * theList, Node * nn) {
    addAfter(theList, theList->head, nn);
}

/**
 * Inserts the given node after the "after" node
 */
void addAfter(LinkedList *theList, Node *after, Node *nn) {
    // Since we're in a doubly-linked list with a dummy head node, we don't have to worry about
    // null pointer dereferences here. If this is the first node being added to the list, then "after" will be head.
    // by default after->next is after, so nn->next will point back to head. That's circular in one direction.
    // When we set nn->next->prev, that's setting head->prev to be nn so we're now circular in both directions!
    // The same deal works for if this node is added last.
    nn->prev = after;
    nn->next = after->next;
    nn->next->prev = nn;
    after->next = nn;

    theList->size++;
}

/**
 * Removes the given node from the linked list. removeData is some function which will free the data in the node,
 * while compare is the comparison function
 */
void removeItem(LinkedList * theList, Node * nn, void (*removeData)(void *), int (*compare)(const void *, const void *)) {
    Node *currentNode = theList->head;
    size_t currentIndex = 0;

    while (currentIndex < theList->size) {
        currentNode = currentNode->next;

        // If the compare method returns 0, we've found the node we're looking for and can remove it
        if (!compare(currentNode, nn)) {
            // Remove dangling pointers to this node.
            currentNode->prev->next = currentNode->next;
            currentNode->next->prev = currentNode->prev;

            // Clean up our data.
            removeData(currentNode->data);
            free(currentNode);

            theList->size--;
            // This really doesn't do anything since we break the loop, BUT if this is a removeAll method then this
            // would prevent an off-by-one error
//            currentIndex--;

            break;
        }

        // To avoid an infinite loop break if we reach the head again. This can happen if the item isn't found
        if (currentNode == theList->head) {
            break;
        }

        currentIndex++;
    }

    removeData(nn->data);
    free(nn);
}

/**
 * Clears all items in the list and calls removeData for each item
 */
void clearList(LinkedList * theList, void (*removeData)(void *)) {
    Node *currentNode = theList->head;

    while (theList->size > 0) {
        currentNode = currentNode->next;

        removeData(currentNode->data);

        free(currentNode->prev);
        theList->size--;
    }

    free(currentNode);
}

/**
 * Prints the list by calling printData for each node.
 */
void printList(FILE *outFile, const LinkedList * theList, void (*printData)(FILE *outFile, void * a)) {
    Node *currentNode = theList->head->next;

    while (currentNode != theList->head) {
        printData(outFile, currentNode->data);

        currentNode = currentNode->next;
    }
}

/**
 * Sorts the given linked list.
 * this is probably the coolest method ever made
 */
void sort(LinkedList * theList, int (*compare)(const void *, const void *)) {
    // Create an array of data pointers that we'll pass to qsort
    void **nodeData = calloc((size_t)theList->size, sizeof(void*));

    Node *currentNode = theList->head;
    size_t index = 0;
    while (index < theList->size) {
        currentNode = currentNode->next;

        nodeData[index] = currentNode->data;
        index++;
    }

    qsort(nodeData, (size_t)theList->size, sizeof(void*), compare);

    // Walk through the list again. The first real node will have the first sorted data, second node will have the
    // second sorted data, etc.
    currentNode = theList->head;
    index = 0;
    while (index < theList->size) {
        currentNode = currentNode->next;

        currentNode->data = nodeData[index];
        index++;
    }

    free(nodeData);
}

Node * buildNode(int argc, const char ** argv, void *(*buildData)(int argc, const char ** argv) ) {
    Node *temp = calloc(1, sizeof(Node));

    temp->data = buildData(argc, argv);

    return temp;
}
