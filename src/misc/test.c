#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>
#define HL_NEW_QUEUE(x) \
    x=malloc(sizeof(hlite_queue));\
    x->tail=NULL; \
    x->head=NULL;
#define HL_NEW_QUEUE_ITEM(x) \
        x=malloc(sizeof(hlite_queue_item));
#define HL_INIT_QUEUE_ITEM(x,init) \
        x=malloc(sizeof(hlite_queue_item)); \
        x->data=(void * ) init;
#define HL_RESET_POOL(x) \
    x->total_size=0; \
    x->head=NULL;\
    x->using_pool=NULL;\
    x->tail=NULL;\
    x->current_size=0;\
    x->chain=NULL;\
    x->larges=NULL;\
    x->next=NULL; \
    x->error_times=0;

#define DEBUG_MEM 1
#define HL_ALLOC(p,size) p=malloc(size); \
    if(DEBUG_MEM) printf("DLOG:malloc at:%s#%d,size:%d,pointer:%p\n",__FILE__,__LINE__,size,p);
#define HL_FREE(x) if(x!=NULL) { \
    if(DEBUG_MEM) printf("DLOG:free at:%s#%d,pointer:%p\n",__FILE__,__LINE__,x);\
    free(x);\
    x=NULL;\
}
#define HL_LARGE_SIZE 8192


void * hl_pool_alloc(hlite_pool * pool,size_t size);

void * hl_init_pool(hlite_pool * pool,size_t size){
    char * p;
    DHERE
    HL_ALLOC(p,size);
    DHERE
    if(p==NULL)
       return NULL;
    DHERE
    pool->total_size=size;
    DHERE
    pool->head=pool->last=p;
    pool->tail=p+size;
    DHERE
    pool->using_pool=pool;
}
/** alloc large memeory space ,and asign it to the pool 
 * */
void * hl_pool_alloc_large(hlite_pool * pool,size_t size){
    if(pool==NULL) return ;
    hlite_pool * new;
    if(pool->larges==NULL){
        pool->larges=hl_pool_alloc(pool,sizeof(hlite_pool));
    }
    if(pool->larges==NULL){
        return NULL;
    }
    HL_RESET_POOL(pool->larges);
    DHERE
    new=hl_pool_alloc(pool,sizeof(hlite_pool));
    DHERE
    if(new==NULL) return NULL;
    DHERE
    HL_RESET_POOL(new); 
    hl_init_pool(new,size);
    hlite_pool * p;
    p=pool->larges;
    while(p->next!=NULL){
        p=p->next;
    }
    p->next=new;
    return new->head;
}
/**
 * 重置 pool,这之前请记得已经释放了全部的large
 * */
void  hl_pool_reset(hlite_pool * pool){
    if(pool==NULL) return ;
    hlite_pool * pt;
    if(pool->using_pool!=NULL){
        for(pt=pool->using_pool;pt!=NULL;pt=pt->next){
            HL_FREE(pt->head);
            HL_RESET_POOL(pt);
        }
    }
    HL_FREE(pool->head);
    HL_RESET_POOL(pool);
}

/**
 * 释放pool中所有的larges
 * */
void hl_pool_clear(hlite_pool * pool){
    if(pool==NULL) return ;
    hlite_pool * p;
    p=pool->larges;
    if(p!=NULL){
        HL_FREE(p->head);
        while(p->next!=NULL){
           p=p->next;
           if(p==NULL){
               break;
            }else{
                HL_FREE(p->head);
            }
        }
    }
    hl_pool_reset(pool);
}
/**
 * alloc memory from the pool;
 */
