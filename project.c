#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
typedef struct node{
    char *data;
    struct node * right ,* left;
    int hight;
}node;
node *init(char *s){
    node *x=malloc(sizeof(node));
    if(x==NULL)
    return NULL;
    x->data=malloc(strlen(s)+1);
    strcpy(x->data,s);
    x->left=NULL;
    x->right=NULL;
    x->hight=1;
    return x;
}
int max(int x,int y){
    return x>y? x:y;
}
int get_hight(node *x){
    if(x==NULL)
        return 0;
    return x->hight;
}
int get_balance(node *x){
    if(x==NULL)
        return 0;
    return get_hight(x->left)-get_hight(x->right);
}
node *rotate_right(node *x){
    node *temp;
    temp=x->left;
    node *temp1=temp->right;
    temp->right=x;
    x->left=temp1;
    x->hight=1+max(get_hight(x->left),get_hight(x->right));
    temp->hight=1+max(get_hight(temp->left),get_hight(temp->right));
    return temp;
}
node * rotate_left(node *root){
    node *temp;
    temp=root->right;
    node *temp1=temp->left;
    temp->left=root;
    root->right=temp1;
    root->hight=1+max(get_hight(root->left),get_hight(root->right));
    temp->hight=1+max(get_hight(temp->left),get_hight(temp->right));
    return temp;
}
node *search_node(node *root,node *s){
    node *x;
    if(root==NULL)
        return NULL;
    node *temp=root;
    while(temp!=NULL){
        if(strcasecmp(s->data,temp->data)>0)
            temp=temp->right;
        else if (strcasecmp(s->data,temp->data)<0)
            temp=temp->left;
        else{
            x=temp;
            return x;
        }
    }
    return NULL;
}
node *insert_node(node *root,char *s){
    if(root==NULL){
        node *x=init(s);
        root=x;
        return root;
    }
    if(strcasecmp(s,root->data)<0){
        root->left=insert_node(root->left,s);
    }
    else if (strcasecmp(s,root->data)>0){
        root->right=insert_node(root->right,s);
    }
    else{
        printf("the name is exist in the dictionary\n");
        return root;
    }
    root->hight=1+max(get_hight(root->left),get_hight(root->right));
    //check balance
    int balance=get_balance(root);
    //RR
    if(balance<-1&&root->right!=NULL&&strcasecmp(s,root->right->data)>0){
            return rotate_left(root);
    }
    //LL
    else if (balance>1&&root->left!=NULL&&strcasecmp(s,root->left->data)<0){
        return rotate_right(root);
    }
    //LR
    else if (balance>1&&root->left!=NULL&&strcasecmp(s,root->left->data)>0){
        root->left=rotate_left(root->left);
        return rotate_right(root);
    }
    //RL
    else if (balance<-1&&root->right!=NULL&&strcasecmp(s,root->right->data)<0){
        root->right=rotate_right(root->right);
        return rotate_left(root);
    }
    // if he arrive her so the subtree is balanced
    return root;
}
node *max_node(node *root){
    if(root==NULL)
        return NULL;
    node *temp;
    temp=root;      //can i make it using the root only
    while(temp->right!=NULL)
        temp=temp->right;
    return temp;
}
node *min_node(node *root){
    if(root==NULL)
        return NULL;
    node *temp;
    temp=root;   
    while(temp->left!=NULL)
        temp=temp->left;
    return temp;
}
node * get_successor(node *root,node*s){
    node *successor;
    if(s==NULL)
    return NULL;
    node *temp=search_node(root,s);
    if(temp==NULL)
        return NULL;
    if(temp->right!=NULL){
        successor=min_node(temp->right);
        return successor;
    }
    else{           // we search for the depest parent making the node in the left
        successor=NULL;
        node *current=root;     //we can make it using the root directly
        while(current!=temp){
            if(strcasecmp(temp->data,current->data)>0){
                current=current->right;
            }
            else{
                successor=current;
                current=current->left;
            }
        }
        return successor;
    }
}
node *get_predecessor(node *root,node *s){
    node *predecessor;
    node *temp=search_node(root,s);
    if(temp==NULL||s==NULL){
        return NULL;
    }
    if(temp->left!=NULL){
        predecessor=max_node(temp->left);
        return predecessor;
    }
    else{
        predecessor=NULL;
        node *current=root;     //we can make it using the root directly
        while(current!=temp){
            if(strcasecmp(temp->data,current->data)>0){
                predecessor=current;
                current=current->right;
            }
            else{
                current=current->left;
            }
        }
        return predecessor;
    }
}
void avl_search(node *root,char *x){
    if (root == NULL) {
        printf("Dictionary is empty.\n");
        return;
    }
    int flag =0;
    node *temp=root,*parent=NULL,*successor1,*predecssor1;;
    while(temp!=NULL){
    parent=temp;
    if(strcasecmp(x,temp->data)>0)
        temp=temp->right;
    else if(strcasecmp(x,temp->data)<0)
        temp=temp->left;
    else if (strcasecmp(x,temp->data)==0){
         printf("%s -CORRECT\n",x);
         flag=1;
         break;
    }    
    }
    if(!flag){
    printf("%s - Incorrect, Suggestions : ", x);   
    successor1=get_successor(root,parent);
    predecssor1=get_predecessor(root,parent);
    printf("%s",parent->data);
    if(predecssor1!=NULL)
        printf(" %s",predecssor1->data);
    if(successor1!=NULL)
        printf(" %s",successor1->data);
    printf("\n");}
}
int get_size(node *root){
    if(root==NULL)
        return 0;
    else
        return 1+get_size(root->left)+get_size(root->right);
}
node * load_file(char *filename){
    FILE * fp=fopen(filename,"r");
    if (fp == NULL) {
    printf("Error: file not found\n");
    return NULL;
    }
    node *root;
    root=NULL;
    char s[1000];
    while(fgets(s,1000,fp)!=NULL){
    s[strcspn(s,"\n")]='\0';
    root=insert_node(root,s);
    }
    printf("Dictionary loaded successfully\n----------------------\n");
    printf("Size of of the tree: %d\n----------------------\n",get_size(root));
    printf("Hight of the tree: %d\n----------------------\n",get_hight(root));
    fclose(fp);
    return root;
}
int main(){
    node *root=load_file("Dictionary.txt");
    char s[1000];
    printf("Enter a sentence: ");
    fgets(s,1000,stdin);
    s[strcspn(s, "\n")] = '\0';
    char *token;
    token=strtok(s," ");
    while(token!=NULL){
        avl_search(root,token);
        token=strtok(NULL," ");
    }
    return 0;
}