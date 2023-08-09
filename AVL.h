#ifndef FINAL_ASSIGNMENT_AVL_H
#define FINAL_ASSIGNMENT_AVL_H

#include "stdio.h"
#include "stdlib.h"

struct Record{
    char note[100];
};


struct Tracking{
    int recordCounter;
    unsigned int totalTransfer;
    unsigned int totalReceive;
    unsigned int totalCashOut;
    unsigned int trans_perDay;
    unsigned int cashOut_perDay;
    double loan_payment;
    double total_loan_payment;
    int loanTime_rec_counter;
    int repaid_switch;
    char loanTime_Rec[30];
    char repaidTime_Rec[30];
};

struct Seed{
    unsigned int id;
    char name[30];
    char email[50];
    unsigned long long int phNumber;
    char nrc[50];
    char address[100];
    char password[50];
    char pOrb[20]; // personal or business
    unsigned int cur_amount;
    double total_loanAmount;
    unsigned int transAmoLimitPerDay;
    int acc_level;
    struct Record record[300];
    struct Tracking tracking;
    int index;
};

struct Node{
    struct Seed *seed;
    struct Node *left;
    struct Node *right;
    int height;
};

struct Seed *newSeed(struct Seed *cultivate);
struct Node *newNode(struct Seed *cultivate);
struct Node *leftRotate(struct Node *x);
struct Node *rightRotate(struct Node *y);
struct Node *insertNode(struct Node *node,struct Seed *data_seed);
struct Node *minValueNode(struct Node *node);
struct Node *deleteNode(struct Node *root,unsigned int key);

int height(struct Node *N);
int maX(int a, int b);
int getBalance(struct Node *N);

void preOrder(struct Node *root);
void inOrder(struct Node *root);
void postOrder(struct Node *root);

int height(struct Node *N){
    if(N == NULL){
        return 0;
    }
    return N->height;
}

int maX(int a, int b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}

int getBalance(struct Node *N){
    if(N==NULL){
        return 0;
    }
    return height(N->left)-height(N->right);
}

struct Seed *newSeed(struct Seed *cultivate){
    struct Seed *new_seed=(struct Seed*)malloc(sizeof(struct Seed));
    new_seed=cultivate;
    return new_seed;
}

struct Node *newNode(struct Seed *cultivate){
    struct Node *node=(struct Node*)malloc(sizeof(struct Node));
    node->seed = newSeed(cultivate);
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
}

struct Node *minValueNode(struct Node *node){
    struct Node *current = node;
    while(current->left != NULL){
        current=current->left;
    }
    return current;
}

struct Node *leftRotate(struct Node *x){

    struct Node *y = x->right;
    struct Node *z = y->left;

    y->left=x;
    x->right=z;

    x->height=1+maX(height(x->left),height(x->right));
    y->height=1+maX(height(y->left),height(y->right));

}

struct Node *rightRotate(struct Node *y){

    struct Node *x = y->left;
    struct Node *z = x->right;

    x->right=y;
    y->left=z;

    y->height=1+maX(height(y->left),height(y->right));
    x->height=1+maX(height(x->left),height(x->right));

}

