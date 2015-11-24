//
// Created by major on 9/7/15.
//

#include <sys/wait.h>
#include <stdlib.h>
#include "NodeLooper.h"
#include "MicomThread.h"

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
        //int ready = -1;
        //read(pfd[0], &ready, sizeof(int)); // wait parent log reader ready.

        int cchildPid = -1;

        if ((cchildPid = fork()) < 0)
        {
            LOGE("dmesg fork failure");

            if (pfd[0] > 0) {
                close(pfd[0]);
                close(pfd[1]);
            }

            exit(-1);
        }

        if (cchildPid == 0)
        {
            close(pfd[0]);
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[1]);

            char *dmesgargv[] = {"dmesg", NULL};

            if (execvp("dmesg", dmesgargv) == -1)
            {
                LOGE("dmesg exec failed");
            }
        }
        else
        {
            int status = -1;

            waitpid(childPid, &status, WUNTRACED);  // Wait dmesg dump.

            close(pfd[0]);
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[1]);

            char *eargv[] = {"logcat", NULL};

            if (execvp("logcat", eargv) == -1) {
                LOGE("logcat exec failed");
            }
        }
    }
    else // parent
    {
        int status = -1;

        LogCatThread logCatThread(pfd[0]);

        logCatThread.run();

        waitpid(childPid, &status, WUNTRACED);
    }

    exit(EXIT_SUCCESS);
}