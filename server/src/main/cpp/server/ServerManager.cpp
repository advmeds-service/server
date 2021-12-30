//
// Created by shrek on 2021/12/30.
//

#include "ServerManager.h"
#include "Define.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>

void ServerManager::startServer(int port) {
    LOGI("start server port %d",port);

    const char* address = "127.0.0.1";

    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) {
        LOGI("socket error %d", listenFd);
        return;
    }

    int on = 1;
    int result = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (result < 0) {
        LOGI("set socket error %d", result);
        return;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    result = inet_aton(address, &(server_addr.sin_addr));
    if (result < 0) {
        LOGI("convert address error %s", address);
        return;
    }
    server_addr.sin_port = htons(port);

    result = bind(listenFd, (const struct  sockaddr *)&server_addr, sizeof (server_addr));
    if (result < 0) {
        LOGI("bind error %d", result);
        return;
    }

    result = setSocketNonBlocking(listenFd);
    if (result < 0) {
        return;
    }

    result = listen(listenFd, 64);
    if (result < 0) {
        LOGI("listen error fd %d result %d",listenFd, result);
        return;
    }

    epFd = epoll_create(64);

    if (epFd > 0) {
        ev.data.fd = listenFd;
        ev.events = EPOLLIN | EPOLLET;
        result = epoll_ctl(epFd, EPOLL_CTL_ADD, listenFd, &ev);
        if (result < 0) {
            LOGI("add ev error %d", result);
            return;
        }
    } else {
        LOGI("create epoll error %d", epFd);
        return;
    }

    while (loop) {
        LOGI("loop %d", loop);
        int waitCount = epoll_wait(epFd, events, eventCount, -1);
        for (int i = 0; i < waitCount; ++i) {
            uint32_t event = events[i].events;
            if (event & EPOLLERR || event & EPOLLHUP || !(event & EPOLLIN)) {
                LOGI("epoll event error %d", event);
                close(events[i].data.fd);
                continue;
            } else if (events[i].data.fd == listenFd) {
                crateConnection(&ev);
            } else {
                readData(events[i].data.fd);
            }
        }
    }

    LOGI("end loop");
}

int ServerManager::setSocketNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        LOGI("get fd state error %d", fd);
        return -1;
    }

    flags |= O_NONBLOCK;
    int result = fcntl(fd, F_SETFL, flags);
    if (result < 0) {
        LOGI("set fd status error (%d, %d)", fd, result);
        return -1;
    }
    return 0;
}

void ServerManager::readData(int fd) {
    LOGI("start read");
    for (; ;) {
        char buf[128 * 2] = { 0 };
        ssize_t length = read(fd, buf, sizeof(buf) / sizeof(buf[0]));
        LOGI("length %d data: %s", length, buf);
        if (length < 0) {
            if (EAGAIN != errno) {
                LOGI("read data error %d", errno);
            }
            break;
        }
    }
    LOGI("close %d",fd);
    close(fd);
}

int ServerManager::crateConnection(struct epoll_event *ev) {
    int result = 0;
    LOGI("ev %p",ev);
    char clientAddress[NI_MAXHOST];
    char clientPort[NI_MAXSERV];
    for (; ;) {
        struct sockaddr inAddress = { 0 };
        socklen_t inAddressLength = sizeof(inAddress);
        int acceptFd = accept(listenFd, &inAddress, &inAddressLength);

        if (acceptFd < 0) {
            LOGI("accept error %d", listenFd);
            break;
        }
        result = getnameinfo(&inAddress, sizeof(inAddress), clientAddress, sizeof(clientAddress), clientPort, sizeof(clientPort), NI_NUMERICHOST | NI_NUMERICSERV);
        if (result < 0) {
            LOGI("get client info error");
        } else {
            LOGI("new connection: host = %s, port = %s", clientAddress, clientPort);
        }

        result = setSocketNonBlocking(acceptFd);
        if (result < 0) {
            LOGI("set accept fd error");
            close(acceptFd);
            return -1;
        }

        ev->data.fd = acceptFd;
        ev->events = EPOLLIN | EPOLLET;
        result = epoll_ctl(epFd, EPOLL_CTL_ADD, acceptFd, ev);
        if (result < 0) {
            LOGI("add epoll event accept fd error %d", result);
            close(acceptFd);
            return -1;
        }

    }
    return 0;
}

void ServerManager::stopServer() {
    loop = false;
    close(epFd);
    close(listenFd);
    loop = true;
    LOGI("stop server");
}

