//
// Created by major on 9/7/15.
//

#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils/RefBase.h>
#include <signal.h>
#include "Message.h"
#include "NBus.h"
#include "USBThread.h"

using namespace android;

int main(int argc, char **argv)
{
    pid_t   childPid = -1;
    int     pfd[2];
    int     id = MSG_KEYWORD(USB_LOGCAT_DONE);
    char    *text = "Logcat saved in USB Storage";
    FILE    *pFile = NULL;

    pFile = fopen("/storage/usb0/_u.log", "w");

    if (pFile == NULL)
    {
        id = MSG_KEYWORD(USB_NOT_READY);
        text = "USB Storage Not Ready";
        
        char *jsonString = new char[512];
        memset(jsonString, 0, 512);
        
        sprintf(jsonString, "{\"id\":\"%d\",\"text\":\"%s\"}", id, text);
        
        nbus_cast("node.wilson", jsonString, strlen(jsonString));
        
        exit(EXIT_FAILURE);
    }
    else
    {
        fclose(pFile);
        unlink("/storage/usb0/_u.log");
    }
    
    pipe(pfd);

    if ( (childPid = fork()) < 0 )
    {
        LOGE("USBLogCat Fork Failure");

        if (pfd[0] > 0)
        {
            close(pfd[0]);
            close(pfd[1]);
        }

        exit(EXIT_FAILURE);
    }

    if (childPid == 0) // child
    {
        //int ready = -1;
        //read(pfd[0], &ready, sizeof(int)); // wait parent log reader ready.

        int status = -1;

        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        //close(pfd[1]);

        char *eargv[] = {"logcat", NULL};

        if (execvp("logcat", eargv) == -1)
        {
            LOGE("logcat exec failed");
            
            exit(EXIT_FAILURE);
        }
    }
    else // parent
    {
        int status = -1;
        char *jsonString = new char[512];
        int sleepTime = 10;

        USBThread reportThread(pfd[0]);

        reportThread.run();
        
        memset(jsonString, 0, 512);
        
        id = MSG_KEYWORD(USB_LOGCAT_WAIT);
        text = "Making logcat in USB. Wait.";
        
        sprintf(jsonString, "{\"id\":\"%d\",\"text\":\"%s\"}", id, text);
        
        nbus_cast("node.wilson", jsonString, strlen(jsonString));

        for (int i = 0; i < sleepTime; i++)
        {
            sleep(1);
        }

        kill(childPid, SIGQUIT);

        waitpid(childPid, &status, WUNTRACED);

        reportThread.stopThread();
        
        if (WEXITSTATUS(status) == EXIT_FAILURE)
        {
            id = MSG_KEYWORD(USB_LOGCAT_FAIL);
            text = "Logcat Failure";
        }
        else
        {
            id = MSG_KEYWORD(USB_LOGCAT_DONE);
            text = "Logcat saved in USB Storage";
        }
        
        memset(jsonString, 0, 512);
        
        sprintf(jsonString, "{\"id\":\"%d\",\"text\":\"%s\"}", id, text);
        
        nbus_cast("node.wilson", jsonString, strlen(jsonString));
    }
    
    close(pfd[0]);
    close(pfd[1]);

    exit(EXIT_SUCCESS);
}