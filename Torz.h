/**
 * @file Torz.h
 * @brief Header file for Torz proxy connection implementation
 *
 * This header file contains the necessary structures and definitions for
 * establishing SOCKS4 proxy connections. It includes definitions for proxy
 * requests and responses according to the SOCKS4 protocol specification.
 *
 * @note Required socket functions:
 *       - socket()
 *       - connect()
 *       - close()
 *       - htons()
 *       - inet_addr()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//socket library
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//setup variables for the proxy server and proxy port number
#define PROXY "127.0.0.1"
#define PROXY_PORT 9050
#define USERNAME "Torz"
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)


/**
 * @typedef int8
 *          Unsigned 8-bit integer type
 * @typedef int16
 *          Unsigned 16-bit integer type
 * @typedef int32
 *          Unsigned 32-bit integer type
 */
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

/**
 * @struct proxy_request
 * @brief Structure for SOCKS4 proxy request
 * @member vn    Version number
 * @member cd    Command code
 * @member dstport Destination port
 * @member dstip   Destination IP address
 * @member userid  User identification string
 */
struct proxy_request {
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
};

//typedef Req - alias for struct proxy_request;
typedef struct proxy_request Req;


/**
 * @struct proxy_response
 * @brief Structure for SOCKS4 proxy response
 * @member vn    Version number
 * @member cd    Command code
 * @member dstport Destination port
 * @member dstip   Destination IP address
 */
struct proxy_response{
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
};

//typedef Res - alias for struct proxy_response;
typedef struct proxy_response Res;

/**
 * * @function request
 * @brief Creates a proxy request
 * @param char* Destination address
 * @param int   Port number
 * @return Req* Pointer to the created proxy request
 */
Req *request(const char*,const int );

/**
 * @function main
 * @brief Entry point of the program
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return int  Program exit status
 */
int main(int, char**);