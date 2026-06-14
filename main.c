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

void changeDirectory(char **args){
    if(args[1] == NULL){
        perror("Error: No arguments");
    }else if(chdir(args[1]) != 0){    // 0 mean the cammand was sucess
        perror("Error:");
    }
}

void help(){
    printf("=== ZephShell, A Unix shell ===\n");
    printf("Enter cammand and hit Enter\n");
    printf("Make sure to provide necessary arguments for a cammand\n");
    printf("some of the builtins are:\n 1.cd \n 2. help \n 3. exit \n");
    printf("To get info on each cammand Use Man\n\n");
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


        if(strcmp(args[0],"cd")==0){
            changeDirectory(args);

        }else if(strcmp(args[0],"exit")==0){
            exit(0);

        }else if(strcmp(args[0],"help")==0) {
            help();
        }else{
            execute(args);
        }



        free(lineInput);
        free(args);
    }while(1);


    return 0;
}

