#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>
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
    free(list);
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

void hlite_free_keyval_pair(hlite_keyval_pair * kvpair){

}

int maink(int argc,char ** argv){
    /**
    hlite_string * p;
    p=hlite_init_string();
    hlite_fill_string(p,"hello");
    printf("string len:%d,string:%s\n",p->len,p->data);
    printf("\n\n");
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
    *     */
    /**
    int list_len,k;
    DHERE;

    hlite_string * test_str;
    hlite_string * test_sep;
    hlite_list * segments;

    test_str=hlite_new_string("http://www.handle.com/?");
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
    */
    

   return 0;
}



/**
 * parse the ini config file;
 * @configfilepath 为配置文件路径
 * */
int hlite_parseconfigfile(char * configfilepath){
    int k=0;
    int len=0;
    char comment[2]="#";
    FILE * fd;
    fd=fopen(configfilepath,"r");
    if(!fd){
        fprintf(stderr,"Can't open the file:%s\n",configfilepath);
    }
    char  buf[1024];
    char key[1024],val[1024];
    char root[1024],host[1024],access_log[1024],error_log[1024],cgi_dir[1024];
    int daemon_y_n,childs,clients,port;
    while(fgets(buf,1024,fd)){
        if(cbstrfwimatch(buf, comment))
            continue;
        sscanf(buf,"%1024[^=]=%s",key,val);

        len=strlen(val);
        if(!cbstricmp(key,"port")){
            port=atoi(val);
        }
        if(!cbstricmp(key,"host")){
            //host=malloc(len+1);
            bzero(host,len+1);
            memcpy(host,val,len);
        }
        if(!cbstricmp(key,"root")){
            //root=malloc(len+1);
            bzero(root,len+1);
            memcpy(root,val,len);
        }
        if(!cbstricmp(key,"daemon")){
            if(!cbstricmp(val,"yes")){
                daemon_y_n=1;
            }
            if(!cbstricmp(val,"no")){
                daemon_y_n=0;
            }
        }
        if(!cbstricmp(key,"access_log")){
            //access_log=malloc(len+1);
            bzero(access_log,len+1);
            memcpy(access_log,val,len);
        }
        if(!cbstricmp(key,"error_log")){
            //error_log=malloc(len+1);
            bzero(error_log,len+1);
            memcpy(error_log,val,len);
        }
        if(!cbstricmp(key,"clients")){
            clients=atoi(val);
        }
        if(!cbstricmp(key,"childs")){
            childs=atoi(val);
        }
        if(!cbstricmp(key,"cgi_dir")){
            //cgi_dir=malloc(len+1);
            bzero(cgi_dir,len+1);
            memcpy(cgi_dir,val,len);
        }
    }
    fclose(fd);
    return 0;
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

