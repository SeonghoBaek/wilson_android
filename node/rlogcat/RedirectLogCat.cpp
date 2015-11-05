//
// Created by major on 9/7/15.
//

#include <sys/wait.h>
#include <stdlib.h>
#include "NodeLooper.h"
#include "LogCatThread.h"

using namespace android;

// rlogcat node
int main(int argc, char **argv)
{
    pid_t childPid = -1;

    int pfd[2];

    pipe(pfd);

    if ( (childPid = fork()) < 0 )
    {
        LOGE("RedirectLogCat Fork Failure");

        if (pfd[0] > 0)
        {
            close(pfd[0]);
            close(pfd[1]);
        }

        exit(-1);
    }

    if (childPid == 0) // child
    {
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);

        char* eargv[] = {"logcat", NULL};

        if (execvp("logcat", eargv) == -1)
        {
            LOGE("logcat exec failed");
        }
    }
    else // parent
    {
        int status;

        LogCatThread logCatThread(pfd[0]);

        logCatThread.run();

        waitpid(childPid, &status, WUNTRACED);
    }

    exit(EXIT_SUCCESS);
}