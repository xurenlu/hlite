#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>

char e404[]="<h1>404 Error</h1><hr><p>File not found!</p>";
char e403[]="<h1>403 Error</h1><hr><p>Fobidden!</p>";
char e50x[]="<h1>500 Error</h1><hr><p>Internel Error!</p>";


/**
 * free memory
 * */
void hlite_free(void * ptr){
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

hlite_string *  hlite_init_string(){
    hlite_string * p;
    p=malloc(sizeof(hlite_string));
    p->len=0;
    p->data=NULL;
    return p;
}
/**
 * malloc address for the string and set value ;
 * */
int hlite_fill_string(hlite_string * p,const char * data){
    p->len=(int)strlen(data)+1;
    p->data=malloc(p->len*sizeof(char));
    bzero(p->data,p->len);
    memcpy(p->data,data,p->len);
    return p->len;
}
/**
 * generate hlite_string struct from a char point;
 */
hlite_string *  hlite_new_string(char * p){
    hlite_string * str=hlite_init_string();
    hlite_fill_string(str,p);
    return str;
}

/**
 * free the string memory
 * */
void hlite_string_free(hlite_string * str){
    hlite_free(str->data);
    hlite_free(str);
}

/**
 * generate new LIST */
hlite_list * hlite_list_new(int size){
    hlite_list * list;
    list=(hlite_list *) malloc(sizeof(hlite_list));

    list->size=size;
    list->pos=0;
    list->p=malloc(sizeof(void *)*size);
    return list;
}
/**
 * append new data to the end of the pointer
 * */
int hlite_list_append(hlite_list * p,void * data){
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
int hlite_list_is_bof(hlite_list * p){
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
int hlite_list_is_empty(hlite_list * p ){
    return hlite_list_is_bof(p);
}
/**
 * check if is the tail of the list 
 * */
int hlite_list_is_eof(hlite_list * p){
    if(p->pos==p->size)
        return 1;
    else
        return 0;
}
/**
 * under construction
 */
int hlite_list_shift(){}

/**
 * pop out the last item of the list , delete it from the list;
 * return NULL if we have reached the end;
 */
void *  hlite_list_pop(hlite_list  * list){
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
void * hlite_list_free(hlite_list * list){
    hlite_free(list);
}
/**
 * split a string to list
 * please call this function twice
 * first with third argument NULL,then you can got a number:the size of the list
 * then,call it again,and pass it with third argument a hlite_list variable with the known size;
 * @param int just_first:return when the [separator] first found.
 * */
int hlite_split(hlite_string * separator,hlite_string * str,hlite_list * list,int just_first){
    /**
     * when list == NULL,will return the size of new list
     */
    DHERE;
    hlite_string * newstr;
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
                newstr=hlite_new_string(pj);
                hlite_list_append(list,newstr);
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
            newstr=hlite_new_string(pj);
            hlite_list_append(list,newstr);
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
hlite_keyval_pair * hlite_init_keyval_pair(){

    hlite_keyval_pair * kvpair;
    kvpair=malloc(sizeof(hlite_keyval_pair));
    return kvpair;

}
/**
 * set the key and value of a key-value pair ;
 * */
int hlite_set_keyval_pair(hlite_keyval_pair * kvpair,hlite_string * key,hlite_string * val){
    kvpair->key=key;
    kvpair->value=val;
    return 1;
}
/**
 * init a key-value-pair with specificed key and value 
 * */
hlite_keyval_pair * hlite_init_keyval_pair_withkv(hlite_string * key,hlite_string * val){
    int ret=-1;
    hlite_keyval_pair * kvpair;
    kvpair=hlite_init_keyval_pair();
    if(!kvpair) return NULL;
    ret=hlite_set_keyval_pair(kvpair,key,val);
    if(ret<0)
        return NULL;
    else
        return kvpair;
}


/**
 * free the memory of the key-value-pair
 * It didn't release the memory of the key and value;
 * */
void hlite_free_keyval_pair(hlite_keyval_pair * kvpair){
    hlite_free(kvpair);
}


/**
 * set the value of specificed key of a dict.
 * */
void hlite_dict_set(hlite_dict * dict,hlite_string * key,hlite_string * val){
    hlite_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hlite_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            pair->value=val;
            return;
        }
    }
    pair=hlite_init_keyval_pair_withkv(key,val);
    hlite_list_append(dict,pair);
}

/**
 * return that if the dict has the specified key
 **/
int hlite_dict_has_key (hlite_dict * dict,hlite_string * key){
    hlite_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hlite_keyval_pair * )dict->p[i];
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
hlite_string * hlite_dict_get(hlite_dict * dict,hlite_string * key){
    hlite_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hlite_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            return pair->value;
        }
    }
    hlite_string * null_string=hlite_init_string();
    hlite_fill_string(null_string,"");
    return null_string;
    
}

/**
 * get dict data using "const char * key " as key.
 * 
 * */
hlite_string * hlite_dict_get_by_chars(hlite_dict * dict ,const char  * key){
    hlite_string * return_str;
    hlite_string * key_str;
    return_str= hlite_init_string();
    key_str= hlite_init_string();
    hlite_fill_string(key_str,key);
    return_str =  hlite_dict_get(dict,key_str);
    hlite_string_free(key_str);
    return return_str;
}
/**
 * return that if the dict has the specified value
 **/
int hlite_dict_has_val (hlite_dict * dict,hlite_string * val){
    hlite_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hlite_keyval_pair * )dict->p[i];
        if(!strcmp(pair->value->data,val->data))
        {
            return 1;
        }
    }
    return 0;
}




/**
 * parse the ini config file;
 * @configfilepath 为配置文件路径
 * */
int  hlite_parse_config_file(hlite_string *  config_file,hlite_dict * dict ){

    assert(dict);

    int k=0;
    int len=0;
    char comment[2]="#";
    char buf[1024],key[1024],val[1024],temp[1024];
    FILE * fd;
    char * configfilepath=config_file->data;

    hlite_dict_set(dict,hlite_new_string("root"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("port"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("hostname"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("access_log"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("error_log"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("max_clients"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("max_childs"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("run_daemon"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("cgi_pattern"),hlite_new_string(""));
    hlite_dict_set(dict,hlite_new_string("cgi_dir"),hlite_new_string(""));


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
            hlite_dict_set(dict,hlite_new_string("port"),
                    hlite_new_string(val));
        }
        else if(!cbstricmp(key,"host")){
            hlite_dict_set(dict,hlite_new_string("host"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"root")){
            hlite_dict_set(dict,hlite_new_string("root"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"cgi_pattern")){
            hlite_dict_set(dict,hlite_new_string("cgi_pattern"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"access_log")){
            hlite_dict_set(dict,hlite_new_string("access_log"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"error_log")){
            hlite_dict_set(dict,hlite_new_string("error_log"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"max_clients")){
            hlite_dict_set(dict,hlite_new_string("max_clients"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"max_childs")){
            hlite_dict_set(dict,hlite_new_string("max_childs"),hlite_new_string(val));
        }
        else if(!cbstricmp(key,"run_daemon")){
            hlite_dict_set(dict,hlite_new_string("run_daemon"),hlite_new_string("y"));
            if(!cbstricmp(val,"no")){
                hlite_dict_set(dict,hlite_new_string("run_daemon"),hlite_new_string("n"));
            }
        }
        else if(!cbstricmp(key,"cgi_dir")){
            hlite_dict_set(dict,hlite_new_string("cgi_dir"),hlite_new_string(val));
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

