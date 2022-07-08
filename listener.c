#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT "8888"
#define BACKLOG 10

int main(void){
    
    int sockfd, new_sockfd;
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage their_addr;
    int yes = 1;
    socklen_t sin_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    int status;
    if((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0){
        
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }


    for(p = res; p != NULL; p = p->ai_next){
    
        if((sockfd = socket(p->ai_family, 
                        p->ai_socktype, 
                        p->ai_protocol)) == -1){
            
            perror("socket");
            continue; 
        }
        
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))){
            
            perror("setsockopt");
            exit(1);
        }

        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            
            close(sockfd);
            perror("connect");
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if(p == NULL){
        
        fprintf(stderr, "failed to bind socket");
        return 2;
    }
    
    if(listen(sockfd, BACKLOG) == -1){

        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    
    sin_size = sizeof(their_addr);
    new_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

    if(new_sockfd == -1){
        
        perror("accept");
        return 3;
    } 
    
    if(!fork()){

        close(sockfd);

        char buffer[1024];
        int buffsize;
        
        while(1){ 
            
            if((buffsize = read(0, buffer, 1024)) <= 0){
                
                perror("send");
                break;
            }
            if(write(new_sockfd, buffer, buffsize)<=0){
                
                perror("write");
                break;
            }
        }
        
        close(new_sockfd);
        
        exit(0);
    }

    close(sockfd);

    char buffer[1024];
    int buffsize;
    while(1){ 
        
        if((buffsize = read(new_sockfd, buffer, 1024)) <= 0){
            
            perror("recv");
            break;
        }
        write(1, buffer, buffsize);
    }


    close(new_sockfd);
    
    return 0;
}
