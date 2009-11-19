#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <hlite.h>
#include <liteutil.h>
#include <getopt.h>
#include <dirent.h>
#include <signal.h>
#include <semaphore.h>

#define TDG printf("%s,line %d,thread:%d\n",__FILE__,__LINE__,self_id);
#define MAXSOCKFD 10
#define MAX_EVENTS 4096
#define MAXBUF 8192
#define MAX_THREADS 64
#define LOCK pthread_mutex_lock(&mutex);
#define UNLOCK pthread_mutex_unlock(&mutex);


#define HERR_FGETFD 31
#define HERR_FSETFD 32


int access_log_fd=0;
int error_log_fd=0;
int daemon_y_n=0;
int epollfd;
int current_thread;
int gg=0;
int global_sock;
struct epoll_event ev, events[MAX_EVENTS];
hlite_dict * conf;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_main = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond;
pthread_cond_t  cond_main;
int _thread_status[MAX_THREADS];
pthread_t _threads[MAX_THREADS];
sem_t sem[MAX_THREADS];
int seed=0;
int gen_rand(){
    if(seed==0)
        seed=(int)time(0);

    srand(seed);
    printf("%d\n",seed);
    seed=rand();
    return seed;
}


void clean_global_mem(){
    if(access_log_fd)
        close(access_log_fd);
    if(error_log_fd)
        close(error_log_fd);
    hlite_dict_free(conf);

}
void sig(int signal){
    kill(getpid(),SIGSEGV);
}
void removeevt(FILE * sock,int thread_id){
    //fprintf(stderr,"response:%d\n",__LINE__);
    DHERE
        int fd;
    fd=fileno(sock);
    ev.events = EPOLLIN | EPOLLET;
    DHERE
        ev.data.fd = fd;
    DHERE
    fclose(sock);
    close(fd);
    epoll_ctl(epollfd, EPOLL_CTL_DEL,fd, &ev);
        DHERE
        //_threads[thread_id].active=0;
        //pthread_exit((void *)2);
        //fprintf(stderr,"response:%d\n",__LINE__);
        //fprintf(stderr,"fd removed:%d\n",fd);
}
/**
 * generate response
 * */
