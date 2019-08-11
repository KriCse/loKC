//
// Created by kricse on 8/11/19.
//
#include <security/pam_appl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pam_manager.h"
#include "lock.h"
#include <unistd.h>
char* passwordBuffer;
int conversation(int num_msg, const struct pam_message **msg,
                 struct pam_response **resp, void *appdata_ptr) {
    if(num_msg <= 0 ){
        return PAM_CONV_ERR;
    }
    struct pam_response *response = (struct pam_response *) malloc(num_msg * sizeof(struct pam_response));
    if (response == NULL){
        fprintf(stderr, "Can not allocate Resource");
        return PAM_CONV_ERR;
    }
    for (int i = 0; i < num_msg; ++i) {
        if (msg[i]->msg_style != PAM_PROMPT_ECHO_OFF &&
            msg[i]->msg_style != PAM_PROMPT_ECHO_ON)
            continue;
        response[i].resp_retcode = 0;
        response[i].resp = passwordBuffer;
        if(response[i].resp == NULL){
            return PAM_CONV_ERR;
        }
    }
    *resp = response;
    return PAM_SUCCESS;
}

int startPam(pam_handle_t** pamHandle, char* username){
    int returnValue = pam_start(SERVICE_NAME,
                                username,
            &conv,
            pamHandle);
    if(returnValue == PAM_SUCCESS)
        return 1;
    return 0;
}

int isPasswordCorrect(pam_handle_t** handle) {
    int returnValue  = pam_authenticate(
            *handle,
            0);
    if (returnValue == PAM_SUCCESS) {
        return 1;
    }
    return 0;
}