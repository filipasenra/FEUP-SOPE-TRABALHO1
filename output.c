#include "output.h"

/**
 * @brief Reedirects the output of the command given to a FILE
 * 
 * @param File to be analysed
 *        Command to be given
 * 
 * @return Returns a pointer to the FILE where the output of the command is redirected
*/
FILE *file_of_command(char *file, const char command[])
{
    FILE *in = NULL;
    char cmd[256];
    strcpy(cmd, command);
    strcat(cmd, " ");
    strcat(cmd, file);
    in = popen(cmd, "r");

    return in;
}

int outputTypeOfFile(char file[])
{
    //===============================================
    //TYPE OF FILE

    //Runs command of shell, but to read within program
    FILE *in_type_of_file = file_of_command(file, "file ");

    if (in_type_of_file == NULL)
    {
        printf("Error in file_of_command ");
        return 2;
    }

    //Reads line by line the result of the command
    char temp[256];
    fgets(temp, 255, in_type_of_file);

    //Cuts C-string to give only what we want
    char *type_file = strndup(temp + strlen(file) + 2, strlen(temp));

    for (int i = 0; i < (int)strlen(type_file); i++)
    {
        if (type_file[i] == '\n')
        {
            type_file[i] = 0;
            break;
        }
    }

    printf("%s, ", type_file);

    pclose(in_type_of_file);

    return 0;
}

int outputTimeISO_8601(struct tm *time)
{
    //Printing modification time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d", time->tm_mday, time->tm_mon + 1, time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec);

    return 0;
}

int outputHash(char file[], char command[])
{

    FILE *hash = file_of_command(file, command);

    if (hash == NULL)
    {
        printf("Error in file_of_command!\n");
        return 3;
    }

    //Reads line by line the result of the command
    char temp[256];

    if (fgets(temp, 255, hash) == NULL)
    {
        printf("Error in fgets!\n");
        return 4;
    }

    //Cuts C-string to get only what we want
    char *string_md5 = strndup(temp, strlen(temp) - (strlen(file) + 3));

    printf(", %s", string_md5);

    pclose(hash);

    return 0;
}

int outputPermissions(mode_t mode)
{
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf(", ");

    return 0;
}

/**
 * @brief Getting the output of a file
 * 
 * @param file Name of the file
 *        MD5 If it should display MD5 hash
 *        SHA1 If it should display SHA1 hash
 *        SHA256 If it should display SHA256 hash
 * 
 * @return Returns zero upon sucess, non-zero otherwise
*/
int gettingOutputFile(char *file, bool MD5, bool SHA1, bool SHA256)
{
    struct stat fileStat;

    if (stat(file, &fileStat) < 0)
    {
        printf("FileStat failed!\n");
        return 1;
    }

    //FILE NAME
    printf("%s, ", file);

    //TYPE OF FILE
    outputTypeOfFile(file);

    //FILE SIZE
    printf("%ld, ", fileStat.st_size);

    //FILE PERMISSIONS
    outputPermissions(fileStat.st_mode);

    //MODIFICATION TIME
    outputTimeISO_8601(localtime(&fileStat.st_mtime));
    printf(", ");

    //LAST ACESS TIME
    outputTimeISO_8601(localtime(&fileStat.st_atime));

    //HASH

    //se é um diretorio, não tem hash
    if (S_ISDIR(fileStat.st_mode))
    {
        printf("\n");
        return 0;
    }

    if (MD5)
    {
        char md5command[] = "md5sum ";
        outputHash(file, md5command);
    }

    if (SHA1)
    {
        char sha1command[] = "sha1sum ";
        outputHash(file, sha1command);
    }

    if (SHA256)
    {
        char sha256command[] = "sha256sum ";
        outputHash(file, sha256command);
    }

    printf("\n");

    //=================================================

    return 0;
}