void * handleresponse(void * resp){
    int inner_sockfd;
    int self_id=*((int *)resp);
    int locked=0;
    char msg[1024]="HTTP/1.1 200 OK\r\nSERVER:litehttpd-1.0.0\r\nConnection: keep-alive\r\nContent-type:text/html;\r\n\r\ndo you hlite?";
    printf("thread created\n");
    while(1){ 
        //fprintf(stderr,"\t\t\tto lock here,id:%d\n",self_id);
        //locked=pthread_mutex_lock(&mutex);
        //printf("locked result:%d\n",pthread_mutex_lock(&mutex));
        //fprintf(stderr,"\t\t\tlocked here,lock result:%d,id:%d,waiting for &cond\n",locked,self_id);
        /**
        locked=pthread_mutex_lock(&mutex);
        printf("thread %d locked:%d\n",self_id,locked);

        pthread_cond_wait(&cond,&mutex);
        pthread_mutex_unlock(&mutex);
        */
        printf("thread %d will block till the message\n",self_id);
        sem_wait(&sem[self_id]);
        printf("thread %d got the signal\n",self_id);
        inner_sockfd=global_sock; 
    
        //UNLOCK
        //pthread_mutex_unlock(&mutex);
        //pthread_cond_signal(&cond_main);
        read_data(inner_sockfd);        
        write(inner_sockfd,msg,sizeof(msg));
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = inner_sockfd;
        close(inner_sockfd);
        epoll_ctl(epollfd, EPOLL_CTL_DEL,inner_sockfd, &ev);
        printf("thread:%d finished,sockfd:%d,try to next loop\n\n\n",self_id,inner_sockfd);
        continue;
        FILE * sock;
        char * f;
        int fd;
        int thread_id;
        thread_id=((response_arg *)resp)->thread_id;
        fd=((response_arg * )resp)->fd;
        sock=fdopen(fd,"w");
        f=((response_arg *)resp)->fpath;
        hlite_string * root_str;
        root_str = ((response_arg *)resp)->root;
        //fprintf(stderr,"response:%d\n",__LINE__);
        DHERE	
            if(daemon_y_n){
                char * temp;
                temp=(char *)   malloc(sizeof(char) * (strlen((const char * )f)+2));
                bzero((void * )temp,(size_t)strlen((const char *)f)+2);
                sprintf(temp,"%s\n",f);
                log_access(temp);
                hlite_free(temp);
            }
        DHERE	
            struct stat info;
        char * real;
        int len;
        char * query;
        DHERE
            if(root_str==NULL){
                prterrmsg("root not defined!");
                hlite_abort();
            }
        DHERE	
            //printf("root_str->data:%d\n",strlen(root_str->data));
            //printf("f:%d\n",strlen(f));
            //fprintf(stderr,"response:%d\n",__LINE__);
            //fprintf(stderr,"root_str:%d\n",strlen(root_str->data));
            len=strlen(root_str->data)+strlen(f);
        DHERE	
            real=malloc(len+1);
        bzero(real,len+1);
        DHERE	
            if(cbstricmp(f,"HTTP/1.1")==0){
                sprintf(real,"%s/",root_str->data);
            }
            else
            {
                sprintf(real,"%s%s",root_str->data,f);
            }
        DHERE	
            cburldecode(real,strlen(real));
        char * orig=malloc(sizeof(char)*strlen(real)+1);
        DHERE	
            bzero(orig,strlen(real)+1);
        strcpy(orig,real);
        DHERE	
            //目前路径还包含有?号,应该把问号前的部分取出来;
            char * delim="?";
        DHERE	
            char * path =strtok(real,delim);
        DHERE	
            query=malloc(sizeof(char)*1024);
        DHERE	
            bzero(query,strlen(real));
        DHERE	
            //fprintf(stderr,"response:%d\n",__LINE__);
            if(strlen(path)<strlen(orig)){
                DHERE	
                    memcpy(query,orig+strlen(path)+1,strlen(orig)-strlen(path)-1);
            }
        DHERE	
            //printf("request file :%s\n",real);
            log_access(real);
        DHERE	
            int stat_result;
        //fprintf(stderr,"response:%d\n",__LINE__);
        if( (stat_result=stat(real,&info))==-1 ){
            fprintf(sock,"HTTP/1.1 200 OK\r\nServer: litehttpd-1.0.0\r\nConnection: close\r\n\r\n<html><head><title>lighthttpd-1.0.0 default page</title></head><body>404 forbiden</body></html>");
            //fprintf(sock,"hello:%s,%d\n",real,stat_result);
            //wrterrmsg("Not found:");
            //wrterrmsg(real);
            DHERE
                hlite_free(real);
            hlite_free(orig);
            hlite_free(query);
            removeevt(sock,thread_id);
            hlite_free(f);
            hlite_free(resp);
            return ;
            //hlite_string_free(root_str);
        }
            /**
             * 优先看是否需要用CGI处理
             */
            /**
              hlite_string * cgi_dir;
              cgi_dir=hlite_dict_get_by_chars(conf,"cgi_dir");
              if(cbstrfwimatch(real,cgi_dir->data)){
              printf("try to handle cgi:%s\n",real);
              handlecgi(sock,real,f);
              }
              hlite_string_free(cgi_dir);
              */
            //fprintf(stderr,"response:%d\n",__LINE__);
        else if(S_ISREG(info.st_mode)){
                DHERE	
                    if(cbstrbwmatch(real,".cgi")){
                        //printf("try to handle cgi:%s\n",real);
                        DHERE	
                            int pid;
                        switch(pid=fork()){
                            case -1:
                                DHERE	
                                    if (!daemon_y_n) {
                                        DHERE	
                                            prterrmsg("fork error while handle cgi process;");
                                    } else {
                                        DHERE	
                                            wrterrmsg("fork error while handle cgi process;");
                                    }
                                break;
                            case 0:
                                // child process

                                //handlecgi(sock,real,orig,query);
                                DHERE	
                                    break;
                            default :
                                DHERE	
                                    /**
                                      hlite_free(real);
                                      DHERE	
                                      hlite_free(query);
                                      DHERE	
                                      hlite_free(orig);
                                      DHERE	
                                      hlite_string_free(root_str);
                                      return 0;
                                      */
                        }
                    }

                    else{
                        handlestaticfile(sock,real,orig);
                    }
            }
            else if (S_ISDIR(info.st_mode)){
                DHERE	
                    handlestaticdir(sock,real,orig);
            }
        //fprintf(stderr,"response:%d\n",__LINE__);
        DHERE	
            hlite_free(real);
        DHERE	
            hlite_free(query);
        DHERE	
            hlite_free(orig);
        DHERE	
            removeevt(sock,thread_id);
        hlite_free(f);
        hlite_free(resp);
    }
    //return ;
}


