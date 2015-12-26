#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#define AF_DAEMON_NAME "Adaptive Firewall"
#define WWW_DAEMON_NAME "Router web interface"

#define CONFIG_DIRECTORY "/opt/router/config/"
#define VAR_DIRECTORY "/opt/router/var/"
#define ERROR_PAGES_DIR "/opt/router/error_pages/"
#define MODULES_DIR "/opt/router/sites/"
#define HTML_TEMPLATES "/opt/router/sites/html/"
#define CSS_TEMPLATES "/opt/router/sites/css/"
#define CRITERION_DIRECTORY "/opt/router/criterion"
#define POLICY_DIRECTORY "/opt/router/policy"

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
