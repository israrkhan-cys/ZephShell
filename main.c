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

int check_for_pipes(char **args, int pip){
    int i=0;
    while(args[i]!= NULL){
        if(strcmp(args[i],"|")==0){
            pip=i;
            break;
        }
        i++;
    }
    return pip;
}

// int Count_pipes(char **args){
// 	int count=0, i=0; 
// 	while(args[i] != NULL){	
// 	      if(strcmp(args[i],"|")==0){
// 		      count++;
// 	   }
// 	      i++;
//     }
// 	return count;
// }

int Execute_pipe_Cammand(char ** args, int pip){
    //int count_pip= Count_pipes(args);
    char **cmd1, **cmd2;
    cmd1=args;
    args[pip]= NULL;
    cmd2=&args[pip+1];

    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
       exit(EXIT_FAILURE);
    }
    pid_t p1 =fork();
    if(p1 <0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(p1==0){
        
        dup2(pipefd[1],STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
	    if(execvp(cmd1[0],cmd1)==-1){
            perror("exec");
            exit(EXIT_FAILURE);
        }
    }

    pid_t p2 =fork();
    if(p2 <0){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(p2==0){
        dup2(pipefd[0],STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(cmd2[0],cmd2);

    }


    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);
   // free(cmd1); free(cmd2);
    return 1;
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
        
        printf(">>>");
        char * lineInput=NULL;
        ssize_t size=0;
        
        getline(&lineInput,&size, stdin);
    
        lineInput[strcspn(lineInput,"\n")] = '\0';  //replacing the newline "\n" wiht null 
         if(strcmp(lineInput,"\0")==0){ //handling the empty inputss
            continue;
        }

        char **args= split_cammand(lineInput);
        int pip=-1;
        pip=check_for_pipes(args,pip); // checking if the cammand contain pipe to handle it differently 

        if(strcmp(args[0],"cd")==0){
            changeDirectory(args);
        }else if(strcmp(args[0],"exit")==0){
            exit(0);

        }else if(strcmp(args[0],"help")==0) {
            help();
        }else if (pip != -1){
            Execute_pipe_Cammand(args,pip);
        }else{
         execute(args);
        }

        free(lineInput);
        free(args);
    }while(1);


    return 0;
}

