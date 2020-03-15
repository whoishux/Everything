
#include <stdio.h>
#include <ws2tcpip.h>

#include "csocket.h"
#include "cthread.h"

#include "ccommon.h"
#include "ccounter.h"


static CCounter GSocketCnt = 0;

bool Hostent2HostAddrInfo_t(struct hostent *in, HostAddrInfo_t &out)
{
    struct in_addr addr;

    if(in == nullptr) return false;

    out.hType = (CSockHostAddrType)(in->h_addrtype);
    out.hName = in->h_name;
    int i = 0;
    while (in->h_addr_list[i] != nullptr) {
        addr.s_addr = *(u_long *) in->h_addr_list[i++];
        out.hAddrList.push_back(string(inet_ntoa(addr)));
    }

    return true;
}

CSocket::CSocket() : m_fd(-1)
{
    if(GSocketCnt == 0){
        //初始化 DLL
        WSADATA wsaData;
        WSAStartup( MAKEWORD(2, 2), &wsaData);
    }
    GSocketCnt++;
    m_sendHeartThread = MNew(CThreadFunc);
    m_recvHeartThread = MNew(CThreadFunc);
}

CSocket::~CSocket()
{
    GSocketCnt--;
    if(GSocketCnt == 0){
        //终止 DLL 的使用
        WSACleanup();
    }
    CThreadFuncP(m_sendHeartThread)->Join();
    CThreadFuncP(m_recvHeartThread)->Join();

    MDelete(m_sendHeartThread);
    MDelete(m_recvHeartThread);

}

bool CSocket::Socket(CSockDomain domain, CSockType type, CSockProtocol protocol)
{
    m_family = domain;
    m_fd = socket(domain, type, protocol);
    if(m_fd == INVALID_SOCKET) return false;
    return true;
}

bool CSocket::Bind(string addr, unsigned short port)
{
    // 绑定套接字;
    bool ret = false;
    sockaddr_in sockAddr;

    memset(&sockAddr, 0, sizeof(sockaddr_in));
    sockAddr.sin_family = m_family;
    sockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    sockAddr.sin_port = htons(port);
    ret = bind(m_fd, (sockaddr*)&sockAddr, sizeof(struct sockaddr)) == 0;

    return ret;
}

bool CSocket::Listen(int backlog)
{
    return listen(m_fd, backlog) == 0;
}

unsigned long long CSocket::Accept()
{
    // 接收客户端请求;
    sockaddr clntAddr;
    int nSize = sizeof(sockaddr);
    SOCKET clntSock = accept(m_fd, (sockaddr*)&clntAddr, &nSize);

    return clntSock;
}

bool CSocket::Connect(string addr, unsigned short port)
{
    // 向服务器发起请求;
    bool ret = false;
    sockaddr_in sockAddr;

    memset(&sockAddr, 0, sizeof(sockAddr));  // 每个字节都用0填充;
    sockAddr.sin_family = m_family;
    sockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    sockAddr.sin_port = htons(port);
    ret = connect(m_fd, (sockaddr*)&sockAddr, sizeof(sockaddr)) == 0;

    return ret;
}

int CSocket::Send(unsigned long long sockfd, const void *buf, int len, CSockFlag flags)
{
    return send(sockfd, (const char *)buf, len, flags);
}

int CSocket::Recv(unsigned long long sockfd, void *buf, int len, CSockFlag flags)
{
    return recv(sockfd, (char *)buf, len, flags);
}

int CSocket::Sendto(unsigned long long sockfd, const void *buf, int len, string dstaddr, unsigned short dstport)
{
    sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = m_family;
    sin.sin_port = htons(dstport);
    sin.sin_addr.S_un.S_addr = inet_addr(dstaddr.c_str());

    return sendto(sockfd, (const char *)buf, len, 0, (sockaddr *)&sin, sizeof(sockaddr));
}

int CSocket::Recvfrom(unsigned long long sockfd, void *buf, int len, string &srcaddr, unsigned short &srcport)
{
    int ret = -1;
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr);

    ret = recvfrom(sockfd, (char *)buf, len, 0, (sockaddr *)&remoteAddr, &nAddrLen);
    if(ret >= 0){
        srcaddr = inet_ntoa(remoteAddr.sin_addr);
        srcport = ntohs(remoteAddr.sin_port);
    }

    return ret;
}

