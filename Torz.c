/**
 * @file Torz.c
 * @brief Implementation of SOCKS4 protocol client functionality
 */
#include "Torz.h"


/**
 * @brief Creates a SOCKS4 request structure with the specified destination IP and port
 * @param dstip The destination IP address as a string
 * @param dstport The destination port number
 * @return Pointer to the newly allocated request structure
 * 
 * This function allocates memory for a new SOCKS4 request structure and initializes
 * it with the specified destination IP address and port. It sets the protocol
 * version to 4 and command code to 1 (CONNECT). The user ID is copied from the
 * USERNAME macro.
 */
Req *request(const char *dstip,const int dstport){
    Req *req = malloc(reqsize);

    req->vn = 4;
    req->cd = 1;
    req->dstport = htons(dstport);
    req->dstip = inet_addr(dstip);
    strncpy(req->userid, USERNAME, 8);

    return req;
}

/**
 * @brief Main function - entry point of the SOCKS4 client program
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Program exit status
 *
 * Variables:
 * @var host Stores the target host name
 * @var port Stores the target port number
 * @var s Socket file descriptor
 * @var sock Socket address structure for connection
 * @var req Pointer to SOCKS4 request structure
 * @var res Pointer to SOCKS4 response structure
 * @var buff Buffer for storing response data
 * @var success Status flag for operations
 * @var tmp Temporary buffer for string operations
 */
int main(int argc, char *argv[])
{
    char *host; //host name
    int port, s; //port number
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buff[ressize];
    int success;  //predicate - property that is either true or false
    char tmp[512];


    /**
     * If the user does not provide the host and port number, print an error message
     * example command = ./Torz 1.2.3.4 8080
     */
    if (argc < 3){
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }


    //assign host and port number
    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        perror("socket");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    sock.sin_addr.s_addr = inet_addr(PROXY);


    if (connect(s , (struct sockaddr *)&sock, sizeof(sock))){
        perror("connect");
        return -1;
    } 

    printf("Connected to proxy\n");

    req = request(host, port); //req  variable is a pointer to the request structure
    write(s, req, reqsize); //write the request to the socket
        
    memset(buff, 0, ressize);
    if(read(s, buff, ressize) < 1){
        perror("read");
        free(req);
        close(s);
        return -1;
    }
    
    res = buff;
    success = (res->cd == 90);
    if(!success){
        fprintf(stderr, "Unable to traverse the proxy %d\n", res->cd);
        close(s);
        free(req);
        return -1;
    }

    printf("Successfully Traversing the proxy EZZZZZ\n", "%s:%d\n", host, port);
    memset(tmp, 0, 512);
    snprintf(tmp, 511, 
        "HEAD / HTTP/1.0\r\n"
        "Host: %s\r\n"
        "\r\n");
    write(s, tmp, strlen(tmp));

    memset(tmp, 0, 512);
    read(s, tmp, 511);
    printf("'%s'\n", tmp);

    close(s);
    free(req);
    return 0;
}