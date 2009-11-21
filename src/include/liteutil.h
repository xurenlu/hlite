#include "stdio.h"
#define SMALL 16
#define MIDDLE 256
#define BUFSIZE 8192 //8K
#define BIG 65536
#define HUGE 4194304 //65536*64;

//#define DHERE fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);
//#define DHERE2 fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);
#define DHERE ;
#define FALSE 0
#define TRUE 1
#define VERSION "0.1.1"

#define prterrmsg(msg) {fprintf(stderr,msg);}
#define wrterrmsg(msg) {write(error_log_fd,msg,strlen(msg));}
#define log_access(msg){write(access_log_fd,msg,strlen(msg));}
#define prtinfomsg(msg) {fputs(msg, stdout); }
#define wrtinfomsg(msg) {write(error_log_fd,msg,strlen(msg));}

int cbstricmp(const char *astr, const char *bstr);
int cbstrfwimatch(const char *str, const char *key);
int cbstrbwmatch(const char *str, const char *key);


typedef struct {
        char **ptr;
        size_t size;
        size_t used;
} char_array;

 struct hlite_string_t {
    size_t len;
    char * data;
} ;
typedef struct hlite_string_t  hlite_string;
 struct  hlite_string_array_t {
    int size;
    int used;
    hlite_string *p;
} ;
typedef struct hlite_string_array_t hlite_string_array;
struct hlite_int_array_t {
    int size;
    int used;
    int ** p;
};
typedef struct hlite_int_array_t hlite_int_array;
struct hlite_keyval_pair_t  {
    hlite_string * key;
    hlite_string * value;
} ;
typedef struct hlite_keyval_pair_t  hlite_keyval_pair;

struct hlite_list_t {
    int size;
    int pos;    //当前位置
    void * * p;
};
typedef struct hlite_list_t hlite_list;
typedef struct hlite_list_t hlite_dict;

struct response_arg_t {
    int fd;
    char * fpath;
    int thread_id;
    hlite_string *root;
    int thread_no;
} ;
typedef struct response_arg_t response_arg;

struct hlite_thread_node_t {
    pthread_t  thread;
    int active;
    int create_res;
};
typedef  struct hlite_thread_node_t hlite_thread_node;

struct hlite_queue_item_t {
    void * data;
    struct hlite_queue_item_t * next;
    struct hlite_queue_item_t * prev;
};
typedef struct hlite_queue_item_t hlite_queue_item;
struct hlite_queue_t {
    hlite_queue_item * head;
    hlite_queue_item * tail;
};
typedef struct hlite_queue_t hlite_queue;


struct hlite_request_t {
    int sock_fd;
};
typedef struct hlite_request_t hlite_request;

struct hlite_response_t {
    int sock_fd;
};
typedef struct hlite_response_t hlite_response;


struct hlite_pool_t {
    size_t total_size;
    char * head;
    char * last;
    char * tail;
    size_t current_size;
    struct hlite_pool_t * chain;
    struct hlite_pool_t  * larges;
    struct hlite_pool_t * next;
    struct hlite_pool_t * using_pool;
    int error_times;
};
typedef struct hlite_pool_t hlite_pool;
