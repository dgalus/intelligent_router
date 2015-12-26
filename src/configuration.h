#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#define AF_DAEMON_NAME "Adaptive Firewall"
#define WWW_DAEMON_NAME "Router web interface"

#define CONFIG_DIRECTORY "../config/"
#define VAR_DIRECTORY "../var/"
#define ERROR_PAGES_DIR "../error_pages/"
#define MODULES_DIR "../sites/"
#define HTML_TEMPLATES "../sites/html/"
#define CSS_TEMPLATES "../sites/css/"
#define CRITERION_DIRECTORY "../criterion"
#define POLICY_DIRECTORY "../policy"

#define WWW_PID_FILE VAR_DIRECTORY "www.pid"
#define AF_PID_FILE VAR_DIRECTORY "af.pid"
#define WWW_PORT_FILE VAR_DIRECTORY "www.port"
#define LOADED_POLICY VAR_DIRECTORY "loaded.policy"
#define FIREWALL_RULES VAR_DIRECTORY "firewall.rules"
#define ROUTING_PROTOCOLS VAR_DIRECTORY "routing.protocols"
#define STATIC_ROUTING VAR_DIRECTORY "static.routing"
#define LOG_FILE VAR_DIRECTORY "af.log"

#define CRITERIONS_CONFIG_PATH CONFIG_DIRECTORY "criterions.config"
#define POLICIES_CONFIG_PATH CONFIG_DIRECTORY "policies.config"


#endif
