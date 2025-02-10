#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    pid_t pid;
    char directory[50];

    // For this program layout, we look at "../test_files"
    printf("Type in the directory where you want to look: \n");
    scanf("%s",&directory);

    char word[50];
    printf("Type the word you want to find in said directory: \n");
    scanf("%s",&word);

    // WE HAVE TO BUILD THE "word_explorer" PROGRAM FIRST IN THE "word_explorer/build" FOLDER FOR THIS TO WORK
    char * argArray[] = {"../../word_explorer/build/word_explorer",directory,word,NULL};

    pid = fork();
    if(pid==0){
        printf("I am the child, and I will look for the word %s!\n", word);
        execv(argArray[0], argArray);
        exit(0);
    }
    else{
        wait(NULL);
        printf("I am the parent, and I have waited on my child.\n");
    }
}