int handlestaticfile(FILE * sock,char * real,char * f){
    int len,ret,sockfd;
    sockfd=fileno(sock);
    len=ret=0;
    char * p=NULL;
    int fd=0;
    errno=0;
    fd=open(real,O_RDONLY);
    if(!fd){
        fprintf(stderr,"file reading error:%s\n",real);
        hlite_abort();
    }
    len=lseek(fd,0,SEEK_END);
    p=(char *)malloc(len+1);
    bzero(p,len+1);
    lseek(fd,0,SEEK_SET);
    ret=read(fd,p,len);
    close(fd);


    /**
     *  handle the content-type:
     *  */
    char * buf="text/html";

    if(cbstrbwmatch(real,".gif")){
        buf="image/gif";
    }
    if(cbstrbwmatch(real,".png")){buf="image/png";}
    if(cbstrbwmatch(real,".bmp")){buf="image/bmp";}
    if(cbstrbwmatch(real,".jpg")){buf="image/jpg";}

    if(cbstrbwmatch(real,".html")){buf="text/html";}
    if(cbstrbwmatch(real,".htm")){buf="text/html";}
    if(cbstrbwmatch(real,".php")){buf="text/html";}
    if(cbstrbwmatch(real,".txt")){buf="text/html";}

    if(cbstrbwmatch(real,".css")){buf="text/css";}
    if(cbstrbwmatch(real,".js")){buf="text/javascript";}

    fputs("HTTP/1.1 200 OK\r\n",sock);
    fputs("SERVER:litehttpd-1.0.0\r\n",sock);
    fputs("Connection: keep-alive\r\n",sock);
    fputs("Content-type: ",sock);
    fputs(buf,sock);
    fputs("\r\n",sock);
    //  fputs("Content-Length:",sock);
    //  fputs(len,sock);
    fputs("\r\n",sock);
    //fputs(p,sock);
    write(sockfd,p,ret);
    //sizeof(p));
    hlite_free(p);
}


int  handlestaticdir(FILE * sock,char * real,char * f){
    DIR * dir ;
    struct dirent *diritem;
    struct stat info;
    char * filename;
    int len;
    //fprintf(stderr,"response:%d\n",__LINE__);
    //dir=opendir(real);
    fprintf(sock, "HTTP/1.1 200 OK\r\nServer:lighttpd-1.0.0\r\nConnection: keep-alive\r\nContent-type: text/html\r\n\r\n\r\n<ul>");
    fprintf(sock,"testing");
    return 0;
    while(TRUE){ 
        diritem=readdir(dir);
        if(diritem==NULL) break;
        filename=(char * )malloc(MAXBUF+1);
        bzero(filename,MAXBUF+1);
        if(cbstrbwmatch(real,"/")){
            sprintf(filename,"%s%s",real,diritem->d_name);
        }else{
            sprintf(filename,"%s/%s",real,diritem->d_name);
        }
        stat(filename,&info);
        if(S_ISDIR(info.st_mode)){
            fprintf(sock,"<li><a href='%s/'>%s/</a></li>",diritem->d_name,diritem->d_name);
        }else{
            fprintf(sock,"<li><a href='%s'>%s</a></li>",diritem->d_name,diritem->d_name);
        }
        //fprintf(stderr,"response:%d\n",__LINE__);
        DHERE
            hlite_free(filename);
        DHERE
            //hacked here.to fix mem bugs;
    }
    //fprintf(stderr,"response:%d\n",__LINE__);
    closedir(dir);
    DHERE
}






