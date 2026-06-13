#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 64

void execute(char **args){
    int x;
    x =fork();

    if (x < 0){
        perror("Fork Failed");
    }

    if(x == 0){
        if(execvp(args[0],args) == -1){
            perror("Error:");
        }
        exit(0);
    }
    if(x > 0){
        wait(0);
    }
}

/* Fuction that will be used to split a long cammand with flags
and many argumnets into in array 
*/
char ** split_cammand(char *line){
    int buf = MAX_ARGS;
  char **args= malloc(buf* sizeof(char*));
  char *arg;

  int i=0;
  arg = strtok(line," ");
  while(arg != NULL){
    args[i]=arg;
    i++;
    arg = strtok(NULL," ");
  }

  args[i]= NULL;
  return args;
}

int main(){
    // main shell  loop
    do{
        
        printf(">>");
        char * lineInput=NULL;
        ssize_t size=0;
        
        getline(&lineInput,&size, stdin);
        lineInput[strcspn(lineInput,"\n")] = '\0';  //replacing the newline "\n" wiht null 
        char ** args= split_cammand(lineInput);

        execute(args);
        free(lineInput);
        free(args);
    }while(1);


    return 0;
}

