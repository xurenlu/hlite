#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <liteutil.h>
#include <getopt.h>

#define PORT 1234
#define MAXSOCKFD 10
#define MAX_EVENTS 1024


int access_log_fd=0;
int error_log_fd=0;

void do_use_fd (int fd){
    char buffer[128];
    int len;
    len=read(fd,buffer,128);
    printf("readed:%d bytes\n",len);
}

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
void usage(){
    printf("    Hlite :a simple httpd;\n\n");
    printf("\nUsage: conf -f file.conf\n");
    printf("Author renlu.xu<helloasp@hotmail.com>\n");
    printf("               <xurenlu@gmail.com>\n");
    printf("Version:");printf("%s\n",VERSION);printf("\n");
}
void hlite_abort(){
    exit(0);
}

/** Enter the Daemon model */
int daemonize(char * access_log,char * error_log){
    if (fork())
        exit(0);
    if (fork())
        exit(0);
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
    hlite_dict * conf;

    int c;
    while((c = getopt (argc, argv, "hf:")) != -1)
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

    str_buf1=hlite_new_string("run_daemon");
    DHERE
    str_buf2=hlite_dict_get(conf,str_buf1);
    DHERE
    hlite_string_free(str_buf1);
    DHERE
    str_buf1=hlite_new_string("access_log");
    DHERE
    access_log=hlite_dict_get(conf,str_buf1);
    DHERE
    hlite_string_free(str_buf1);
    DHERE
    str_buf1=hlite_new_string("error_log");
    DHERE
    error_log=hlite_dict_get(conf,str_buf1);
    DHERE
    hlite_string_free(str_buf1);

    if(!strcmp(str_buf2->data,"y")){
        daemonize(access_log->data,error_log->data);
    }
    
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket");
        exit(1);
    }
    bzero(&addr,sizeof(addr));
    addr.sin_family =AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd,&addr,sizeof(addr))<0){
        perror("connect");
        exit(1);
    }
    if(listen(sockfd,3)<0){
        perror("listen");
        exit(1);
    }




   
   int  conn_sock, nfds, epollfd,n;


    epollfd = epoll_create(10);
    if (epollfd == -1) {
        perror("epoll_create");
        exit(3);
    }

    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl: sockfd");
        exit(3);
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_pwait");
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
                do_use_fd(events[n].data.fd);

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
                    if((newsockfd = accept (sockfd,&addr,&addr_len))<0)
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

