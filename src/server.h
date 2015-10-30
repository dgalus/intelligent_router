#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include "helpers.h"
#include "configuration.h"

extern const char* program_name;
extern int verbose;

extern void* xmalloc(size_t size);
extern void* xrealloc(void* ptr, size_t size);
extern char* xstrdup(const char* s);
extern void systemError(const char* operation);
extern void error(const char* cause, const char* message);
extern char* getSelfExecutableDirectory();

struct ServerModule
{
    void* handle;
    const char* name;
    void (*generateFunction)(int, std::map<std::string, std::string>);
};
extern char* moduleDir;
extern struct ServerModule* moduleOpen(const char* modulePath);
extern void moduleClose(struct ServerModule* module);
extern void serverRun(struct in_addr localAddress, uint16_t port);

#endif // SERVER_H
