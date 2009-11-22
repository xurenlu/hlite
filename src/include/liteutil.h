#include "stdio.h"
#define SMALL 16
#define MIDDLE 256
#define BUFSIZE 8192 //8K
#define BIG 65536
#define HUGE 4194304 //65536*64;

#define DHERE fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);
//#define DHERE2 fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);
//#define DHERE ;
#define FALSE 0
#define TRUE 1
#define VERSION "0.1.1"

#define prterrmsg(msg) {fprintf(stderr,msg);}
#define wrterrmsg(msg) {write(error_log_fd,msg,strlen(msg));}
#define log_access(msg){write(access_log_fd,msg,strlen(msg));}
#define prtinfomsg(msg) {fputs(msg, stdout); }
#define wrtinfomsg(msg) {write(error_log_fd,msg,strlen(msg));}

#define HL_NEW_QUEUE(x) \
    x=malloc(sizeof(hl_queue));\
    x->tail=NULL; \
    x->head=NULL;
#define HL_NEW_QUEUE_ITEM(x) \
        x=malloc(sizeof(hl_queue_item));
#define HL_INIT_QUEUE_ITEM(x,init) \
        x=malloc(sizeof(hl_queue_item)); \
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


int cbstricmp(const char *astr, const char *bstr);
int cbstrfwimatch(const char *str, const char *key);
int cbstrbwmatch(const char *str, const char *key);


typedef struct {
        char **ptr;
        size_t size;
        size_t used;
} char_array;

 struct hl_string_t {
    size_t len;
    char * data;
} ;
typedef struct hl_string_t  hl_string;
 struct  hl_string_array_t {
    int size;
    int used;
    hl_string *p;
} ;
typedef struct hl_string_array_t hl_string_array;
struct hl_int_array_t {
    int size;
    int used;
    int ** p;
};
typedef struct hl_int_array_t hl_int_array;
struct hl_keyval_pair_t  {
    hl_string * key;
    hl_string * value;
} ;
typedef struct hl_keyval_pair_t  hl_keyval_pair;

struct hl_list_t {
    int size;
    int pos;    //当前位置
    void * * p;
};
typedef struct hl_list_t hl_list;
typedef struct hl_list_t hl_dict;

struct response_arg_t {
    int fd;
    char * fpath;
    int thread_id;
    hl_string *root;
    int thread_no;
} ;
typedef struct response_arg_t response_arg;

struct hl_thread_node_t {
    pthread_t  thread;
    int active;
    int create_res;
};
typedef  struct hl_thread_node_t hl_thread_node;

struct hl_queue_item_t {
    int fd;
    struct hl_queue_item_t * next;
    struct hl_queue_item_t * prev;
};
typedef struct hl_queue_item_t hl_queue_item;

struct hl_queue_t {
    hl_queue_item * head;
    hl_queue_item * tail;
};
typedef struct hl_queue_t hl_queue;


struct hl_request_t {
    int sock_fd;
};
typedef struct hl_request_t hl_request;

struct hl_response_t {
    int sock_fd;
};
typedef struct hl_response_t hl_response;


struct hl_pool_t {
    size_t total_size;
    char * head;
    char * last;
    char * tail;
    size_t current_size;
    struct hl_pool_t * chain;
    struct hl_pool_t  * larges;
    struct hl_pool_t * next;
    struct hl_pool_t * using_pool;
    int error_times;
};
typedef struct hl_pool_t hl_pool;
