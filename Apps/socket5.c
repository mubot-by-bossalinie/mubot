#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket5.h"

#define MAX_USER 10
#define BUFF_SIZE (1024 * 2)
#define AUTH_CODE 0x00

#define TIME_OUT (6000000 * 1)
#define USER_NAME "test"
#define PASS_WORD "test"

int SelectMethod(int socket_fd)
{
    char recv_buffer[BUFF_SIZE] = {0};
    char reply_buffer[2] = {0};
    METHOD_SELECT_REQUEST * method_request = NULL;
    METHOD_SELECT_RESPONSE * method_response = NULL;

    int ret = recv(socket_fd, recv_buffer, BUFF_SIZE, 0);
    if(ret <= 0) {
       // perror("[E]: recv error.");
        close(socket_fd);
        return -1;
    }
    //printf("[I]: SelectMethod, recv %d bytes.\n", ret);
    method_request = (METHOD_SELECT_REQUEST *)recv_buffer;
    method_response = (METHOD_SELECT_RESPONSE *)reply_buffer;

    method_response->version = VERSION;

    // if not socket5
    //printf("[I]: request version %d.\n", method_request->version);
    if((int)method_request->version != VERSION)
    {
        method_response->select_method = 0xff;
        send(socket_fd, method_response, sizeof(METHOD_SELECT_RESPONSE), 0);
        close(socket_fd);
        return -1;
    }
    method_response->select_method = AUTH_CODE;
    if(-1 == send(socket_fd, method_response, sizeof(METHOD_SELECT_RESPONSE), 0))
    {
        close(socket_fd);
        return -1;
    }
    return 0;
}

int AuthPassword(int socket_fd)
{
    char recv_buffer[BUFF_SIZE] = {0};
    char reply_buffer[BUFF_SIZE] = {0};

    AUTH_REQUEST * auth_request = NULL;
    AUTH_RESPONSE * auth_response = NULL;

    int ret = recv(socket_fd, recv_buffer, BUFF_SIZE, 0);
    if(ret <= 0)
    {
     //   perror("[E]: recv username and password error.\n");
        close(socket_fd);
        return -1;
    }
    //printf("[I]: AuthPassword, recv %d bytes.\n", ret);

    auth_request = (AUTH_REQUEST *)recv_buffer;
    memset(reply_buffer, 0, BUFF_SIZE);
    auth_response = (AUTH_RESPONSE *)reply_buffer;
    auth_response->version = 0x01;

    char recv_name[256] = {0};
    char recv_pass[256] = {0};

    //auth_request->name_len is a char, max number is 0xff
    char pwd_str[2] = {0};
    strncpy(pwd_str, auth_request->name + auth_request->name_len, 1);
    int pwd_len = (int)pwd_str[0];

    strncpy(recv_name, auth_request->name, auth_request->name_len);
    strncpy(recv_pass, auth_request->name + auth_request->name_len + sizeof(auth_request->pwd_len),
            pwd_len);

    //printf("[I]: username, %s; password, %s.\n", recv_name, recv_pass);
    if((strncmp(recv_name, USER_NAME, strlen(USER_NAME)) == 0)
            && (strncmp(recv_pass, PASS_WORD, strlen(PASS_WORD)) == 0))
    {
        auth_response->result = 0x00;
        if(-1 == send(socket_fd, auth_response, sizeof(AUTH_RESPONSE), 0))
        {
            close(socket_fd);
            return -1;
        }
        else{
            return 0;
        }
    }
    else
    {
        auth_response->result = 0x01;
        send(socket_fd, auth_response, sizeof(AUTH_RESPONSE), 0);
        close(socket_fd);
        return -1;
    }

}

