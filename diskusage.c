#define _GNU_SOURCE
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
#include <sys/statfs.h>

#define BUF_SIZE 8192
#define READING_SIZE 2048
#define BUFFER_SIZE 8192

int main(int argc,char*argv[]){

int fd;
   char *buf;


	 buf = (char*) syscall(SYS_mmap,0,  BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

 fd = open("/proc/mounts", O_RDONLY );

  enum MState{ReadingDevice,ReadingMountPoint,ReadingFormat,ReadingOtherData};	
  enum MState state=ReadingDevice;

 char buffer[2048];
	int read_byte, offset=0 , k;

  struct mountinfo{
	char dev[32];
	char path[32];
	char format[8];

	};

	 struct mountinfo mi;
	 int l=0;


  while( (read_byte = read(fd,buffer + offset,
                         (BUFFER_SIZE-offset)  >= READING_SIZE ? READING_SIZE :  (BUFFER_SIZE-offset)   )) >0 ) 
    for(k=offset;k<read_byte + offset;k++)  {



	switch(state){

		case ReadingDevice : if(buffer[k]!=' ') mi.dev[l++]=buffer[k];else { mi.dev[l]='\0'; state=ReadingMountPoint; l=0;}  break;
		case ReadingMountPoint : if(buffer[k]!=' ') mi.path[l++]=buffer[k];else { mi.path[l]='\0'; state=ReadingFormat; l=0;}  break;
		case ReadingFormat : if(buffer[k]!=' ') mi.format[l++]=buffer[k];else { mi.format[l]='\0'; state=ReadingOtherData; l=0;}  break;
		case ReadingOtherData : if(buffer[k] == '\n') {
					
			int dfd = open(mi.path, O_RDONLY | O_DIRECTORY);
			char *stat_buffer = (char*) syscall(SYS_mmap,0,  BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
			fstatfs(dfd,stat_buffer);
			
			unsigned long  xtotal =  (*(unsigned long *)(stat_buffer+16)) * (*(unsigned long *)(stat_buffer+8));
			unsigned long  xfree =   (*(unsigned long *)(stat_buffer+32)) * (*(unsigned long *)(stat_buffer+8));
			

			printf("%s %s %s (%lu/%lu) %lu     \n",mi.dev,mi.path,mi.format,(xtotal-xfree),xtotal,xfree);
			state=ReadingDevice;
					}break;

		}





}






}
