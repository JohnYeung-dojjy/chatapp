#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PORT "9034" // Port we're listening on

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket() {
    int listener;
    int yes = 1;
    int rv;

    struct addrinfo hints{}, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(nullptr, PORT, &hints, &ai)) != 0) {
        std::cerr << "pollserver: " << gai_strerror(rv) << std::endl;
        exit(1);
    }

    for (p = ai; p != nullptr; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    if (p == nullptr) {
        return -1;
    }

    freeaddrinfo(ai);

    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(std::vector<pollfd> &pfds, int newfd) {
    pollfd pfd{};
    pfd.fd = newfd;
    pfd.events = POLLIN;
    pfds.push_back(pfd);
}

// Remove an index from the set
void del_from_pfds(std::vector<pollfd> &pfds, int index) {
    pfds.erase(pfds.begin() + index);
}

// Handle new connection
void handleNewConnection(
    std::vector<pollfd> &pfds,
    int listener,
    struct sockaddr_storage &remoteaddr,
    char *remoteIP
) {
    socklen_t addrlen = sizeof remoteaddr;
    int newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

    if (newfd == -1) {
        perror("accept");
    } else {
        add_to_pfds(pfds, newfd);
        std::cout << "pollserver: new connection from "
                  << inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP, INET6_ADDRSTRLEN)
                  << " on socket " << newfd << std::endl;
    }
}

// Handle client data
void handleClientData(
    std::vector<pollfd> &pfds,
    int index,
    char *buf,
    int listener
) {
    int sender_fd = pfds[index].fd;
    int nbytes = recv(sender_fd, buf, 256, 0);

    if (nbytes <= 0) {
        if (nbytes == 0) {
            std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl;
        } else {
            perror("recv");
        }

        close(sender_fd);
        del_from_pfds(pfds, index);
    } else {
        for (size_t j = 0; j < pfds.size(); ++j) {
            int dest_fd = pfds[j].fd;

            if (dest_fd != listener && dest_fd != sender_fd) {
                if (send(dest_fd, buf, nbytes, 0) == -1) {
                    perror("send");
                }
            }
        }
    }
}

// Main
int main(int argc, char *argv[]) {
    int listener;
    struct sockaddr_storage remoteaddr;
    char buf[256];
    char remoteIP[INET6_ADDRSTRLEN];

    std::vector<pollfd> pfds;

    listener = get_listener_socket();

    if (listener == -1) {
        std::cerr << "error getting listening socket" << std::endl;
        exit(1);
    }

    pollfd listener_pfd{};
    listener_pfd.fd = listener;
    listener_pfd.events = POLLIN;
    pfds.push_back(listener_pfd);

    for (;;) {
        int poll_count = poll(pfds.data(), pfds.size(), -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }

        for (size_t i = 0; i < pfds.size(); ++i) {
            if (pfds[i].revents & (POLLIN | POLLHUP)) {
                if (pfds[i].fd == listener) {
                    handleNewConnection(pfds, listener, remoteaddr, remoteIP);
                } else {
                    handleClientData(pfds, i, buf, listener);
                }
            }
        }
    }

    return 0;
}