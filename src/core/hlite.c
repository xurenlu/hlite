#include <stdlib.h>
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

#define PORT 1234
#define MAXSOCKFD 10
#define MAX_EVENTS 1024
#define MAXBUF 8192

int access_log_fd=0;
int error_log_fd=0;
int daemon_y_n=0;

hlite_dict * conf;

void clean_global_mem(){
    if(access_log_fd)
    close(access_log_fd);
    if(error_log_fd)
    close(error_log_fd);
    hlite_dict_free(conf);

}
/**
 * generate response
 * */
int handleresponse(FILE * sock,const char * f){
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
    hlite_string * root_str;
    char * query;
    DHERE
    root_str=hlite_dict_get_by_chars((hlite_dict *)conf,(const char *) "root");
    if(root_str==NULL){
        prterrmsg("root not defined!");
        hlite_abort();
    }
    DHERE	
    //printf("root_str->data:%d\n",strlen(root_str->data));
    //printf("f:%d\n",strlen(f));
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
    query=malloc(sizeof(char)*8192);
    DHERE	
    bzero(query,strlen(real));
    DHERE	
    if(strlen(path)<strlen(orig)){
        DHERE	
        memcpy(query,orig+strlen(path)+1,strlen(orig)-strlen(path)-1);
    }
    DHERE	
    //printf("request file :%s\n",real);
    log_access(real);
    DHERE	
    int stat_result;
    if( (stat_result=stat(real,&info))==-1 ){
        fprintf(sock,"HTTP/1.1 200 OK\r\nServer: litehttpd-1.0.0\r\nConnection: close\r\n\r\n<html><head><title>lighthttpd-1.0.0 default page</title></head><body>404 forbiden</body></html>");
        //fprintf(sock,"hello:%s,%d\n",real,stat_result);
        //wrterrmsg("Not found:");
        //wrterrmsg(real);
        DHERE
        hlite_free(real);
        hlite_free(orig);
        hlite_free(query);
        //hlite_string_free(root_str);
        return 0;
    }
    DHERE	
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
    DHERE	
    if(S_ISREG(info.st_mode)){
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
    DHERE	
    hlite_free(real);
    DHERE	
    hlite_free(query);
    DHERE	
    hlite_free(orig);
    DHERE	
    //hlite_string_free(root_str);
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
    DHERE
    int len;
    dir=opendir(real);
    fprintf(sock,
            "HTTP/1.1 200 OK\r\nServer:lighttpd-1.0.0\r\nConnection: keep-alive\r\nContent-type: text/html\r\n\r\n\r\n");
    fprintf(sock,"<ul>");
    DHERE
    while(TRUE){ //(diritem=readdir(dir))!=0){
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
    DHERE
        hlite_free(filename);
    DHERE
        //hacked here.to fix mem bugs;
    }
        free(dir);
}






/** callback function of epoll */
void epoll_callback (int fd){
    //printf("got msg from:child process:%d\n",getpid());
    char buffer[MAXBUF];
    int len;
    bzero(buffer, MAXBUF);
    DHERE
    if ((len = recv(fd, buffer, MAXBUF, 0)) > 0) {
        DHERE
        FILE *ClientFP = fdopen(fd, "w");
        if (ClientFP == NULL) {
            if (!daemon_y_n) {
                DHERE
                prterrmsg("fdopen()");
            } else {
                DHERE
                wrterrmsg("fdopen()");
            }
        } else {
            DHERE
            char Req[1024];
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
            handleresponse(ClientFP, Req);
            DHERE
            fclose(ClientFP);
        }
    }
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
 * */
void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }  
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
    access_log_fd=fopen(access_log,"a+");
    if(!access_log_fd){
        fprintf(stderr,"ERROR:Can't Write Access Log file,Terminating...\n");
        exit(0);
    }
    error_log_fd=fopen(error_log,"a+");
    if(!error_log_fd){
        fprintf(stderr,"ERROR:Can't Write Error Log file,Terminating...\n");
        exit(0);
    }
//@debug    close(0), close(1), close(2);
}


int main(int argc,void ** argv)
{

	struct epoll_event ev, events[MAX_EVENTS];
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
    conf=hlite_list_new(16);
    hlite_parse_config_file(configfile,conf);
    DHERE
    str_buf2    =hlite_dict_get_by_chars(conf,"run_daemon");
    DHERE
    access_log  =hlite_dict_get_by_chars(conf,"access_log");
    DHERE
    error_log   =hlite_dict_get_by_chars(conf,"error_log");
    DHERE
        if(str_buf2==NULL){
            DHERE
            printf("Oh,str_buf2 is null\n");
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
    DHERE
    if(bind(sockfd,(const struct sockaddr *)&addr,sizeof(addr))<0){
        perror("connect");
        exit(1);
    }
    if(listen(sockfd,3)<0){
        perror("listen");
        exit(1);
    }

    DHERE 
    int  conn_sock, nfds, epollfd,n;
    epollfd = epoll_create(10);
    if (epollfd == -1) {
        perror("epoll_create");
        exit(3);
    }

    DHERE 
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        wrterrmsg("epoll_ctl: epoll_ctl ADD failed.\n");
        exit(3);
    }
    DHERE 
    for (;;) {
        DHERE
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            prterrmsg("epoll_pwait");
            exit(3);
        }


        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == sockfd) {
                conn_sock = accept(sockfd,
                        (struct sockaddr *) &local, &addr_len);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(3);
                }
                setnonblocking(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                            &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(3);
                }
            } else {
                epoll_callback(events[n].data.fd);
                DHERE
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = events[n].data.fd;
                epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
                close(events[n].data.fd); 
            }
        }
    }


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
                        perror("accept");
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
}

