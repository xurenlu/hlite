#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>

char e404[]="<h1>404 Error</h1><hr><p>File not found!</p>";
char e403[]="<h1>403 Error</h1><hr><p>Fobidden!</p>";
char e50x[]="<h1>500 Error</h1><hr><p>Internel Error!</p>";

void * hl_pool_alloc(hl_pool * pool,size_t size);

/**
 * free memory
 * */
void hl_free(void * ptr){
    if(ptr==NULL){
    }
    else{
        free(ptr);
        ptr=NULL;
    }
}

//
//
/**
 * 初始化一个字符串结构,指定设定为空,长度为0;
 **/

hl_string *  hl_init_string(){
    hl_string * p;
    p=malloc(sizeof(hl_string));
    p->len=0;
    p->data=NULL;
    return p;
}
/**
 * malloc address for the string and set value ;
 * */
int hl_fill_string(hl_string * p,const char * data){
    p->len=(int)strlen(data)+1;
    p->data=malloc(p->len*sizeof(char));
    bzero(p->data,p->len);
    memcpy(p->data,data,p->len);
    return p->len;
}
/**
 * generate hl_string struct from a char point;
 */
hl_string *  hl_new_string(const char * p){
    hl_string * str=hl_init_string();
    hl_fill_string(str,p);
    return str;
}

/**
 * free the string memory
 * */
void hl_string_free(hl_string * str){
    hl_free(str->data);
    hl_free(str);
}

/**
 * generate new LIST */
hl_list * hl_new_list(int size){
    hl_list * list;
    list=(hl_list *) malloc(sizeof(hl_list));

    list->size=size;
    list->pos=0;
    list->p=malloc(sizeof(void *)*size);
    return list;
}
/**
 * append new data to the end of the pointer
 * */
int hl_list_append(hl_list * p,void * data){
    /**
     * handle when pos<size...
     * */
    if(p->pos < p->size ) {
        p->p[p->pos]=data;
        p->pos++;
    }else{
        return -1;//means that ,list is full.
    }
}
/**
 * check if reach the head of the list;
 */
int hl_list_is_bof(hl_list * p){
    if(p->pos==0)
    {
        return 1;
    }
    else
        return 0;
}
/**
 * check  if the list is empty
 * */
int hl_list_is_empty(hl_list * p ){
    return hl_list_is_bof(p);
}
/**
 * check if is the tail of the list 
 * */
int hl_list_is_eof(hl_list * p){
    if(p->pos==p->size)
        return 1;
    else
        return 0;
}
/**
 * under construction
 */
int hl_list_shift(){}

/**
 * pop out the last item of the list , delete it from the list;
 * return NULL if we have reached the end;
 */
void *  hl_list_pop(hl_list  * list){
    if(list->pos>0){
        list->pos--;
        void * p=list->p[list->pos];
        return p;
    }
    else{
        return NULL;
    }
}
/**
 * 释放list资源
 */
void * hl_list_free(hl_list * list){
    hl_free(list);
}
/**
 * split a string to list
 * please call this function twice
 * first with third argument NULL,then you can got a number:the size of the list
 * then,call it again,and pass it with third argument a hl_list variable with the known size;
 * @param int just_first:return when the [separator] first found.
 * */
