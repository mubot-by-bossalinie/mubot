#ifndef SOCKET5_H
#define SOCKET5_H

#define VERSION 0x05
#define CONNECT 0x01
#define IPV4    0x01
#define IPV6    0x04
#define DOMAIN  0x03

typedef struct _method_select_response // 协商方法服务器响应
{
    char version;       // 服务器支持的 SOCKET 版本，0x04 或者 0x05
    char select_method; // 服务器选择的方法， 0x00 为匿名，0x02 认证
} METHOD_SELECT_RESPONSE;

typedef struct _method_select_request // 协商方法客户端请求
{
    char version;           // 客户端支持的版本，0x04 或者 0x05
    char number_methods;    // 客户端支持的方法的数量
    char methods[255];      // 客户端支持的方法类型，最多 255 个，0x00 为匿名，0x02 认证
} METHOD_SELECT_REQUEST;

typedef struct _auth_response
{
    char version;   // 版本，此处恒定为 0x01
    char result;    // 服务端认证结果，0x00 为成功，其他均为失败
} AUTH_RESPONSE;

typedef struct _auth_request
{
    char version;   // 版本，此处 0x01
    char name_len;  //
    char name[255]; // 用户名
    char pwd_len;   //
    char pwd[255];  // 密码
} AUTH_REQUEST;

typedef  struct _socket_response // 代理服务器响应
{
    char version;
    char reply;             // 代理服务器连接真实主机的结果， 0x00 表示成功
    char reserved;          // 保留位，恒定位 0x00
    char address_type;      // socket 代理服务器绑定的地址类型：ipv4 0x01 ipv6 0x04 domain 0x03
    char address_port[1];   // 如果 address_type 为域名，此处第一字节为域名长度，其后为域名本身，无0字符结尾,域名后为Socks代理服务器绑定端口 节
} SOCKET5_RESPONSE;

typedef  struct _scoket_request // 客户端连接主机
{
    char version;
    char cmd;
    char reserved;
    char address_type;
    char address_port[1];
} SOCKET5_REQUEST;
#endif
