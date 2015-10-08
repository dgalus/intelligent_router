#ifndef PAM_H
#define PAM_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <security/pam_appl.h>

#include "log.h"

struct pam_response *reply;

int functionConversation(int numMsg, const struct pam_message **msg, struct pam_response **resp, void *appdataPtr)
{
    *resp = reply;
    return PAM_SUCCESS;
}

bool authenticate(const char *login, const char *password)
{
    if(strlen(login) == 0 || strlen(password) == 0)
    {
        //logg::Instance()->append("Authentication failure. Empty login or password.");
        return false;
    }
    const struct pam_conv localConversation = { functionConversation, NULL };
    pam_handle_t *localAuthHandle = NULL;
    int retval;
    retval = pam_start("whoami", login, &localConversation, &localAuthHandle);
    if(retval != PAM_SUCCESS)
    {
        //logg::Instance()->append("Authentication failure. Could not start PAM module.");
        return false;
    }
    reply = (struct pam_response *) malloc(sizeof(struct pam_response));

    char* p = (char *) malloc(strlen(password)+1);
    p[strlen(password)] = '\0';
    strncpy(p, password, strlen(password));
    reply[0].resp = p;
    reply[0].resp_retcode = 0;
    retval = pam_authenticate(localAuthHandle, 0);

    if(retval != PAM_SUCCESS)
    {
        //logg::Instance()->append("Authentication failure. Bad credentials.");
        return false;
    }
    retval = pam_end(localAuthHandle, retval);
    if(retval != PAM_SUCCESS)
    {
        //logg::Instance()->append("Authentication failure. Could not finish PAM authentication.");
        return false;
    }
    //logg::Instance()->append("User successfully logged in.");
    return true;
}

#endif // PAM_H
