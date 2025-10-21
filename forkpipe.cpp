#include <iostream>
#include <cstdlib>
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <cerrno>
#include <cstring>   
#include <signal.h>

void signal_reaction(int sig_id){
    // std::cerr  << "GEN TERMINATED" << std::endl;
    // fflush(stdout);
    char msg[]= "GEN TERMINATED\n";
    write(STDERR_FILENO, msg, sizeof(msg)-1);
    _exit (0);
}

int main(int argc, char *argv[]) {
    pid_t gen, nsd;
    // int status;
    // pid_t pid = wait (&status);

    int fd [2];
    if (pipe(fd) == -1){
        perror("pipe");
        return 2;
    }
    gen = fork(); 

    if (gen < 0 ){
        std::cerr << "ERROR" << std::endl;
        return 2;
    }
    if (gen == 0){

        signal(SIGTERM, signal_reaction);
        if(dup2(fd[1], STDOUT_FILENO) == -1) return 2;
        close(fd[0]);
        close(fd[1]);
        setbuf(stdout, NULL);
        while(1){
            int a = rand() % 4096;
            int b = rand() % 4096;
            std::cout << a << " " << b << "\n";
            fflush(stdout);
            sleep(1);
        }
        _exit(0);
    }

    nsd = fork();
    if (nsd < 0 ){
        std::cerr << "ERROR\n";
        fflush(stdout);
        return 2;
    }

    if (nsd == 0){
        if(dup2(fd[0], STDIN_FILENO) == -1) return 2;

        close(fd[0]);
        close(fd[1]);
        // char buf [128];
        
        execl("./nsd", "nsd", NULL);
        // perror("nsd: execl");
        exit(2);
    }
 
    close(fd[0]);
    close(fd[1]);
    sleep(5);

    if(kill(gen, SIGTERM) == -1) return 2;
    // kill(nsd, SIGTERM);
    int status;
    bool error_found = false;
    int k = 2;
    do{
       pid_t w = wait(&status);
       if(w == -1 ){perror("wait"); return 2;}
       // stoppped by signal or returnd value != 0
       if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0)){
            error_found = true;
       }
       --k;
    }while (k > 0);

    if(!error_found){ std::cout << "OK\n";}
    else {std::cout << "ERROR\n"; fflush(stdout);}


    // CLosign the parent now
  
    

    return error_found ? 1 : 0;
}