/** callback function of epoll */
void read_data(int fd){
    //printf("got msg from:child process:%d\n",getpid());
    char buffer[MAXBUF];
    gg++;
    int len;
    int joined;
    hlite_thread_node   pthread_node;
    int previous=0;
    response_arg * resp;
    bzero(buffer, MAXBUF);
    pthread_attr_t attr;
    /**
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    */
    //fprintf(stderr,"got fd:%d\n",fd);
        while((len = recv(fd, buffer, MAXBUF, 0)) > 0) {
            printf("receive data:%d bytes\n",len);
        }
        return ;
    DHERE
        if ((len = recv(fd, buffer, MAXBUF, 0)) > 0) {
            DHERE
            {
                DHERE
                    char * Req;
                Req=malloc(sizeof(char)*1024);
                if(cbstrfwimatch(buffer,"GET")){
                    DHERE
                        //printf("GET command GET");
                        DHERE
                        sscanf(buffer, "GET /%s HTTP", Req);
                }
                else if (cbstrfwimatch(buffer,"POST")) {
                    DHERE
                        //printf("GET command POST");
                        DHERE
                        sscanf(buffer, "POST /%s HTTP", Req);
                }
                else {
                    DHERE
                        //printf("GET command UNKOWN");
                        // to do.
                }
                DHERE
                    bzero(buffer, MAXBUF);
                DHERE
                    resp=malloc(sizeof(response_arg  ));
                resp->fd=fd;
                resp->fpath=Req;
                hlite_string * root_str;
                root_str=hlite_dict_get_by_chars((hlite_dict *)conf,(const char *) "root");
                resp->root=root_str;//some thing wrong
                DHERE
                    previous=current_thread+1;
                    if(previous==MAX_THREADS){
                        previous=0;
                    }

                fprintf(stderr,"%d,",gg);
                if(_thread_status[previous]==1){
                    joined=pthread_join(_threads[previous],NULL);
                    if(joined!=0){
                        fprintf(stderr,"\njoin result:%d\n",joined);
                    }
                    DHERE
                        _thread_status[previous]=0;
                }
                DHERE
                    resp->thread_id=current_thread;
                DHERE
                int iret1 = pthread_create(&_threads[current_thread], &attr, handleresponse, (void *) resp);
                DHERE
                    /**
                      joined=pthread_join(_threads[current_thread],NULL);
                      */
                    if(iret1!=0){
                        fprintf(stderr,"not zero returned\n");
                        exit(16);
                    }
                _thread_status[current_thread]=1;
                current_thread++;
                if(current_thread==MAX_THREADS){
                    current_thread=0;
                }
                //fprintf(stderr,"curent thread points to:%d\n",current_thread);
                //handleresponse(ClientFP, Req);
                DHERE
            }
        }
    pthread_attr_destroy(&attr);
}


/**
 * @param str 需要解码的url字符串
 * @param len 需要解码的url的长度
 * @return int 返回解码后的url长度
 */
int cburldecode(char *str, int len)
{
    char *dest = str;
    char *data = str;

    int value;
    int c;

    while (len--) {
        if (*data == '+') {
            *dest = ' ';
        }
        else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1))
                && isxdigit((int) *(data + 2)))
        {

            c = ((unsigned char *)(data+1))[0];
            if (isupper(c))
                c = tolower(c);
            value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
            c = ((unsigned char *)(data+1))[1];
            if (isupper(c))
                c = tolower(c);
            value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

            *dest = (char)value ;
            data += 2;
            len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    return dest - str;
}














