#ifndef CSOCKET_H
#define CSOCKET_H

#include <string>
#include <vector>

#include "cthread.h"

using namespace std;

#if 1
typedef enum {
    SD_AF_UNSPEC = 0,
    // 本地通信;
    SD_AF_UNIX = 1,
    // IPv4 Internet协议;
    SD_AF_INET = 2,
    // IPv6 Internet协议;
    SD_AF_INET6 = 23,
}CSockDomain;

typedef enum {
    // Tcp连接，提供序列化的、可靠的、双向连接的字节流。支持带外数据传输;
    ST_SOCK_STREAM = 1,
    // 支持UDP连接（无连接状态的消息）;
    ST_SOCK_DGRAM = 2,
    // RAW类型，提供原始网络协议访问;
    ST_SOCK_RAW = 3,
    // 提供可靠的数据报文，不过可能数据会有乱序;
    ST_SOCK_RDM = 4,
    // 序列化包，提供一个序列化的、可靠的、双向的基本连接的数据传输通道，数据长度定常。每次调用读系统调用时数据需要将全部数据读出;
    ST_SOCK_SEQPACKET = 5,
}CSockType;

typedef enum {
    // Dummy protocol for TCP;
    SP_IPPROTO_IP = 0,
    // Internet Control Message Protocol;
    SP_IPPROTO_ICMP = 1,
    // Transmission Control Protocol;
    SP_IPPROTO_TCP = 6,
    // User Datagram Protocol;
    SP_IPPROTO_UDP = 17,
}CSockProtocol;

typedef enum {
    SF_DEFAULT = 0x0,
    // Sends OOB data (stream-style socket such as SOCK_STREAM only.
    SF_MSG_OOB = 0x1,
    // Specifies that the data should not be subject to routing. A Windows Sockets service provider can choose to ignore this flag.
    SF_MSG_DONTROUTE = 0x4,
}CSockFlag;

typedef enum {
    // Shutdown receive operations.
    SST_SD_RECEIVE = 0,
    // Shutdown send operations.
    SST_SD_SEND = 1,
    // Shutdown both send and receive operations.
    SST_SD_BOTH = 2,
}CSockShutdownType;

#define SockSeleteRead      0x1
#define SockSeleteWrite     0x2
#define SockSeleteExcept    0x4

typedef enum {
    // 基本套接口;
    SOL_SOL_SOCKET = 0xffff,
    // TCP套接口;
    SOL_IPPROTO_TCP = 6,
    // IPv4套接口;
//    IPPROTO_IP = 0,
    // IPv6套接口;
//    IPPROTO_IPV6 = 41,
}CSockOptLevel;

typedef enum {
    // level = SOL_SOCKET
    // 允许调试 int;
    SON_SO_DEBUG = 0x0001,
    // 允许重用本地地址和端口 int;
    SON_SO_REUSERADDR = 0x0004,
    // 独占模式使用端口,就是不充许和其它程序使用SO_REUSEADDR共享的使用某一端口;
    SON_SO_EXCLUSIVEADDRUSE = ((int)(~SON_SO_REUSERADDR)),
    // 保持连接 int;
    SON_SO_KEEPALIVE = 0x0008,
    // 不查找路由 int;
    SON_SO_DONTROUTE = 0x0010,
    // 允许发送广播数据 int;
    SON_SO_BROADCAST = 0x0020,
    // 延迟关闭连接 struct linger;
    SON_SO_LINGER = 0x0080,
    // 若为真，则SO_LINGER选项被禁止;
    SON_SO_DONTLINGER = (int)(~SON_SO_LINGER),
    // 带外数据放入正常数据流,在普通数据流中接收带外数据 int;
    SON_SO_OOBINLINE = 0x0100,
    // 发送缓冲区大小 int;
    SON_SO_SNDBUF = 0x1001,
    // 接收缓冲区大小 int
    SON_SO_RCVBUF = 0x1002,
    // 发送缓冲区下限 int;
    SON_SO_SNDLOWAT = 0x1003,
    // 接收缓冲区下限 int;
    SON_SO_RCVLOWAT = 0x1004,
    // 发送超时 struct timeval;
    SON_SO_SNDTIMEO = 0x1005,
    // 接收超时 struct timeval;
    SON_SO_RCVTIMEO = 0x1006,
    // 获得套接字错误 int;
    SON_SO_ERROR = 0x1007,
    // 获得套接字类型 int;
    SON_SO_TYPE = 0x1008,
/*
    // IPPROTO_IP;
    // IP首部选项 int;
    IP_OPTIONS = 1,
    // 在数据包中包含IP首部 int;
    IP_HDRINCL = 2,
    // 服务类型;
    IP_TOS = 3,
    // 生存时间 int;
    IP_TTL = 4,
*/
    // level = IPPROTO_TCP;

}CSockOptName;

