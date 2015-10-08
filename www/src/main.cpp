#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "common.h"
#include "daemon.h"
#include "log.h"
#include "server.h"
#include "templates.h"

int port = -1;
char* logPath;

extern void daemonShutdown();
extern void daemonize(char *runDir, char *pidFile);
extern void signalHandler(int sig);
extern void printUsage(bool isError, char *programName);

int main(int argc, char* argv[])
{
    struct in_addr localAddress;
    uint16_t port;
    int nextOption;
    //char* logPath = const_cast<char*>(std::string("router.www.log").c_str());

    localAddress.s_addr = INADDR_ANY;
    port = 0;
    moduleDir = getSelfExecutableDirectory();
    assert(moduleDir != NULL);

    /* get arguments */
    do
    {
        nextOption = getopt_long(argc, argv, shortOptions, longOptions, NULL);
        switch(nextOption)
        {
            case 'a':
                {
                    struct hostent* localHostName;
                    localHostName = gethostbyname(optarg);
                    if(localHostName == NULL || localHostName->h_length == 0)
                    {
                        error(optarg, "Invalid host name");
                    }
                    else
                    {
                        localAddress.s_addr = *((int *) (localHostName->h_addr_list[0]));
                    }
                }
                break;
            case 'h':
                printUsage(false, argv[0]);
            case 'm':
                {
                    struct stat dirInfo;
                    if(access(optarg, F_OK) != 0)
                    {
                        error(optarg, "Module directory does not exists");
                    }
                    if(access(optarg, R_OK | X_OK) != 0)
                    {
                        error(optarg, "Module directory is not accessible");
                    }
                    if(stat(optarg, &dirInfo) != 0 || !S_ISDIR(dirInfo.st_mode))
                    {
                        error(optarg, "Not a directory");
                    }
                    moduleDir = strdup(optarg);
                }
                break;
            case 'l':
                {
                    logPath = strdup(optarg);
                }
            case 'p':
                {
                    long value;
                    char* end;
                    value = strtol(optarg, &end, 10);
                    if(*end != '\0')
                    {
                        printUsage(true, argv[0]);
                    }
                    port = (uint16_t) htons(value);

                }
                break;
            case '?':
                printUsage(true, argv[0]);
            case -1:
                break;
            default:
                abort();
        }
    } while(nextOption != -1);

    if(port <= 0)
    {
        printUsage(true, argv[0]);
    }


    if(optind != argc)
    {
        printUsage(true, argv[0]);
    }

    /* logging to syslog */
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);

    /* setting up log file */
    //logg::Instance()->setPath(std::string(logPath));

    /* run the server */
    serverRun(localAddress, port);

    return 0;
}
