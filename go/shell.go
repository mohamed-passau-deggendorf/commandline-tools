package main

/*
#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>
#include <signal.h>

int waitprog(const char*file,const char*  argv[]){
		pid_t pid;
		if((pid=fork())  == 0 ) return execvpe(file, argv, NULL);
	waitpid(pid,NULL,0);
	}


int runprog(const char*file,const char*  argv[]){
		if(fork()  == 0 ) return execvpe(file, argv, NULL);
	
	}



*/
import "C"
import "strings"
import "bufio"
import "fmt"
import "os"


func main() {
  
	reader := bufio.NewReader(os.Stdin)
	redo :
	fmt.Print(">> ")
	input, _ := reader.ReadString('\n')
	 input = strings.TrimSpace(input)
	 argv := strings.Fields(input) 
	 cArgv := make([]*C.char, len(argv)+1)
	

	 for key, value := range argv {
        cArgv[key] = C.CString(value) }
	cArgv[len(argv)] = nil  

	

	C.waitprog(C.CString( argv[0]) , &cArgv[0]  )
	goto redo


}





