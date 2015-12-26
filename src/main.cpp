#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "common.h"
#include "configuration.h"
#include "daemon.h"
#include "helpers.h"
#include "log.h"
#include "server.h"

int port = -1;

extern void daemonShutdown();
extern void daemonize(char *runDir, char *pidFile);
extern void signalHandler(int sig);

int main(int argc, char* argv[])
{
    struct in_addr localAddress;
    uint16_t port;
    //char* logPath = const_cast<char*>(std::string("router.www.log").c_str());

    localAddress.s_addr = INADDR_ANY;
    port = 0;
    moduleDir = std::string(MODULES_DIR).c_str();
    assert(moduleDir != NULL);

    std::string portStr = FileReader::readFile(std::string(WWW_PORT_FILE));
    port = htons(std::stoi(portStr));
    if(port <= 0)
    {
        exit(PORT_FILE_EMPTY_OR_NOT_VALID_PORT);
    }

    /* logging to syslog */
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(WWW_DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);

    /* setting up log file */
    //logg::Instance()->setPath(std::string(logPath));

    /* run the server */
    serverRun(localAddress, port);

    return 0;
}
