//
// Created by shrek on 2021/12/30.
//

#ifndef SERVERDEMO_SERVERMANAGER_H
#define SERVERDEMO_SERVERMANAGER_H

#include <sys/epoll.h>

class ServerManager {
private:
    bool loop = true;
    int epFd;
    int listenFd;
    struct epoll_event ev;
    const static int eventCount = 128;
    struct epoll_event events[eventCount];

    ServerManager() {};
    ~ServerManager() {};
    ServerManager(const ServerManager&);
    ServerManager& operator=(const ServerManager&);

    int setSocketNonBlocking(int fd);

    int crateConnection(struct epoll_event *ev);

    void readData(int fd);

public:
    static ServerManager& getInstance() {
        static ServerManager instance;
        return instance;
    }

    void startServer(int port);

    void stopServer();
};


#endif //SERVERDEMO_SERVERMANAGER_H