void * hl_pool_alloc(hlite_pool * pool,size_t size){
    if(pool==NULL) return NULL;
    void * p;
    hlite_pool * pt;
    DHERE
    if (size>HL_LARGE_SIZE){
        //alloc a large block,and add it to the large chains;
        return hl_pool_alloc_large(pool,size);
    }
    /*
     * find from the pool chains
     */
    DHERE
    if(pool->using_pool!=NULL){
        for(pt=pool->using_pool;pt!=NULL;pt=pt->next){
            DHERE
            if(pt->error_times< 8 && size<(pool->total_size-pool->current_size)){
                //alloc from the pool
                p=pt->last+size;
                DHERE
                pt->last=p;
                pt->current_size+=size;
                return p;
            }
            else{
                printf("can't get enough space \n");
                //exit(1);
            }
            /** if there is no enough space,log it */

        }
        DHERE
    }
    else{
        DHERE
        printf("using_pool is empty");
        exit(2);
    }
    DHERE
    /** if we can't find space from pools,we create new one 
         and add to the "next" */
    hlite_pool * new;
    new=hl_pool_alloc(pool,sizeof(hlite_pool));
    if(new==NULL){
        HL_ALLOC(new,sizeof(hlite_pool));
    }
    if(new==NULL) return NULL;
    DHERE
    HL_RESET_POOL(new); 
    hl_init_pool(new,2 * pool->total_size);
    new->next=pool->using_pool;
    pool->using_pool=new;      
}
int main(int argc,char ** argv){

    printf("\n==============test string\n");

    /**
    hlite_string * p;
    p=hlite_init_string();
    hlite_fill_string(p,"hello");
    printf("string len:%d,string:%s\n",p->len,p->data);
    printf("\n\n");


    printf("\n==============test list \n");
    hlite_list * list;
    list= hlite_new_list (6);
    char * str1;
    char * str2;
    str1=malloc(sizeof(char)*6);
    str2=malloc(sizeof(char)*6);
    memcpy(str1,"hello",6);
    memcpy(str2,"world",6);
    hlite_list_append(list,str1);
    hlite_list_append(list,str2);


    printf("%d\n",str1);
    printf("%d",str2);

    int i=0;
    for(;i<list->pos;i++){
        printf("%s\n",(char * )list->p[i]);
    }
    printf("is eof:%d\n",hlite_list_is_eof(list));
    char * j;
    j=hlite_list_pop(list);
    int list_len,k;
    DHERE;

    hlite_string * test_str;
    hlite_string * test_sep;
    hlite_list * segments;

    printf("\n==============test string_split \n");
    test_str=hlite_new_string("http://www.handle.com/?gdoooo");
    segments=hlite_new_list(MIDDLE);
    test_sep=hlite_new_string("?");
    DHERE;
    printf("test_sep:%s\n",test_sep->data);
    printf("test_str:%s\n",test_str->data);

    list_len=hlite_split(test_sep,test_str,segments,1);
    for( k=0;k<segments->pos;k++){
        test_str=segments->p[k];
        printf("new item:%s\n",test_str->data);
    }
    printf("split returns:%d\n",list_len);
    
    list_len=hlite_split(test_sep,test_str,segments,0);
    for( k=0;k<segments->pos;k++){
        test_str=segments->p[k];
        printf("new item:%s\n",test_str->data);
    }
    printf("split returns:%d\n",list_len);
    //just test the keyval pair 
    printf("\n============ test keyval pair\n");


    DHERE
    hlite_string * key1;
    hlite_string * val1;
    key1=hlite_init_string();
    val1=hlite_init_string();
    hlite_fill_string(key1,"myfirstkey");
    hlite_fill_string(val1,"myfirstvalue");
    DHERE
    hlite_keyval_pair * pr;
    hlite_keyval_pair * pr2;
    pr=hlite_init_keyval_pair();
    pr2=hlite_init_keyval_pair();
    pr->key=key1;
    pr->value=val1;
    printf("visit the keyval pari:key:%s,\tvalue:%s\n",pr->key->data,pr->value->data);

    hlite_string * key2;
    hlite_string * val2;
    key2=hlite_init_string();
    hlite_fill_string(key2,"root");

    val2=hlite_init_string();
    hlite_fill_string(val2,"/home/z/share/");
    pr2->key=key2;
    pr2->value=val2;

    hlite_keyval_pair * pir3;
    pir3=hlite_init_keyval_pair_withkv(key2,val2);
    printf("\nkey equals '%s':%s ?\nvalue equals '%s':%s \n",
            key2->data,pir3->key->data,val2->data,pir3->value->data);


    printf("\n======== test dicts =======\n");
    hlite_dict * dict;
    dict=hlite_new_list(4);
    hlite_dict_set(dict,key2,val2);
    hlite_dict_set(dict,key1,val1);
    hlite_string * test_string;
    test_string = hlite_dict_get(dict,key1);
    printf("value of dict[key1]:%s\n",test_string->data);
    test_string = hlite_dict_get(dict,key2);
    printf("value of dict[key1]:%s\n",test_string->data);





    printf("\n======== test keyval pair and list\n");
    hlite_list * lista;
    lista=hlite_new_list(8);
    hlite_list_append(lista,pr);
    hlite_list_append(lista,pr2);

    hlite_keyval_pair * temppair;
    printf("current lits position:%d\n",lista->pos);
    int x=0;
    for(;x<lista->pos;x++){
        temppair=lista->p[x];
        printf("address:%f\n",temppair);
        printf("[notice] the key:%s\t the value:%s\t\n",temppair->key->data,temppair->value->data);

    }
    DHERE
    while(temppair=(hlite_keyval_pair *) hlite_list_pop(lista)){
        printf("[notice] the key:%s\t the value:%s\t\n",temppair->key->data,temppair->value->data);
        free(temppair);
    }
   
    printf("\n======= test parse_config_file =========\n");
    hlite_dict * conf;
    conf=hlite_new_list(16);
    int parsed=0;
    parsed=hlite_parse_config_file(hlite_new_string("./../etc/demo.conf"), conf);
    printf("length:%d\n",conf->pos);
    for(x=0;x<conf->pos;x++){
        temppair=conf->p[x];
        assert(temppair);
        printf("[notice] the key:%s\t the value:%s\t\n",temppair->key->data,temppair->value->data);
    }

    printf("\n======== test queue =========\n");
    hlite_queue * queue;
//    queue=malloc(sizeof(hlite_queue));
//    queue->tail=NULL;
//    queue->head=NULL;
    
    HL_NEW_QUEUE(queue);
    //add to the queue;
    char testing[10][100]={
        "God bless you",
        "Hi",
        "I am a boy",
        "Where are you from?"
    };
    int qi=0;
    hlite_queue_item * qitem;
    for(;qi<4;qi++){
        HL_INIT_QUEUE_ITEM(qitem,testing[qi]);
        //qitem=malloc(sizeof(hlite_queue_item));
        //qitem->data=(void * ) testing[qi];
        if(qi==0){
            queue->head=qitem;
            queue->tail=qitem;
        }
        else{
            queue->tail->next=qitem;
            qitem->prev=queue->tail;
            queue->tail=qitem;
        }
    }
    qitem=queue->head;
    while(1){
        printf("data:%s\n",qitem->data);
        if(qitem==queue->tail)
            break;
        qitem=qitem->next;
    }
    */
    /*** testing the pool */
    printf("\n======== test pool =========\n");
    hlite_pool * pool;
    HL_ALLOC(pool,sizeof(hlite_pool));
    HL_RESET_POOL(pool);
    hl_init_pool(pool,BUFSIZE);
    char * pl1=hl_pool_alloc(pool,sizeof(char )*100);
    printf("debug:new alloced:p1:%p\n",pl1);
    char * pl2=hl_pool_alloc(pool,sizeof(char )*200);
    printf("debug:new alloced:p2:%p\n",pl2);
    char * pl4=hl_pool_alloc(pool,sizeof(char )*500);
    printf("debug:new alloced:p4:%p\n",pl4);
    char * pl3=hl_pool_alloc(pool,sizeof(char )*8000);
    printf("debug:new alloced:p3:%p\n",pl3);
    /*
    hl_pool_reset(pool);
    */
    hl_pool_clear(pool);
    HL_FREE(pool);
    return 0;
}

