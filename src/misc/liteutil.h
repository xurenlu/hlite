#define SMALL 16
#define MIDDLE 256
#define BUFSIZE 8192 //8K
#define BIG 65536
#define HUGE 4194304 //65536*64;
//#define DHERE printf("debug:%s,%d\n",__FILE__,__LINE__);
#define DHERE fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);

typedef struct {
        char **ptr;
        size_t size;
        size_t used;
} char_array;

 struct lite_string_t {
    size_t len;
    char * data;
} ;
typedef struct lite_string_t  lite_string;
 struct  lite_string_array_t {
    int size;
    int used;
    lite_string *p;
} ;
typedef struct lite_string_array_t lite_string_array;
 struct lite_int_array_t {
    int size;
    int used;
    int ** p;
};
typedef struct lite_int_array_t lite_int_array;
 struct lite_keyval_pair_t  {
    lite_string key;
    lite_string value;
} ;
typedef struct lite_keyval_pair_t  lite_keyval_pair;

 struct lite_list_t {
    int size;
    int pos;    //当前位置
    void * * p;
};
typedef struct lite_list_t lite_list;