int hl_split(hl_string * separator,hl_string * str,hl_list * list,int just_first){
    /**
     * when list == NULL,will return the size of new list
     */
    DHERE;
    hl_string * newstr;
    int i=0;
    int j=0;
    int equal=1;
    DHERE;
    if(separator->data==NULL){
        printf("separator->data is null");
    }
    char * pj;
    char * temp;
    pj=malloc(6);
    memcpy(pj,"hello",5);
    int sep_len=separator->len;
    int str_len=str->len;
    DHERE;
    int last_pos=0;
    int return_array_size=0;
    for(i=0;i<(str_len-sep_len);i++){
        equal=1;
        for(j=0;j<sep_len-1;j++){
            if(str->data[i+j]!=separator->data[j]){
                equal=0;
                break;
            }
        }
        if(equal==1){
            DHERE;
            return_array_size++;
            if(list!=NULL){
                temp=str->data+i+j;
                pj=malloc( (i-last_pos)*sizeof(char));
                memcpy(pj,str->data+last_pos,(i-last_pos)*sizeof(char));
                newstr=hl_new_string(pj);
                hl_list_append(list,newstr);
                printf("append:%s\n",pj);
                last_pos=i+j;
            }
        }
        if(equal==1 && just_first){
            equal=0;
            break;
        }

    }
    if(equal==0){
        return_array_size++;
        if(list!=NULL){
            temp=str->data+last_pos+1;
            pj=malloc( (str->len-last_pos)*sizeof(char));
            memcpy(pj,str->data+last_pos,(str->len-last_pos)*sizeof(char));
            newstr=hl_new_string(pj);
            hl_list_append(list,newstr);
        }
        return return_array_size;
    }
    else{
            return return_array_size;
    }


}
/**
 * generate a key-value-pair point
 * */
hl_keyval_pair * hl_init_keyval_pair(){

    hl_keyval_pair * kvpair;
    kvpair=malloc(sizeof(hl_keyval_pair));
    return kvpair;

}
/**
 * set the key and value of a key-value pair ;
 * */
int hl_set_keyval_pair(hl_keyval_pair * kvpair,hl_string * key,hl_string * val){
    hl_string * keystr;
    hl_string * valstr;

    keystr=hl_init_string();
    valstr=hl_init_string();

    hl_fill_string(keystr,key->data);
    hl_fill_string(valstr,val->data);
    kvpair->key=keystr;
    kvpair->value=valstr;
    return 1;
}
/**
 * init a key-value-pair with specificed key and value 
 * */
hl_keyval_pair * hl_init_keyval_pair_withkv(hl_string * key,hl_string * val){
    int ret=-1;
    hl_keyval_pair * kvpair;
    kvpair=hl_init_keyval_pair();
    if(!kvpair) return NULL;
    ret=hl_set_keyval_pair(kvpair,key,val);
    if(ret<0)
        return NULL;
    else
        return kvpair;
}


/**
 * free the memory of the key-value-pair
 * It didn't release the memory of the key and value;
 * */
void hl_free_keyval_pair(hl_keyval_pair * kvpair){
    hl_free(kvpair);
}


/**
 * set the value of specificed key of a dict.
 * */
void hl_dict_set(hl_dict * dict,hl_string * key,hl_string * val){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            pair->value=val;
            return;
        }
    }
    pair=hl_init_keyval_pair_withkv(key,val);
    hl_list_append(dict,pair);
}

/**
 * set the value of specificed key of a dict,parameters are chars 
 * */
void hl_dict_set_by_chars(hl_dict * dict,char * key,char * val){
    hl_string * keystring;
    hl_string * valstring;
    keystring=hl_new_string(key);
    valstring=hl_new_string(val);
    hl_dict_set(dict,keystring,valstring);
    hl_string_free(keystring);
}
/**
 * return that if the dict has the specified key
 **/
int hl_dict_has_key (hl_dict * dict,hl_string * key){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            return 1;
        }
    }
    return 0;
}

/** 
 * return the value of specificed key of a dict 
 * if there is no item with specified key, you would get a string "" 
 * */
hl_string * hl_dict_get(hl_dict * dict,hl_string * key){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            return pair->value;
        }
    }
    return NULL;
}

/**
 * get dict data using "const char * key " as key.
 * 
 * */
hl_string * hl_dict_get_by_chars(hl_dict * dict ,const char  * key){
    hl_string * return_str;
    hl_string * key_str;
    key_str= hl_init_string();
    hl_fill_string(key_str,key);
    return_str =  hl_dict_get(dict,key_str);
    hl_string_free(key_str);
    return return_str;
}
/**
 * return that if the dict has the specified value
 **/