struct Node *insertNode(struct Node *node,struct Seed *data_seed){
//insert data
    if(node == NULL){
        return (newNode(data_seed));
    }
    if(data_seed->id < node->seed->id){
        node->left=insertNode(node->left,data_seed);
    }else if(data_seed->id  > node->seed->id){
        node->right= insertNode(node->right,data_seed);
    }else{
        return node;
    }

//balance the tree
    node->height = 1+max(height(node->left),height(node->right));
    int balance = getBalance(node);

//Rotations
    if(balance > 1 && data_seed->id < node->left->seed->id){
        return rightRotate(node);
    }if(balance < -1 && data_seed->id > node->right->seed->id){
        return leftRotate(node);
    }if(balance > 1 && data_seed->id > node->left->seed->id){
        node->left=leftRotate(node->left);
        return rightRotate(node);
    }if(balance < -1 && data_seed->id < node->right->seed->id){
        node->right=leftRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct Node *deleteNode(struct Node *root,unsigned int key){
    //find the node & delete

    if(key < root->seed->id){
        root->left=deleteNode(root->left,key);
    }else if(key > root->seed->id){
        root->right=deleteNode(root->right,key);

    }else {
        if((root->left==NULL) || (root->right==NULL)){

            struct Node *temp=NULL;

            if(root->left!=NULL){
                temp = root->left;
            }
            if(root->right!=NULL){
                temp = root->right;
            }

            if(temp==NULL){
                temp=root;
                root=NULL;
            }else{
                *root=*temp;
                temp=NULL;
            }

        }else{
            struct Node *temp = minValueNode(root->right);
            root->seed->id = temp->seed->id;
            root->right = deleteNode(root->right, temp->seed->id);
        }
    }

    if(root==NULL){
        return root;
    }

    root->height=1+maX(height(root->left),height(root->right));
    int balance=getBalance(root);

    if(balance > 1 && getBalance(root->left) >= 0){
        return rightRotate(root);
    }if(balance > 1 && getBalance(root->left) < 0){
        root->left= leftRotate(root->left);
        return rightRotate(root);
    }if(balance < -1 && getBalance(root->right) <= 0){
        return leftRotate(root);
    }if(balance < -1 && getBalance(root->right) > 0){
        root->right= rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void preOrder(struct Node *root){

    if(root != NULL){
        printf("%u-%s-%s-%llu-%s-%s-%s-%s-%u-%.0f-%u-%d",root->seed->id ,root->seed->name ,root->seed->email ,
               root->seed->phNumber ,root->seed->nrc ,root->seed->address ,root->seed->password ,root->seed->pOrb ,
               root->seed->cur_amount , root->seed->total_loanAmount, root->seed->transAmoLimitPerDay, root->seed->acc_level);

        for(int gcc=0; gcc<root->seed->tracking.recordCounter ; gcc++){
            printf("\n");
            printf(" %s",root->seed->record[gcc].note);
        }
        printf("\n\n");
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(struct Node *root){

    if(root != NULL){
        inOrder(root->left);
        printf("%u-%s-%s-%llu-%s-%s-%s-%s-%u-%.0f-%u-%d",root->seed->id ,root->seed->name ,root->seed->email ,
               root->seed->phNumber ,root->seed->nrc ,root->seed->address ,root->seed->password ,root->seed->pOrb ,
               root->seed->cur_amount , root->seed->total_loanAmount, root->seed->transAmoLimitPerDay, root->seed->acc_level);

        for(int gcc=0; gcc<root->seed->tracking.recordCounter ; gcc++){
            printf("\n");
            printf(" %s",root->seed->record[gcc].note);
        }
        printf("\n\n");
        inOrder(root->right);
    }
}

void postOrder(struct Node *root){

    if(root != NULL){
        postOrder(root->left);
        postOrder(root->right);
        printf("%u-%s-%s-%llu-%s-%s-%s-%s-%u-%.0f-%u-%d",root->seed->id ,root->seed->name ,root->seed->email ,
               root->seed->phNumber ,root->seed->nrc ,root->seed->address ,root->seed->password ,root->seed->pOrb ,
               root->seed->cur_amount , root->seed->total_loanAmount, root->seed->transAmoLimitPerDay, root->seed->acc_level);

        for(int gcc=0; gcc<root->seed->tracking.recordCounter ; gcc++){
            printf("\n");
            printf(" %s",root->seed->record[gcc].note);
        }
        printf("\n\n");
    }
}

void preOrder_track(struct Node *root){
    if(root != NULL){
        printf("%d-%u-%u-%u-%u-%u",root->seed->tracking.recordCounter ,root->seed->tracking.totalTransfer ,
               root->seed->tracking.totalReceive ,root->seed->tracking.totalCashOut ,root->seed->tracking.trans_perDay ,
               root->seed->tracking.cashOut_perDay);

        printf("\n");
        preOrder_track(root->left);
        preOrder_track(root->right);
    }
}

void preOrder_loan(struct Node *root){
    if(root != NULL){
        printf("%.2f-%.2f",root->seed->tracking.loan_payment,root->seed->tracking.total_loan_payment);

        printf("\n");
        preOrder_loan(root->left);
        preOrder_loan(root->right);
    }
}

#endif //FINAL_ASSIGNMENT_AVL_H