/**
 * 设置为非阴塞模式
 * setting to non-blocking mode
 * return 0 normally
 * return -1 if can't get F_GETFL
 * return -2 if can't set NONBLOCK;
 * */
int setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        prterrmsg("fcntl(sock,GETFL)");
        return HERR_FGETFD ;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        prterrmsg("fcntl(sock,SETFL,opts)");
        return HERR_FSETFD ;
    }  
    return 0;
}


/**
 * usage :
 * */
void usage() {
    printf("    Hlite :a simple httpd;\n\n");
    printf("\nUsage: conf -f file.conf\n");
    printf("Author renlu.xu<helloasp@hotmail.com>\n");
    printf("               <xurenlu@gmail.com>\n");
    printf("Version:");printf("%s\n",VERSION);printf("\n");
}

/**
 * just exit.
 * */
void hlite_abort(){
    exit(0);
}

/** Enter the Daemon model */
int daemonize(char * access_log,char * error_log){
    /**
      return 0;
      if (fork())
      exit(0);
      if (fork())
      exit(0);
      */
    access_log_fd=open(access_log,O_CREAT|O_RDWR);
    if(!access_log_fd){
        fprintf(stderr,"ERROR:Can't Write Access Log file,Terminating...\n");
        exit(0);
    }
    error_log_fd=open(error_log,O_CREAT|O_RDWR);
    if(!error_log_fd){
        fprintf(stderr,"ERROR:Can't Write Error Log file,Terminating...\n");
        exit(0);
    }
    //@debug    close(0), close(1), close(2);
}


