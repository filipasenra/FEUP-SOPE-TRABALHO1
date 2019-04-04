#include "log.h"

long double gettingTime() {
    long double ms; // Milliseconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    ms = (double) spec.tv_nsec / 1.0e4; // Convert nanoseconds to milliseconds
    ms = ms/100;
    return ms;
}

int gettingRegFileCommand(FILE *regFile, long double start, char *cmd)
{
    char act[256] = "COMMAND ";
    strcat(act, cmd);

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileCommand\n");
        return 1;
    }

    return 0;
}

int gettingRegFileSignalOne(FILE *regFile, long double start)
{
    // Setting up char array act
    char act[256] = "SIGNAL USR1";

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileSignalOne\n");
        return 1;
    }

    return 0;
}

int gettingRegFileSignalTwo(FILE *regFile, long double start)
{
    //Setting up char array act
    char act[256] = "SIGNAL USR2";

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileSignalTwo\n");
        return 1;
    }

    return 0;
}

int gettingRegFileAnalized(char *file, FILE *regFile, long double start)
{
    //Setting up char array act
    char act[256] = "ANALIZED ";
    strcat(act, file);

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileAnalized\n");
        return 1;
    }

    return 0;
}

int gettingRegFileFinished(FILE *regFile, long double start)
{
    //Setting up char array act
    char act[256] = "Finished process execution";

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileFinished\n");
        return 1;
    }

    return 0;
}

/**
* @brief Adds a log at the end of a file
*
* @param start  Initial instant
*        end    Final instant
*        act    Description
*        output Output file's name
*
* @return Return zero upon sucess, non-zero otherwise
*/
int addLog(long double start, long double end, char act[], FILE *file_output)
{
    long double inst = end-start;

    fprintf(file_output, "%4.2Lf", inst);
    fflush(file_output);

    fprintf(file_output, " - ");
    fflush(file_output);

    fprintf(file_output, "%d", getpid());
    fflush(file_output);
    fprintf(file_output, " - ");
    fflush(file_output);

    fprintf(file_output, "%s", act);
    fflush(file_output);
    fprintf(file_output, "\n");
    fflush(file_output);

    return 0;
}