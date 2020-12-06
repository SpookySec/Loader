#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

// ProtoTypes here...
void printShit(const char *fileName);

// Main
int main(int argc, char *argv[]) {

    // Check for the target file
    if (argc != 2) {
        printf("argsss argssssssssssss!!!\n");
        printf("aka GIVE A BINARY FILE <BINARY>\n");
        return 1;
    }

    // Print stat.h things
    printf("--- FILE NAME INFO ---\n======================\n");
    char *fileName = *(argv + 1);
    printShit(fileName);

    // Create an anonymous & empty file descriptor 
    char *anonymousFileName = "I'm an anonymous file!";
    int bytes = 0;
    printf("\n--- FILE DATA INFO ---\n======================\n");
    int memFd = memfd_create(anonymousFileName, 0);
    int fd = open(fileName, O_RDONLY);
    printf("[*] Binary File Descriptor    : %d\n", fd);
    printf("[*] Anonymous File Descriptor : %d\n", memFd);

    // Using 'sendfile' (SO GLAD I FOUND THIS) 
    struct stat stats;
    stat(fileName, &stats);
    printf("[~] Running 'sendfile'...\n");
    if ( (bytes = sendfile(memFd, fd, 0,  stats.st_size)) == -1) {
        return 1;
    } else {
        printf("[*] Copied %d bytes\n", bytes);
    }

    // Running the new anonymous file using fexecve()
    struct stat new;
    fstat(memFd, &new);
    printf("\n--- ANONYMOUS INFO ---\n======================\n");
    printf("[*] Anonymous File Name : %s\n", anonymousFileName);
    printf("[*] Anonymous File Size : %ld\n", new.st_size);
    printf("[*] Anonymous Block Size: %ld\n", new.st_blksize);
    printf("\n--- RUNNING BINARY ---\n======================\n");
    char *envp[] = { NULL };
    char *argshit[] = { fileName, NULL };
    fexecve(memFd, argshit, envp);
}

// Function to print stat.h file stuff
void printShit(const char *fileName) 
{ 
    struct stat stats;

    if ( stat(fileName, &stats) != -1 ) {
        printf("[*] File Name : %s\n", fileName);
        printf("[*] File Size : %ld\n", stats.st_size);
        printf("[*] Device ID : %ld\n", stats.st_dev);
        printf("[*] Block Size: %ld\n", stats.st_blksize);
        printf("[*] USER ID   : %d\n", stats.st_uid);
        printf("[*] GROUP ID  : %d\n", stats.st_gid);
        printf("[*] Hard Links: %ld\n", stats.st_nlink);
    } else {
        return;
    }
}