int hl_dict_has_val (hl_dict * dict,hl_string * val){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->value->data,val->data))
        {
            return 1;
        }
    }
    return 0;
}

void hl_dict_free(hl_dict * dict){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        hl_free_keyval_pair(pair);
    }
}


/**
 * parse the ini config file;
 * @configfilepath 为配置文件路径
 * */
int  hl_parse_config_file(hl_string *  config_file,hl_dict * dict ){

    assert(dict);

    int k=0;
    int len=0;
    char comment[2]="#";
    char buf[1024],key[1024],val[1024],temp[1024];
    FILE * fd;
    char * configfilepath=config_file->data;

    hl_dict_set_by_chars(dict,"root","");
    hl_dict_set_by_chars(dict,"port","");
    hl_dict_set_by_chars(dict,"hostname","");
    hl_dict_set_by_chars(dict,"access_log","");
    hl_dict_set_by_chars(dict,"error_log","");
    hl_dict_set_by_chars(dict,"max_clients","");
    hl_dict_set_by_chars(dict,"max_childs","");
    hl_dict_set_by_chars(dict,"run_daemon","");
    hl_dict_set_by_chars(dict,"cgi_pattern","");
    hl_dict_set_by_chars(dict,"cgi_dir","");


    fd=fopen(configfilepath,"r");
    if(!fd){
        fprintf(stderr,"Can't open the file:%s\n",configfilepath);
    }
    int rt_value=0;
    int line_no=0;
    while(fgets(buf,1024,fd)){
        line_no++;
        //ignore the comment;
        if(cbstrfwimatch(buf, comment))
            continue;
        sscanf(buf,"%1024[^=]=%s",key,val);
        len=strlen(val);
        if(!cbstricmp(key,"port")){
            hl_dict_set_by_chars(dict,"port",
                    val);
        }
        else if(!cbstricmp(key,"host")){
            hl_dict_set_by_chars(dict,"host",val);
        }
        else if(!cbstricmp(key,"root")){
            hl_dict_set_by_chars(dict,"root",val);
        }
        else if(!cbstricmp(key,"cgi_pattern")){
            hl_dict_set_by_chars(dict,"cgi_pattern",val);
        }
        else if(!cbstricmp(key,"access_log")){
            hl_dict_set_by_chars(dict,"access_log",val);
        }
        else if(!cbstricmp(key,"error_log")){
            hl_dict_set_by_chars(dict,"error_log",val);
        }
        else if(!cbstricmp(key,"max_clients")){
            hl_dict_set_by_chars(dict,"max_clients",val);
        }
        else if(!cbstricmp(key,"max_childs")){
            hl_dict_set_by_chars(dict,"max_childs",val);
        }
        else if(!cbstricmp(key,"run_daemon")){
            hl_dict_set_by_chars(dict,"run_daemon","y");
            if(!cbstricmp(val,"no")){
                hl_dict_set_by_chars(dict,"run_daemon","n");
            }
        }
        else if(!cbstricmp(key,"cgi_dir")){
            hl_dict_set_by_chars(dict,"cgi_dir",val);
        }
        else {
            rt_value=-1;
        }
    }
    fclose(fd);
    return rt_value;
}




void
hexdump(const void *buf, int len)
{
  int offset = 0;
  int line_offset;

  while (offset < len) {
    printf("%08X:", offset);
    for (line_offset = 0; line_offset < 16; line_offset++) {
      if ((line_offset % 4) == 0)
    printf(" ");
      if (offset + line_offset < len)
    printf(" %02X", ((unsigned char *) buf)[offset + line_offset]);
      else
    printf("   ");
    }
    printf("  ");
    for (line_offset = 0; line_offset < 16; line_offset++) {
      if (offset + line_offset >= len)
    break;
      printf("%c", isprint(((unsigned char *) buf)[offset + line_offset]) ?
      ((unsigned char *) buf)[offset + line_offset] : '.');
    }
    offset += 16;
    printf("\n");
  }
}



