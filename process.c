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

int pidstr(const char *path,int size){
int pid = 0;
int k;

for(k=0;k<size;k++) if(path[k]>='0' && path[k]<='9') pid = 10*pid + (path[k]-'0');else return -1;
return pid;

}

int main(int argc,char*argv[])  {


int fd, nread;
   char *buf;
   void *d;
    int bpos;	
   char d_type;
   int position;
  int pid; 

	 buf = (char*) syscall(SYS_mmap,0,  BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

 fd = /*syscall(SYS_open,*/open("/proc", O_RDONLY | O_DIRECTORY);

	while ((nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE))!=0 )  for (bpos = 0; bpos < nread;bpos +=*((unsigned short *) (d +16) ))

{

				d =  (buf + bpos);
		d_type = *(buf + bpos +18);
		const char *name = ((char *) (d +19) );
		int size = strlen(name); 
		pid=pidstr(name,size);
		pid_t cpid = getpid();


	if(d_type == 4 && strcmp((char *) (d +19),".") !=0  && strcmp((char *) (d +19),"..") !=0 

&& (pid=pidstr(name,size))>0 && pid !=cpid


) {

			int pfd =  openat(fd,name, O_RDONLY | O_DIRECTORY);

				char exe_buffer[2048];
                        size_t exe_size = readlinkat(pfd,"exe",exe_buffer,2048);
                        exe_buffer[exe_size]=0;
			

printf("%d %s\n",pid,exe_buffer);

			


			int fdfd =  openat(pfd,"fd", O_RDONLY | O_DIRECTORY);

			int file_nread;
			 char *file_buf;
			   void *file_d;
    			int file_bpos;   
   			char file_d_type;
   			int file_position;



			  file_buf = (char*) syscall(SYS_mmap,0,  BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  
			if(fdfd>0)
			while ((file_nread = syscall(SYS_getdents64, fdfd, file_buf, BUF_SIZE))>0 )  


				 for (file_bpos = 0;file_bpos < file_nread;file_bpos +=*((unsigned short *) (file_d +16) ))  {


				 file_d =  (file_buf + file_bpos);
                		file_d_type = *(file_buf + file_bpos +18);
                		const char *file_name = ((char *) (file_d +19) );
                		int file_size = strlen(file_name); 
				if(file_d_type == 10) {
				char original_buffer[2048];
				size_t link_size = readlinkat(fdfd,file_name,original_buffer,2048);
				original_buffer[link_size]=0;
				printf("\t%s\n",original_buffer);




				}

///



	}


}



	}




}
