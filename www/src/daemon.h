#ifndef DAEMON_H
#define DAEMON_H

#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>

#define DAEMON_NAME "Router web interface"

int pidFileDescriptor;

void daemonShutdown()
{
    close(pidFileDescriptor);
}

void signalHandler(int sig)
{
    switch(sig)
    {
        case SIGHUP:
            syslog(LOG_WARNING, "%s: Received SIGHUP signal.", DAEMON_NAME);
            break;
        case SIGINT:
        case SIGTERM:
            syslog(LOG_INFO, "%s: Daemon exiting.", DAEMON_NAME);
            daemonShutdown();
            exit(EXIT_SUCCESS);
            break;
        default:
            syslog(LOG_WARNING, "%s: Unhandled signal %s", DAEMON_NAME, strsignal(sig));
            break;
    }
}

void daemonize(char *runDir, char *pidFile)
{
    pid_t pid;
    pid_t sid;
    int i;
    char str[10];
    struct sigaction sigAction;
    sigset_t sigSet;
    int ret;

    if(getppid() == 1)
    {
        return;
    }

    sigemptyset(&sigSet);
    sigaddset(&sigSet, SIGCHLD);
    sigaddset(&sigSet, SIGTSTP);
    sigaddset(&sigSet, SIGTTOU);
    sigaddset(&sigSet, SIGTTIN);
    sigprocmask(SIG_BLOCK, &sigSet, NULL);

    sigAction.sa_handler = signalHandler;
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags = 0;

    sigaction(SIGHUP, &sigAction, NULL);
    sigaction(SIGTERM, &sigAction, NULL);
    sigaction(SIGINT, &sigAction, NULL);

    pid = fork();
    if(pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        //printf("Child process created: %d\n", pid);
        exit(EXIT_SUCCESS);
    }
    umask(027);

    sid = setsid();
    if(sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    for(i = getdtablesize(); i >= 0; --i)
    {
        close(i);
    }

    i = open("/dev/null", O_RDWR);
    ret = dup(i);
    ret = dup(i);

    ret = chdir(runDir);
    pidFileDescriptor = open(pidFile, O_RDWR | O_CREAT, 0600);
    if(pidFileDescriptor == -1)
    {
        syslog(LOG_INFO, "Could not open PPID lock file %s, exiting.", pidFile);
        exit(EXIT_FAILURE);
    }

    if(lockf(pidFileDescriptor, F_TLOCK, 0) == -1)
    {
        syslog(LOG_INFO, "Could not lock PID lock file %s, exiting.", pidFile);
        exit(EXIT_FAILURE);
    }

    sprintf(str, "%d\n", getpid());
    ret = write(pidFileDescriptor, str, strlen(str));
}

#endif // DAEMON_H
