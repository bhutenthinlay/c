#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//structure of node
struct node{
    int value;
    struct node* next;
};

typedef struct node node_t;
void print_nodes(node_t *head){
    node_t *tmp = head;
    while(tmp != NULL){
        printf("value = %d ->", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}
node_t *create_node(int value){
    node_t *tmp;
    tmp =(node_t *) malloc(sizeof(node_t));
    tmp->value = value;
    tmp->next = NULL;
    return tmp; 
}

node_t *insert_at_the_head(node_t *head, node_t *insert_node){
    //head = insert_at_the_head;
    insert_node->next = head;
    head = insert_node;
    return head;
}
node_t *insert_at_the_end(node_t *head, node_t *insert_node){
   
    if(head == NULL){ //if list is empty
        head = insert_node;
    }else{ //if not
        node_t *tail = head;
        while(tail->next != NULL){
            tail = tail->next;
        }
        tail->next = insert_node;
    }
    return head;
}
void find_node(node_t *head, int value){
    bool gotvalue = false;
    while(head != NULL){
        if(head->value == value){
            printf("Found %d in the list\n", value);
            gotvalue = true;
            break;
        }else{
            head = head->next;
        }
    }
    if(!gotvalue){
        printf("Value %d not found\n", value);
    }
}
int main(){
    node_t *tmp, *tmp2;
    node_t *head = NULL, *tail= NULL;
    // tmp = create_node(10);
    // head = tmp;
    // tmp = create_node(20);
    // tmp->next = head;
    // head = tmp;
    // tmp = create_node(30);
    // tmp->next = head;
    // head = tmp;

    // tmp = create_node(10);
    // head = tmp;
    // tmp = create_node(20);
    // head->next= tmp;
    // tail = tmp;
    // tmp = create_node(30);
    // tail->next= tmp;
    // tail = tmp;
    // tmp = create_node(40);
    // tail->next= tmp;
    // tail = tmp;
    // tmp = create_node(50);
    // tail->next= tmp;
    // tail = tmp;
    //create 25 nodes
    for(int i =0; i<25; i++){
        tmp = create_node(i);
        if(tail == NULL){
            head = tmp;
            tail = tmp;
        }else{
            tail->next = tmp;
            tail = tmp;
        }
    }
    
    print_nodes(head);
    printf("************************************************************************\n");
    tmp = create_node(100);
    head = insert_at_the_head(head, tmp);
    print_nodes(head);
    printf("************************************************************************\n");
    tmp = create_node(101);
    head = insert_at_the_end(head, tmp);
    print_nodes(head);
    printf("************************************************************************\n");
    find_node(head, 25);

    return EXIT_SUCCESS;
}