//void log_access(char * msg){  fputs(msg,access_log_fd);   fflush(access_log_fd);  }
/* Convert the letters of a string to lower case. */
char *cbstrtolower(char *str){
  int i;
  assert(str);
  for(i = 0; str[i] != '\0'; i++){
    if(str[i] >= 'A' && str[i] <= 'Z') str[i] += 'a' - 'A';
  }
  return str;
}

/* compare two strings with case insensitive evaluation. */
int cbstricmp(const char *astr, const char *bstr){
  int ac, bc;
  assert(astr && bstr);
  while(*astr != '\0'){
    if(*bstr == '\0') return 1;
    ac = (*astr >= 'A' && *astr <= 'Z') ? *astr + ('a' - 'A') : *(unsigned char *)astr;
    bc = (*bstr >= 'A' && *bstr <= 'Z') ? *bstr + ('a' - 'A') : *(unsigned char *)bstr;
    if(ac != bc) return ac - bc;
    astr++;
    bstr++;
  }
  return *bstr == '\0' ? 0 : -1;
}

/* Check whether a string begins with a key, with case insensitive evaluation. */
int cbstrfwimatch(const char *str, const char *key){
  int sc, kc;
  assert(str && key);
  while(*key != '\0'){
    if(*str == '\0') return FALSE;
    sc = *str;
    if(sc >= 'A' && sc <= 'Z') sc += 'a' - 'A';
    kc = *key;
    if(kc >= 'A' && kc <= 'Z') kc += 'a' - 'A';
    if(sc != kc) return FALSE;
    key++;
    str++;
  }
  return TRUE;
}
/* Check whether a string ends with a key. */
int cbstrbwmatch(const char *str, const char *key){
  int slen, klen, i;
  assert(str && key);
  slen = strlen(str);
  klen = strlen(key);
  for(i = 1; i <= klen; i++){
    if(i > slen || str[slen-i] != key[klen-i]) return FALSE;
  }
  return TRUE;
}

/**
 * allocate memory for a hl_thread_node;
 * */
hl_thread_node * hl_new_thread_node(){
    hl_thread_node * p= malloc(sizeof(hl_thread_node));
    p->active=0;
    return p;
}
/**
 * @param pool :
 * */
void * hl_init_pool(hl_pool * pool,size_t size){
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
void * hl_pool_alloc_large(hl_pool * pool,size_t size){
    if(pool==NULL) return ;
    hl_pool * new;
    if(pool->larges==NULL){
        pool->larges=hl_pool_alloc(pool,sizeof(hl_pool));
    }
    if(pool->larges==NULL){
        return NULL;
    }
    HL_RESET_POOL(pool->larges);
    DHERE
    new=hl_pool_alloc(pool,sizeof(hl_pool));
    DHERE
    if(new==NULL) return NULL;
    DHERE
    HL_RESET_POOL(new); 
    hl_init_pool(new,size);
    hl_pool * p;
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
void  hl_pool_reset(hl_pool * pool){
    if(pool==NULL) return ;
    hl_pool * pt;
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
void hl_pool_clear(hl_pool * pool){
    if(pool==NULL) return ;
    hl_pool * p;
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
void * hl_pool_alloc(hl_pool * pool,size_t size){
    if(pool==NULL) return NULL;
    void * p;
    hl_pool * pt;
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
    hl_pool * new;
    new=hl_pool_alloc(pool,sizeof(hl_pool));
    if(new==NULL){
        HL_ALLOC(new,sizeof(hl_pool));
    }
    if(new==NULL) return NULL;
    DHERE
    HL_RESET_POOL(new); 
    hl_init_pool(new,2 * pool->total_size);
    new->next=pool->using_pool;
    pool->using_pool=new;      
}

