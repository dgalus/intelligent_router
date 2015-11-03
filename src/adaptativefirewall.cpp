#include "adaptativefirewall.h"

/*
void signalHandler(int sig)
{
    switch(sig)
    {
        case SIGHUP:
            syslog(LOG_WARNING, "Received SIGHUP signal.");
            break;
        case SIGINT:
        case SIGTERM:
            syslog(LOG_INFO, "Daemon exiting");
            daemonShutdown();
            exit(EXIT_SUCCESS);
            break;
        default:
            syslog(LOG_WARNING, "Unhandled signal %s", strsignal(sig));
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
    dup(i);
    dup(i);

    chdir(runDir);
    pidFileDescriptor = open(pidFile, O_RDWR | O_CREAT, 0600);
    if(pidFileDescriptor == -1)
    {
        syslog(LOG_INFO, "Could not open PID lock file %s, exiting.", pidFile);
        exit(EXIT_FAILURE);
    }

    if(lockf(pidFileDescriptor, F_TLOCK, 0) == -1)
    {
        syslog(LOG_INFO, "Could not lock PID lock file %s, exiting.", pidFile);
        exit(EXIT_FAILURE);
    }

    sprintf(str, "%d\n", getpid());
    write(pidFileDescriptor, str, strlen(str));
}

void daemonShutdown()
{
    close(pidFileDescriptor);
}
*/

int main(int argc, char* argv[])
{
    /* DEBUG */
    /*
     * setlogmask(LOG_UPTO(LOG_DEBUG));
     * openlog(DAEMON_NAME, LOG_CONS, LOG_USER);
     */

    /* logging to syslog */
    //setlogmask(LOG_UPTO(LOG_INFO));
    //openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);

    /* setting up log file */
    Log::getInstance()->setPath(std::string(LOG_FILE));
    std::string test = "Adaptative Firewall Started";
    Log::getInstance()->write(test);

    /* Daemonize */
    //daemonize((char *) "/tmp/", const_cast<char*>(m.adaptativeFirewallPidFile.c_str()));
    //syslog(LOG_INFO, "Adaptive firewall running");

    PolicyApplier* pa = new PolicyApplier(std::string(CRITERION_DIRECTORY), std::string(POLICY_DIRECTORY));
    pa->loop();
/*
    while(1)
    {
        pa->loop();
        sleep(1);
    }*/
  return 0;
}
