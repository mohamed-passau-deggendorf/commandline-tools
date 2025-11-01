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
if(argc< 3) exit(EXIT_FAILURE);


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
	char d_type;
	char *name;
	};

 struct Entry entry_stack[MAX_DEPTH];

	 buf = (char*) syscall(SYS_mmap,0,  BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
 struct Entry *parent;



 entry_stack[0].fd = open(entry_stack[0].name= argv[1], O_RDONLY | O_DIRECTORY);
 int entry_stack_top = 0;
entry_stack[0].d_type=4;				

 

	while(entry_stack_top!=-1)
	{
   
	if(strcmp(entry_stack[entry_stack_top].name,argv[2]) == 0) {
	char path[255]="/proc/self/fd/";
	char absolute_path[255];
	char fbuffer[32];
	gcvt(entry_stack[entry_stack_top].fd,10,fbuffer);
	strcat(path,fbuffer);
	ssize_t filelen = readlink(path, absolute_path, sizeof(absolute_path)-1);
	absolute_path[filelen] = '\0';
	printf("%s\n",absolute_path); }
        fd =  entry_stack[entry_stack_top--].fd;
	

	while ((nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE))>0 )  for (bpos = 0; bpos < nread;bpos +=*((unsigned short *) (d +16) )){


		

				d =  (buf + bpos);
		d_type = *(buf + bpos +18);
		const char *name = ((char *) (d +19) );
		int size = strlen(name); 
		pid_t cpid = getpid();

		if( strcmp((char *) (d +19),".") !=0  && strcmp((char *) (d +19),"..") !=0 ) {

					++entry_stack_top;	
					strcpy(entry_stack[entry_stack_top].name=malloc(size),name);
					if(d_type == 4) entry_stack[entry_stack_top].fd=openat(fd,entry_stack[entry_stack_top].name, O_RDONLY | O_DIRECTORY);else entry_stack[entry_stack_top].fd=openat(fd,entry_stack[entry_stack_top].name, O_RDONLY );
					entry_stack[entry_stack_top].d_type=d_type;
					parent = & entry_stack[entry_stack_top];

			}



}  }

}