int main(int argc,void ** argv)
{
    signal(SIGINT,sig);
    int k;
    int semv;
    for(k=0;k<MAX_THREADS;k++){
        _thread_status[k]=0;
    }
    DHERE
        current_thread=0;
    int sockfd,newsockfd,is_connected[MAXSOCKFD],fd;
    struct sockaddr_in addr;
    struct sockaddr local;
    int addr_len = sizeof(struct sockaddr_in);
    fd_set readfds;
    char buffer[256];
    char msg[] ="Welcome to server!";
    hlite_string * configfile=NULL;
    hlite_string * str_buf1;
    hlite_string * str_buf2;
    hlite_string * access_log;
    hlite_string * error_log;
    DHERE

        int c;
    while((c = getopt (argc,(char * const *) argv, "hf:")) != -1)
        switch(c){
            case 'h':
                usage();
                hlite_abort();
                break;
            case 'f':
                configfile= hlite_new_string(optarg);
                break;
            case '?':
                if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                usage();
                hlite_abort();
        }
    if(configfile==NULL){
        usage();
        hlite_abort();
    }
    DHERE
        conf=hlite_new_list(16);
    hlite_parse_config_file(configfile,conf);
    DHERE
        str_buf2    =hlite_dict_get_by_chars(conf,"run_daemon");
    DHERE
        access_log  =hlite_dict_get_by_chars(conf,"access_log");
    DHERE
        error_log   =hlite_dict_get_by_chars(conf,"error_log");
    DHERE
    if(str_buf2==NULL){
                prterrmsg("Oh,str_buf2 is null\n");
    }
    if(( str_buf2!=NULL) && (!strcmp(str_buf2->data,"y"))){
        DHERE
        daemonize(access_log->data,error_log->data);
    }
    DHERE
        if ((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
            DHERE
                wrterrmsg("create socket failed:");
            hlite_abort();
        }
    DHERE
        bzero(&addr,sizeof(addr));
    addr.sin_family =AF_INET;
    int port;
    DHERE
        hlite_string * port_str;
    DHERE
        port_str=hlite_dict_get_by_chars(conf,"port");
    DHERE
        port=atoi(port_str->data);
    DHERE
        hlite_string_free(port_str);
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //下面这句我还没搞懂是怎么回事
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(struct sockaddr *)&addr,sizeof(addr));

    DHERE
        if(bind(sockfd,(const struct sockaddr *)&addr,sizeof(addr))<0){
            prterrmsg("connect");
            exit(1);
        }
    if(listen(sockfd,3)<0){
        prterrmsg("listen");
        exit(1);
    }

    DHERE 
        int  conn_sock, nfds, n;
    epollfd = epoll_create(10);
    if (epollfd == -1) {
        prterrmsg("epoll_create");
        exit(3);
    }

    DHERE 
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        wrterrmsg("epoll_ctl: epoll_ctl ADD failed.\n");
        exit(3);
    }

    /*
     * 在这个创建一堆线程
     */
    pthread_cond_init(&cond,NULL);
    pthread_cond_init(&cond_main,NULL);
    int l=0;
    int rnd=0;
    int threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];
    for(;l<MAX_THREADS;l++){
        sem_init(&sem[l],0,0);
        thread_ids[l]=l;
        rnd=gen_rand()%100;
        printf("gen rand:%d\n",rnd);
        usleep(gen_rand()%100);
        pthread_create(&threads[l],NULL,handleresponse,&thread_ids[l]);
    }

    while(1){
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            fprintf(stderr,"errno:%d\n",errno);
            prterrmsg("epoll_pwait");
            continue;
            //exit(15);
        }


        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == sockfd) {
                conn_sock = accept(sockfd,(struct sockaddr *) &local, &addr_len);
                printf("\n\n\naccepted new socked:%d\n",conn_sock);
                if (conn_sock == -1) {
                    prterrmsg("accept"); 
                    exit(3);
                }
                else if(setnonblocking(conn_sock)!=0){
                    close(conn_sock);
                    continue;
                }
                else{
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = conn_sock;
                    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
                        prterrmsg("epoll_ctl: conn_sock");
                        exit(3);
                    }
                }
            } else {
                int t;
                /**
                int t=events[n].data.fd;
                char msg[1024]="HTTP/1.1 200 OK\r\nSERVER:litehttpd-1.0.0\r\nConnection: keep-alive\r\nContent-type:text/html;\r\n\r\ndo you hlite?";
                write(t,msg,sizeof(msg));
                close(t);
                */
                t=gen_rand() % MAX_THREADS;
                printf("signal new fd:%d \n",events[n].data.fd);
                //LOCK
                global_sock=events[n].data.fd;
                sem_post(&sem[t]);
                //pthread_cond_signal(&cond);
                //UNLOCK
                //pthread_mutex_unlock(&mutex);
        //pthread_mutex_lock(&mutex_main);
        //pthread_cond_wait(&cond_main,&mutex_main);
        //pthread_mutex_unlock(&mutex_main);
                /**
                  DHERE
                  ev.events = EPOLLIN | EPOLLET;
                  ev.data.fd = events[n].data.fd;
                  epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
                  close(events[n].data.fd); 
                  */
            }
        }
    }
    pthread_join(&thread_ids[0],NULL);
    
    /**
     * {{{
      for(fd=0;fd<MAXSOCKFD;fd++)
      is_connected[fd]=0;
      while(1){
      FD_ZERO(&readfds);
      FD_SET(sockfd,&readfds);
      for(fd=0;fd<MAXSOCKFD;fd++)
      if(is_connected[fd]) FD_SET(fd,&readfds);
      if(!select(MAXSOCKFD,&readfds,NULL,NULL,NULL))continue;
      for(fd=0;fd<MAXSOCKFD;fd++)
      if(FD_ISSET(fd,&readfds)){
      if(sockfd ==fd ){
      if((newsockfd = accept (sockfd,(struct sockaddr *) &addr,(socklen_t *) &addr_len))<0)
      prterrmsg("accept");
      write(newsockfd,msg,sizeof(msg));
      is_connected[newsockfd] =1;
      printf("cnnect from %s\n",inet_ntoa(addr.sin_addr));
      }else{
      bzero(buffer,sizeof(buffer));
      if(read(fd,buffer,sizeof(buffer))<=0){
      printf("connect closed\n");
      is_connected[fd]=0;
      close(fd);
      }else
      printf("%s",buffer);
      }
      }
      }
      //}}}
      */
}

