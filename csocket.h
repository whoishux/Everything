#ifndef CSOCKET_H
#define CSOCKET_H

#include <string>
#include <vector>

#include "cthread.h"

using namespace std;

#if 1
typedef enum {
    SD_AF_UNSPEC = 0,
    // ����ͨ��;
    SD_AF_UNIX = 1,
    // IPv4 InternetЭ��;
    SD_AF_INET = 2,
    // IPv6 InternetЭ��;
    SD_AF_INET6 = 23,
}CSockDomain;

typedef enum {
    // Tcp���ӣ��ṩ���л��ġ��ɿ��ġ�˫�����ӵ��ֽ�����֧�ִ������ݴ���;
    ST_SOCK_STREAM = 1,
    // ֧��UDP���ӣ�������״̬����Ϣ��;
    ST_SOCK_DGRAM = 2,
    // RAW���ͣ��ṩԭʼ����Э�����;
    ST_SOCK_RAW = 3,
    // �ṩ�ɿ������ݱ��ģ������������ݻ�������;
    ST_SOCK_RDM = 4,
    // ���л������ṩһ�����л��ġ��ɿ��ġ�˫��Ļ������ӵ����ݴ���ͨ�������ݳ��ȶ�����ÿ�ε��ö�ϵͳ����ʱ������Ҫ��ȫ�����ݶ���;
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
    // �����׽ӿ�;
    SOL_SOL_SOCKET = 0xffff,
    // TCP�׽ӿ�;
    SOL_IPPROTO_TCP = 6,
    // IPv4�׽ӿ�;
//    IPPROTO_IP = 0,
    // IPv6�׽ӿ�;
//    IPPROTO_IPV6 = 41,
}CSockOptLevel;

typedef enum {
    // level = SOL_SOCKET
    // ������� int;
    SON_SO_DEBUG = 0x0001,
    // �������ñ��ص�ַ�Ͷ˿� int;
    SON_SO_REUSERADDR = 0x0004,
    // ��ռģʽʹ�ö˿�,���ǲ��������������ʹ��SO_REUSEADDR�����ʹ��ĳһ�˿�;
    SON_SO_EXCLUSIVEADDRUSE = ((int)(~SON_SO_REUSERADDR)),
    // �������� int;
    SON_SO_KEEPALIVE = 0x0008,
    // ������·�� int;
    SON_SO_DONTROUTE = 0x0010,
    // �����͹㲥���� int;
    SON_SO_BROADCAST = 0x0020,
    // �ӳٹر����� struct linger;
    SON_SO_LINGER = 0x0080,
    // ��Ϊ�棬��SO_LINGERѡ���ֹ;
    SON_SO_DONTLINGER = (int)(~SON_SO_LINGER),
    // �������ݷ�������������,����ͨ�������н��մ������� int;
    SON_SO_OOBINLINE = 0x0100,
    // ���ͻ�������С int;
    SON_SO_SNDBUF = 0x1001,
    // ���ջ�������С int
    SON_SO_RCVBUF = 0x1002,
    // ���ͻ��������� int;
    SON_SO_SNDLOWAT = 0x1003,
    // ���ջ��������� int;
    SON_SO_RCVLOWAT = 0x1004,
    // ���ͳ�ʱ struct timeval;
    SON_SO_SNDTIMEO = 0x1005,
    // ���ճ�ʱ struct timeval;
    SON_SO_RCVTIMEO = 0x1006,
    // ����׽��ִ��� int;
    SON_SO_ERROR = 0x1007,
    // ����׽������� int;
    SON_SO_TYPE = 0x1008,
/*
    // IPPROTO_IP;
    // IP�ײ�ѡ�� int;
    IP_OPTIONS = 1,
    // �����ݰ��а���IP�ײ� int;
    IP_HDRINCL = 2,
    // ��������;
    IP_TOS = 3,
    // ����ʱ�� int;
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

    // ����socket;
    bool Socket(CSockDomain domain, CSockType type, CSockProtocol protocol);

    // ��socket�����ص�ַ�Ͷ˿ڣ�ͨ���ɷ���˵���;
    bool Bind(string addr, unsigned short port);

    // ��������TCP���͵ķ�����������ã�ʵ�ּ�������;
    bool Listen(int backlog = 20);

    // ����TCP���͵ķ�����������ã��������ȴ��ͻ������ӣ�һ��������̬;
    unsigned long long Accept();

    // ͨ����TCP���Ϳͻ��˵��ã����������������һ��TCP���ӣ�ʵ���Ƿ���3�����ֹ���;
    bool Connect(string addr, unsigned short port);

    // TCP���͵����ݷ���;
    int Send(unsigned long long sockfd, const void *buf, int len, CSockFlag flags = SF_DEFAULT);

    // TCP���͵����ݽ���;
    int Recv(unsigned long long sockfd, void *buf, int len, CSockFlag flags = SF_DEFAULT);

    // ���ڷǿɿ�����(UDP)�����ݷ��ͣ���ΪUDP��ʽδ��������socket�������Ҫ�ƶ�Ŀ��Э���ַ;
    int Sendto(unsigned long long sockfd, const void *buf, int len, string dstaddr, unsigned short dstport);

    // ���ڷǿɿ�����(UDP)�����ݽ���;
    int Recvfrom(unsigned long long sockfd, void *buf, int len, string &srcaddr, unsigned short &srcport);

    /* ����ȷ��һ���׽ӿڵ�״̬����ѯ���Ŀɶ��ԡ���д�Լ�����״̬��Ϣ;
       ms = -1�����ǽ�select��������״̬��һ���ȵ������ļ�������������ĳ���ļ������������仯Ϊֹ;
       ms = 0���ͱ��һ������ķ����������������ļ��������Ƿ��б仯�������̷��ؼ���ִ��;
       ms > 0������ǵȴ��ĳ�ʱʱ�䣬��select��msʱ������������ʱʱ��֮�����¼������ͷ����ˣ������ڳ�ʱ�󲻹�����һ������;
    */
    bool Select(unsigned long long sockfd, int type, unsigned int ms = -1);

    // �ر��׽���;
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

    // �ر�;
    bool Close();

protected:
    unsigned long long m_fd;
    unsigned short m_family;
    //���������߳�
    CThread *m_sendHeartThread;
    //���������߳�
	CThread *m_recvHeartThread;
};

#endif // CSOCKET_H
