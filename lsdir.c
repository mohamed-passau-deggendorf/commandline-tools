#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/mman.h> 
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>


#define BUF_SIZE 8192
#define MAX_DEPTH 2047
#define MAX_NAME_LENGTH 255

int main(int argc,char*argv[]) {
if(argc< 2) exit(EXIT_FAILURE);


int fd, nread;
   char *buf;
   void *d;
    int bpos;	
   char d_type;
   int position;
  int pid; 
 int k;


 struct Entry {
	int fd;
	int level;
	char d_type;
	char *name;

	};

 struct Entry entry_stack[MAX_DEPTH];

	 buf = (char*) syscall(SYS_mmap,0,  BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);



 entry_stack[0].fd = open(entry_stack[0].name= argv[1], O_RDONLY | O_DIRECTORY);
 int entry_stack_top = 0;
 entry_stack[0].level=0;
entry_stack[0].d_type=4;		
 int level;

 

	while(entry_stack_top!=-1)
	{
   
	for(k=0;k<entry_stack[entry_stack_top].level;k++) printf("_"); printf(" %s \n",entry_stack[entry_stack_top].name);
        fd =  entry_stack[entry_stack_top].fd;
	level =  entry_stack[entry_stack_top--].level;
	

	while ((nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE))>0 )  for (bpos = 0; bpos < nread;bpos +=*((unsigned short *) (d +16) )){


		

				d =  (buf + bpos);
		d_type = *(buf + bpos +18);
		const char *name = ((char *) (d +19) );
		int size = strlen(name); 
		pid_t cpid = getpid();

		if( strcmp((char *) (d +19),".") !=0  && strcmp((char *) (d +19),"..") !=0 ) {

					++entry_stack_top;	
					strcpy(entry_stack[entry_stack_top].name=malloc(size),name);
					if(d_type == 4) entry_stack[entry_stack_top].fd=openat(fd,entry_stack[entry_stack_top].name, O_RDONLY | O_DIRECTORY);
					entry_stack[entry_stack_top].d_type=d_type;
					entry_stack[entry_stack_top].level=level+1;

			}



}  }

}
