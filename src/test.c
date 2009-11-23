#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>

int main(int argc,char ** argv){

    printf("\n==============test string\n");

    /**
    hl_string * p;
    p=hl_init_string();
    hl_fill_string(p,"hello");
    printf("string len:%d,string:%s\n",p->len,p->data);
    printf("\n\n");


    printf("\n==============test list \n");
    hl_list * list;
    list= hl_new_list (6);
    char * str1;
    char * str2;
    str1=malloc(sizeof(char)*6);
    str2=malloc(sizeof(char)*6);
    memcpy(str1,"hello",6);
    memcpy(str2,"world",6);
    hl_list_append(list,str1);
    hl_list_append(list,str2);


    printf("%d\n",str1);
    printf("%d",str2);

    int i=0;
    for(;i<list->pos;i++){
        printf("%s\n",(char * )list->p[i]);
    }
    printf("is eof:%d\n",hl_list_is_eof(list));
    char * j;
    j=hl_list_pop(list);
    int list_len,k;
    DHERE;

    hl_string * test_str;
    hl_string * test_sep;
    hl_list * segments;

    printf("\n==============test string_split \n");
    test_str=hl_new_string("http://www.handle.com/?gdoooo");
    segments=hl_new_list(MIDDLE);
    test_sep=hl_new_string("?");
    DHERE;
    printf("test_sep:%s\n",test_sep->data);
    printf("test_str:%s\n",test_str->data);

    list_len=hl_split(test_sep,test_str,segments,1);
    for( k=0;k<segments->pos;k++){
        test_str=segments->p[k];
        printf("new item:%s\n",test_str->data);
    }
    printf("split returns:%d\n",list_len);
    
    list_len=hl_split(test_sep,test_str,segments,0);
    for( k=0;k<segments->pos;k++){
        test_str=segments->p[k];
        printf("new item:%s\n",test_str->data);
    }
    printf("split returns:%d\n",list_len);
    //just test the keyval pair 
    printf("\n============ test keyval pair\n");


    DHERE
    hl_string * key1;
    hl_string * val1;
    key1=hl_init_string();
    val1=hl_init_string();
    hl_fill_string(key1,"myfirstkey");
    hl_fill_string(val1,"myfirstvalue");
    DHERE
    hl_keyval_pair * pr;
    hl_keyval_pair * pr2;
    pr=hl_init_keyval_pair();
    pr2=hl_init_keyval_pair();
    pr->key=key1;
    pr->value=val1;
    printf("visit the keyval pari:key:%s,\tvalue:%s\n",pr->key->data,pr->value->data);

    hl_string * key2;
    hl_string * val2;
    key2=hl_init_string();
    hl_fill_string(key2,"root");

    val2=hl_init_string();
    hl_fill_string(val2,"/home/z/share/");
    pr2->key=key2;
    pr2->value=val2;

    hl_keyval_pair * pir3;
    pir3=hl_init_keyval_pair_withkv(key2,val2);
    printf("\nkey equals '%s':%s ?\nvalue equals '%s':%s \n",
            key2->data,pir3->key->data,val2->data,pir3->value->data);


    printf("\n======== test dicts =======\n");
    hl_dict * dict;
    dict=hl_new_list(4);
    hl_dict_set(dict,key2,val2);
    hl_dict_set(dict,key1,val1);
    hl_string * test_string;
    test_string = hl_dict_get(dict,key1);
    printf("value of dict[key1]:%s\n",test_string->data);
    test_string = hl_dict_get(dict,key2);
    printf("value of dict[key1]:%s\n",test_string->data);





    printf("\n======== test keyval pair and list\n");
    hl_list * lista;
    lista=hl_new_list(8);
    hl_list_append(lista,pr);
    hl_list_append(lista,pr2);

    hl_keyval_pair * temppair;
    printf("current lits position:%d\n",lista->pos);
    int x=0;
    for(;x<lista->pos;x++){
        temppair=lista->p[x];
        printf("address:%f\n",temppair);
        printf("[notice] the key:%s\t the value:%s\t\n",temppair->key->data,temppair->value->data);

    }
    DHERE
    while(temppair=(hl_keyval_pair *) hl_list_pop(lista)){
        printf("[notice] the key:%s\t the value:%s\t\n",temppair->key->data,temppair->value->data);
        free(temppair);
    }
   
    printf("\n======= test parse_config_file =========\n");
    hl_dict * conf;
    conf=hl_new_list(16);
    int parsed=0;
    parsed=hl_parse_config_file(hl_new_string("./../etc/demo.conf"), conf);
    printf("length:%d\n",conf->pos);
    for(x=0;x<conf->pos;x++){
        temppair=conf->p[x];
        assert(temppair);
        printf("[notice] the key:%s\t the value:%s\t\n",temppair->key->data,temppair->value->data);
    }

    printf("\n======== test queue =========\n");
    hl_queue * queue;
//    queue=malloc(sizeof(hl_queue));
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
    hl_queue_item * qitem;
    for(;qi<4;qi++){
        HL_INIT_QUEUE_ITEM(qitem,testing[qi]);
        //qitem=malloc(sizeof(hl_queue_item));
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
    hl_pool * pool;
    HL_ALLOC(pool,sizeof(hl_pool));
    HL_RESET_POOL(pool);
    hl_init_pool(pool,BUFSIZE);
    void * pl1=hl_pool_alloc(pool,sizeof(char )*100);
    printf("debug:new alloced:p1:%p\n",pl1);
    void * pl2=hl_pool_alloc(pool,sizeof(char )*200);
    printf("debug:new alloced:p2:%p\n",pl2);
    void * pl4=hl_pool_alloc(pool,sizeof(char )*500);
    printf("debug:new alloced:p4:%p\n",pl4);
    void * pl3=hl_pool_alloc(pool,sizeof(char )*8000);
    printf("debug:new alloced:p3:%p\n",pl3);
    /*
    hl_pool_reset(pool);
    */
    hl_pool_clear(pool);
    HL_FREE(pool);
    return 0;
}