bool CSocket::Select(unsigned long long sockfd, int type, unsigned int ms)
{
    bool ret = false;
    fd_set readfds, writefds, exceptfds;
    fd_set *rfd = nullptr;
    fd_set *wfd = nullptr;
    fd_set *efd = nullptr;
    timeval tm, *tmp = nullptr;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    if(type & SockSeleteRead){
        rfd = &readfds;
        FD_SET(sockfd, rfd);
    }
    if(type & SockSeleteWrite){
        wfd = &writefds;
        FD_SET(sockfd, wfd);
    }
    if(type & SockSeleteExcept){
        efd = &exceptfds;
        FD_SET(sockfd, efd);
    }

    if(ms == -1)
        tmp = NULL;
    else{
        memset(&tm, 0, sizeof(tm));
        tm.tv_sec = ms / 1000;
        tm.tv_usec = ms % 1000;
        tmp = &tm;
    }

    ret = select(sockfd+1, rfd, wfd, efd, tmp) > 0;

    return ret;
}

bool CSocket::CloseSocket(unsigned long long fd)
{
    return closesocket(fd) == 0;
}

bool CSocket::GetHostByAddr(CSockHostAddrType type, string addr, string &hostName)
{
    struct hostent *remoteHost;
    struct in_addr addr4;

    memset(&addr4, 0, sizeof(in_addr));

    if(type == SHT_AF_INET){
        addr4.s_addr = inet_addr(addr.c_str());
        remoteHost = gethostbyaddr((char *) &addr4, 4, type);
    }else{
        return false;
    }

    if(remoteHost == NULL) return false;
    hostName = remoteHost->h_name;

    return true;
}

bool CSocket::GetHostByName(string name, HostAddrInfo_t &hostInfo)
{
    struct hostent *remoteHost;

    remoteHost = gethostbyname(name.c_str());
    if(remoteHost == NULL) return false;

    return Hostent2HostAddrInfo_t(remoteHost, hostInfo);
}

bool CSocket::GetHostName(string &name)
{
    bool ret = false;
    int len = 65;

    name.resize(len);
    ret = gethostname(&name[0], len) == 0;

    return ret;
}

bool CSocket::GetPeerName(unsigned long long fd, string &addr, unsigned short &port)
{
    struct sockaddr_in sa;
    int len = sizeof(sa);

    if(getpeername(fd, (struct sockaddr *)&sa, &len) == 0){
        addr = inet_ntoa(sa.sin_addr);
        port = ntohs(sa.sin_port);
        return true;
    }else
        return false;
}

bool CSocket::GetSockName(unsigned long long fd, string &addr, unsigned short &port)
{
    struct sockaddr_in sa;
    int len = sizeof(sa);

    if(getsockname(fd, (struct sockaddr *)&sa, &len) == 0){
        addr = inet_ntoa(sa.sin_addr);
        port = ntohs(sa.sin_port);
        return true;
    }else
        return false;
}

bool CSocket::Close()
{
    return CloseSocket(m_fd);
}

int CSocket::SetSockOpt(unsigned long long sockfd, CSockOptLevel level, CSockOptName optname, const char *optval, int optlen)
{
    return setsockopt(sockfd, level, optname, optval, optlen);
}

int CSocket::GetSockOpt(unsigned long long sockfd, CSockOptLevel level, CSockOptName optname, void *optval, int *optlen)
{
    return getsockopt(sockfd, level, optname, (char *)optval, optlen);
}

bool CSocket::IoctlSocket(unsigned long long fd, long cmd, unsigned long *argp)
{
    return ioctlsocket(fd, cmd, argp) == 0;
}

int CSocket::GetLastError()
{
    return WSAGetLastError();
}

void CSocket::RunSendHeart(void (*func)(void *), void *p)
{
    CThreadFuncP(m_sendHeartThread)->SetFunc(func, p);
    CThreadFuncP(m_sendHeartThread)->Start();
}

void CSocket::StopSendHeart(void (*func)(void *), void *p)
{
    func(p);
    CThreadFuncP(m_sendHeartThread)->Stop();
}

void CSocket::RunRecvHeart(void (*func)(void *), void *p)
{
    CThreadFuncP(m_recvHeartThread)->SetFunc(func, p);
    CThreadFuncP(m_recvHeartThread)->Start();
}

void CSocket::StopRecvHeart(void (*func)(void *), void *p)
{
    func(p);
    CThreadFuncP(m_recvHeartThread)->Stop();
}
