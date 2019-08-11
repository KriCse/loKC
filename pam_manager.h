//
// Created by kricse on 8/11/19.
//

#ifndef LOKC_PAM_MANAGER_H
#define LOKC_PAM_MANAGER_H
#define SERVICE_NAME  "lokc"
#define PASSWORD_BUFFER_SIZE 256

int conversation(int num_msg, const struct pam_message **msg,
                 struct pam_response **resp, void *appdata_ptr);

static struct pam_conv conv = {
        conversation,
        NULL
};

int startPam(pam_handle_t** pamHandle, char* username);

int isPasswordCorrect(pam_handle_t** handle);

#endif //LOKC_PAM_MANAGER_H
