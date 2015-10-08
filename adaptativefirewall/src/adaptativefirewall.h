#ifndef ADAPTATIVEFIREWALL_H
#define ADAPTATIVEFIREWALL_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "configfile.h"
#include "log.h"
#include "policyapplier.h"

#define DAEMON_NAME "Adaptative Firewall"
#define CONFIG_DIRECTORY "../config/"
#define MAIN_CONFIG_PATH CONFIG_DIRECTORY "main.config"
#define CRITERIONS_CONFIG_PATH CONFIG_DIRECTORY "criterions.config"
#define POLICIES_CONFIG_PATH CONFIG_DIRECTORY "policies.config"

//void daemonShutdown();
//void signalHandler(int sig);
//void daemonize(char *runDir, char *pidFile);

//int pidFileDescriptor;

#endif