#ifndef _WINSOCK2API_
#ifndef IOCPARM_MASK
#define IOCPARM_MASK 0x7f
#endif
#ifndef IOC_VOID
#define IOC_VOID 0x20000000
#endif
#ifndef IOC_OUT
#define IOC_OUT 0x40000000
#endif
#ifndef IOC_IN
#define IOC_IN 0x80000000
#endif
#ifndef IOC_INOUT
#define IOC_INOUT (IOC_IN|IOC_OUT)
#endif

#ifndef _IO
#define _IO(x,y) (IOC_VOID|((x)<<8)|(y))
#endif
#ifndef _IOR
#define _IOR(x,y,t) (IOC_OUT|(((__LONG32)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#endif
#ifndef _IOW
#define _IOW(x,y,t) (IOC_IN|(((__LONG32)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#endif
#ifndef FIONREAD
#define FIONREAD _IOR('f',127,unsigned long)
#endif
#ifndef FIONBIO
#define FIONBIO _IOW('f',126,unsigned long)
#endif
#ifndef FIOASYNC
#define FIOASYNC _IOW('f',125,unsigned long)
#endif
#endif

typedef enum {
    // The Internet Protocol version 4 (IPv4) address family.
    SHT_AF_INET = 2,
    // The Internet Protocol version 6 (IPv6) address family.
    SHT_AF_INET6 = 23,
}CSockHostAddrType;

typedef struct HostAddrInfo_t{
    string hName;
    CSockHostAddrType hType;
    vector<string> hAddrList;
}HostAddrInfo_t;

#endif

class CSocket
{
public:
    CSocket();
    ~CSocket();

    // 创建socket;
    bool Socket(CSockDomain domain, CSockType type, CSockProtocol protocol);

    // 绑定socket到本地地址和端口，通常由服务端调用;
    bool Bind(string addr, unsigned short port);

    // 函数仅被TCP类型的服务器程序调用，实现监听服务;
    bool Listen(int backlog = 20);

    // 仅被TCP类型的服务器程序调用，服务器等待客户端连接，一般是阻塞态;
    unsigned long long Accept();

    // 通常由TCP类型客户端调用，用来与服务器建立一个TCP连接，实际是发起3次握手过程;
    bool Connect(string addr, unsigned short port);

    // TCP类型的数据发送;
    int Send(unsigned long long sockfd, const void *buf, int len, CSockFlag flags = SF_DEFAULT);

    // TCP类型的数据接收;
    int Recv(unsigned long long sockfd, void *buf, int len, CSockFlag flags = SF_DEFAULT);

    // 用于非可靠连接(UDP)的数据发送，因为UDP方式未建立连接socket，因此需要制定目的协议地址;
    int Sendto(unsigned long long sockfd, const void *buf, int len, string dstaddr, unsigned short dstport);

    // 用于非可靠连接(UDP)的数据接收;
    int Recvfrom(unsigned long long sockfd, void *buf, int len, string &srcaddr, unsigned short &srcport);

    /* 用于确定一个套接口的状态，查询它的可读性、可写性及错误状态信息;
       ms = -1，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止;
       ms = 0，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行;
       ms > 0，这就是等待的超时时间，即select在ms时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管怎样一定返回;
    */
    bool Select(unsigned long long sockfd, int type, unsigned int ms = -1);

    // 关闭套接字;
    bool Shutdown(int sockfd, CSockShutdownType type);
    bool CloseSocket(unsigned long long fd);

    static bool GetHostByAddr(CSockHostAddrType type, string addr, string &hostName);
    static bool GetHostByName(string name, HostAddrInfo_t &hostInfo);
    static bool GetHostName(string &name);
    static bool GetPeerName(unsigned long long fd, string &addr, unsigned short &port);
    static bool GetSockName(unsigned long long fd, string &addr, unsigned short &port);

    int SetSockOpt(unsigned long long sockfd, CSockOptLevel level, CSockOptName optname, const char *optval, int optlen);
    int GetSockOpt(unsigned long long sockfd, CSockOptLevel level, CSockOptName optname, void *optval, int *optlen);
    bool IoctlSocket(unsigned long long fd, long cmd, unsigned long * argp);
    int GetLastError();

    void RunSendHeart(void (*func)(void*), void *p=nullptr);
    void StopSendHeart(void (*func)(void*), void *p=nullptr);
    void RunRecvHeart(void (*func)(void*), void *p=nullptr);
    void StopRecvHeart(void (*func)(void*), void *p=nullptr);

    // 关闭;
    bool Close();

protected:
    unsigned long long m_fd;
    unsigned short m_family;
    //发送心跳线程
    CThread *m_sendHeartThread;
    //接收心跳线程
	CThread *m_recvHeartThread;
};

#endif // CSOCKET_H
