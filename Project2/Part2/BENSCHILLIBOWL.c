#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(BENSCHILLIBOWL* bcb);
bool IsFull(BENSCHILLIBOWL* bcb);
void AddOrderToBack(Order **orders, Order *order);



// help from Ujjwal
// worked together with Anirudd


MenuItem BENSCHILLIBOWLMenu[] = { 
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};
int BENSCHILLIBOWLMenuLength = 10;

/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
  return BENSCHILLIBOWLMenu[rand()%BENSCHILLIBOWLMenuLength];
}

/* Allocate memory for the Restaurant, then create the mutex and condition variables needed to instantiate the Restaurant */

BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {
    BENSCHILLIBOWL *bcb=(BENSCHILLIBOWL*) malloc(sizeof(BENSCHILLIBOWL));
    bcb->orders=NULL;
    bcb->current_size=0;
    bcb->max_size=max_size;
    bcb->next_order_number=1;
    bcb->orders_handled=0;
    bcb->expected_num_orders=expected_num_orders;
    if(pthread_mutex_init(&bcb->mutex,NULL) || pthread_cond_init(&bcb->can_add_orders,NULL) || pthread_cond_init(&bcb->can_get_orders,NULL)){
      perror("Error\n");
      exit(1);
    }
    printf("Restaurant is open!\n");
    return bcb;
}


/* check that the number of orders received is equal to the number handled (ie.fullfilled). Remember to deallocate your resources */

void CloseRestaurant(BENSCHILLIBOWL* bcb) {
    Order *order = bcb->orders;
    while(!IsEmpty(bcb)){
      Order *curr=bcb->orders;
      order=order->next;
      bcb->current_size-=1;
      free(curr);
    }
    pthread_mutex_destroy(&bcb->mutex);
    pthread_cond_destroy(&bcb->can_add_orders);
    pthread_cond_destroy(&bcb->can_get_orders);
    free(bcb);
    printf("Restaurant is closed!\n");
}

/**
 * Add an order to the restaurant. This function should:
 *  - Wait until the restaurant is not full
 *  - Add an order to the back of the orders queue
 *  - populate the order number of the order
 *  - return the order number
 */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
    pthread_mutex_lock(&bcb->mutex);
    int final=bcb->next_order_number;
    if(!IsFull(bcb)){
      order->order_number=bcb->next_order_number;
      AddOrderToBack(&bcb->orders,order);
      bcb->next_order_number+=1;
      bcb->current_size+=1;
    }
    else{
      while(IsFull(bcb)){
        pthread_cond_wait(&bcb->can_add_orders,&bcb->mutex);
      }
      order->order_number=bcb->next_order_number;
      AddOrderToBack(&bcb->orders,order);
      bcb->current_size+=1;
      bcb->next_order_number+=1;
    }
  pthread_cond_signal(&bcb->can_get_orders);
  pthread_mutex_unlock(&bcb->mutex);
  return final;
}

/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* bcb) {
    pthread_mutex_lock(&bcb->mutex);
    Order *order;
    while(IsEmpty(bcb)){
      pthread_cond_wait(&bcb->can_get_orders,&bcb->mutex);
    }
    order= bcb->orders;
    bcb->orders=bcb->orders->next;
    order->next=NULL;
    bcb->current_size-=1;
    bcb->orders_handled-=1;
    pthread_cond_signal(&bcb->can_add_orders);
    pthread_mutex_unlock(&bcb->mutex);
    return order;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(BENSCHILLIBOWL* bcb) {
  return (bcb->current_size==0);
}

bool IsFull(BENSCHILLIBOWL* bcb) {
  return (bcb->current_size==bcb->max_size);
}

/* this methods adds order to rear of queue */
void AddOrderToBack(Order **orders, Order *order) {
  Order *curr=*orders;
  if(!curr){
    *orders=order;
    return;
  }
  while(curr->next!=NULL){
    curr=curr->next;
  }
  curr->next=order;
  order->next=NULL;
}

