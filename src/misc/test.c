#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>

int main(int argc,char ** argv){

    printf("\n==============test string\n");
    hlite_string * p;
    p=hlite_init_string();
    hlite_fill_string(p,"hello");
    printf("string len:%d,string:%s\n",p->len,p->data);
    printf("\n\n");


    printf("\n==============test list \n");
    hlite_list * list;
    list= hlite_list_new (6);
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
    segments=hlite_list_new(MIDDLE);
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
    /**
     * just test the keyval pair */
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

    printf("\n======== test keyval pair and list\n");
    hlite_list * lista;
    lista=hlite_list_new(8);
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

   return 0;
}

