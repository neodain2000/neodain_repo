/*
** File: neo_osapi_net.h
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Network Module
**    
*/

#ifndef __NEO_OSAPI_NET_H__
#define __NEO_OSAPI_NET_H__

namespace neodain {
namespace neo {
namespace os {

/*
** Sockets API wrapper functions
*/

typedef enum {
   NEO_OS_SocketDomain_INVALID,
#ifdef NEO_OS_INCLUDE_NETWORK
   NEO_OS_SocketDomain_INET,
   NEO_OS_SocketDomain_INET6,
#endif
   NEO_OS_SocketDomain_MAX
} NEO_OS_SocketDomain_t;

typedef enum {
   NEO_OS_SocketType_INVALID,
#ifdef NEO_OS_INCLUDE_NETWORK
   NEO_OS_SocketType_DATAGRAM,
   NEO_OS_SocketType_STREAM,
#endif
   NEO_OS_SocketType_MAX
} NEO_OS_SocketType_t;

#ifdef NEO_OS_INCLUDE_NETWORK

/* NOTE - neo_os_config.h may optionally specify the value for NEO_OS_SOCADDR_MAX_LEN */
#include <neo_os_config.h>

#ifndef NEO_OS_SOCKADDR_MAX_LEN
#define NEO_OS_SOCKADDR_MAX_LEN     32
#endif

typedef struct {
   uint32 ActualLength;
   char AddrData[NEO_OS_SOCKADDR_MAX_LEN];
} NEO_OS_SockAddr_t;

typedef struct {
    char name [NEO_OS_MAX_API_NAME];
    uint32 creator;
} NEO_OS_socket_prop_t;


int32 NEO_OS_SocketOpen(uint32 *sock_id, NEO_OS_SocketDomain_t Domain, NEO_OS_SocketType_t Type);
int32 NEO_OS_SocketClose(uint32 sock_id);
int32 NEO_OS_SocketBind(uint32 sock_id, const NEO_OS_SockAddr_t *Addr);
int32 NEO_OS_SocketConnect(uint32 sock_id, const NEO_OS_SockAddr_t *Addr, int32 timeout);
int32 NEO_OS_SocketAccept(uint32 sock_id, uint32 *connsock_id, NEO_OS_SockAddr_t *Addr, int32 timeout);
int32 NEO_OS_SocketRecvFrom(uint32 sock_id, void *buffer, uint32 buflen, NEO_OS_SockAddr_t *RemoteAddr, int32 timeout);
int32 NEO_OS_SocketSendTo(uint32 sock_id, const void *buffer, uint32 buflen, const NEO_OS_SockAddr_t *RemoteAddr);
int32 NEO_OS_SocketGetIdByName (uint32 *sock_id, const char *sock_name);
int32 NEO_OS_SocketGetInfo (uint32 sock_id, NEO_OS_socket_prop_t *sock_prop);

int32 NEO_OS_SocketAddrInit(NEO_OS_SockAddr_t *Addr, NEO_OS_SocketDomain_t Domain);
int32 NEO_OS_SocketAddrToString(char *buffer, uint32 buflen, const NEO_OS_SockAddr_t *Addr);
int32 NEO_OS_SocketAddrFromString(NEO_OS_SockAddr_t *Addr, const char *string);
int32 NEO_OS_SocketAddrGetPort(uint16 *PortNum, const NEO_OS_SockAddr_t *Addr);
int32 NEO_OS_SocketAddrSetPort(NEO_OS_SockAddr_t *Addr, uint16 PortNum);

/*
** NEO_OS_NetworkGetID is currently [[deprecated]] as its behavior is
** unknown and not consistent across operating systems.
*/
int32 NEO_OS_NetworkGetID             (void);
int32 NEO_OS_NetworkGetHostName       (char *host_name, uint32 name_len);

#endif  /* NEO_OS_INCLUDE_NETWORK */


} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OSAPI_NET_H__

