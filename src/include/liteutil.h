#define SMALL 16
#define MIDDLE 256
#define BUFSIZE 8192 //8K
#define BIG 65536
#define HUGE 4194304 //65536*64;
//#define DHERE printf("debug:%s,%d\n",__FILE__,__LINE__);
#define DHERE fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);
#define FALSE 0
#define TRUE 1

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