// parse command, and try to connect real server.
// return socket for success, -1 for failed.
int ParseCommand(int socket_fd)
{
    char recv_buffer[BUFF_SIZE] = {0};
    char reply_buffer[BUFF_SIZE] = {0};

    SOCKET5_REQUEST * socket5_request = NULL;
    SOCKET5_RESPONSE * socket5_response = NULL;

    // recv command
    int ret = recv(socket_fd, recv_buffer, BUFF_SIZE, 0);
    if(ret <= 0)
    {
       // perror("[E]: recv connect command error.");
        close(socket_fd);
        return -1;
    }
    socket5_request = (SOCKET5_REQUEST *)recv_buffer;
    if((socket5_request->version != VERSION) || (socket5_request->cmd != CONNECT) ||
            (socket5_request->address_type == IPV6))
    {
        //perror("[E]: connect command error.");
        close(socket_fd);
        return -1;
    }
    // begain process connect request
    struct sockaddr_in sin;
    memset((void *)&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    // get real server ip address
    if(socket5_request->address_type == IPV4)
    {
        memcpy(&sin.sin_addr.s_addr, &socket5_request->address_type + sizeof(socket5_request->address_type), 4);
        memcpy(&sin.sin_port, &socket5_request->address_type + sizeof(socket5_request->address_type) + 4, 2);
       // printf("[I]: real server, %s %d\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
    }
    else if(socket5_request->address_type == DOMAIN)
    {
        char domain_len = *(&socket5_request->address_type + sizeof(socket5_request->address_type));
        char target_domain[256] = {0};

        strncpy(target_domain, &socket5_request->address_type + 2, (unsigned int)domain_len);
       // printf("[I]: target_domain %s\n", target_domain);
        struct hostent * phost = gethostbyname(target_domain);
        if(phost == NULL)
        {
            close(socket_fd);
            return -1;
        }
        memcpy(&sin.sin_addr, phost->h_addr_list[0], phost->h_length);
        memcpy(&sin.sin_port, &socket5_request->address_type + sizeof(socket5_request->address_type) +
                sizeof(domain_len) + domain_len, 2);
    }
    // try to connect to real server
    int real_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(real_server_socket_fd < 0)
    {
        //perror("[E]: real socket create failed.\n");
        close(socket_fd);
        return -1;
    }
    memset(reply_buffer, 0, sizeof(BUFF_SIZE));
    socket5_response = (SOCKET5_RESPONSE *)reply_buffer;
    socket5_response->version = VERSION;
    socket5_response->reserved = 0x00;
    socket5_response->address_type = 0x01;
    memset(socket5_response + 4, 0, 6);

    ret = connect(real_server_socket_fd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
    if(ret == 0)
    {
        socket5_response->reply = 0x00;
        if(-1 == send(socket_fd, socket5_response, 10, 0))
        {
            close(socket_fd);
            return -1;
        }
    }
    else{
        //perror("[E]: connect to real server error.");
        socket5_response->reply = 0x01;
        send(socket_fd, socket5_response, 10, 0);
        close(socket_fd);
        return -1;
    }
    return real_server_socket_fd;
}


int max(int x, int y)
{
    if(x > y)
        return x;
    return y;
}

void ForwardData(int socket_fd, int real_server_socket_fd)
{
    char recv_buffer[BUFF_SIZE] = {0};
    fd_set fd_read;
    struct timeval time_out;
    time_out.tv_sec = 0;
    time_out.tv_usec = TIME_OUT;
    int ret = 0;
    while(1)
    {
        FD_ZERO(&fd_read);
        FD_SET(socket_fd, &fd_read);
        FD_SET(real_server_socket_fd, &fd_read);
        ret = select(max(socket_fd, real_server_socket_fd) + 1, &fd_read, 0, 0, &time_out);
        if(ret < 0)
            return;
        else if(ret == 0)
            break;
        if(FD_ISSET(socket_fd, &fd_read))
        {
            memset(recv_buffer, 0, BUFF_SIZE);
            ret = recv(socket_fd, recv_buffer, BUFF_SIZE, 0);
            if(ret <= 0)
                return;
            send(real_server_socket_fd, recv_buffer, ret, 0);
        }
        if(FD_ISSET(real_server_socket_fd, &fd_read))
        {
            memset(recv_buffer, 0, BUFF_SIZE);
            ret = recv(real_server_socket_fd, recv_buffer, BUFF_SIZE, 0);
            if(ret <= 0)
                return;
            send(socket_fd, recv_buffer, ret, 0);
        }
    }
}

int Socks5(void * client_sock_fd)
{
    int socket_fd = *(int*)client_sock_fd;
    if(-1 == SelectMethod(socket_fd))
    {
       // printf("[E]: SelectMethod error.\n");
        return -1;
    }
    if(AUTH_CODE == 0x02 && -1 == AuthPassword(socket_fd))
    {
       // printf("[E]: AuthPassword error.\n");
        return  -1;
    }
    int real_server_socket_fd = ParseCommand(socket_fd);
    if(-1 == real_server_socket_fd)
    {
       // printf("[E]: Get real server socket fd error.\n");
        return -1;
    }
    ForwardData(socket_fd, real_server_socket_fd);
    close(socket_fd);
    close(real_server_socket_fd);
    return 0;
}

int socks5srv(char *s) {
	char i1[128]= "", i2[128]= "", i3[128]= "";

	char *text = s;

	sscanf(text, "%s %s %s", &i1, &i2, &i3);
	if (mfork() != 0) return;
    struct sockaddr_in sin;
    memset((void *)&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(i1));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    int listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_socket_fd < 0)
    {
        //perror("[E]: socket create failed.\n");
        return -1;
    }

    int opt = SO_REUSEADDR;
    setsockopt(listen_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(listen_socket_fd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0)
    {
       // perror("[E]: bind error.\n");
        return -1;
    }
    if(listen(listen_socket_fd, MAX_USER) < 0)
    {
      //  perror("[E]: listen error.\n");
        return -1;
    }
    struct sockaddr_in cin;
    int client_sock_fd;
    int client_len = sizeof(struct sockaddr_in);
    while(client_sock_fd = accept(listen_socket_fd, (struct sockaddr *)&cin, (socklen_t *)&client_len))
    {
       // printf("[I]: connected from %s, process...\n", inet_ntoa(cin.sin_addr));
        pthread_t work_thread;
        if(pthread_create(&work_thread, NULL, (void *)Socks5, (void *)&client_sock_fd))
        {
        //    perror("[E]: create thread error...");
            close(client_sock_fd);
        }
        else
        {
            pthread_detach(work_thread);
        }
    }

    return 0;
}

