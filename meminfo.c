#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>

int main(int argc,char* argv[]) {
    struct sysinfo info;
    
    if (sysinfo(&info) != 0) exit (EXIT_FAILURE); 
	 long used_ram = info.totalram - info.freeram;
        printf("Total RAM: %ld MB\n", info.totalram / (1024 * 1024));
        printf("Free RAM:  %ld MB\n", info.freeram / (1024 * 1024));
	printf("Used RAM:  %ld MB\n", used_ram / (1024 * 1024));
        printf("Shared RAM: %ld MB\n", info.sharedram / (1024 * 1024));
        printf("Buffer RAM: %ld MB\n", info.bufferram / (1024 * 1024));
        printf("Swap total: %ld MB\n", info.totalswap / (1024 * 1024));
        printf("Swap free:  %ld MB\n", info.freeswap / (1024 * 1024));
        printf("Processes:  %d\n", info.procs);
        printf("Uptime:     %ld seconds\n", info.uptime);
        

        int percent = (used_ram * 100) / info.totalram;
        printf("RAM Usage: %d%%\n", percent);
    
    exit(EXIT_SUCCESS);
}
