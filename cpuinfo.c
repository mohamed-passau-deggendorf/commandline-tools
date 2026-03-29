#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    long configured_cpus = sysconf(_SC_NPROCESSORS_CONF);
    long online_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    long clock_ticks = sysconf(_SC_CLK_TCK);
    long page_size = sysconf(_SC_PAGESIZE);
    
    printf("=== PROCESSOR INFORMATION ===\n");
    printf("Configured CPUs: %ld\n", configured_cpus);
    printf("Online CPUs:     %ld\n", online_cpus);
    printf("Clock ticks/sec: %ld\n", clock_ticks);
    printf("Page size:       %ld bytes\n", page_size);
    printf("\n");
    
    long phys_pages = sysconf(_SC_PHYS_PAGES);
    long avail_pages = sysconf(_SC_AVPHYS_PAGES);
    
    printf("=== MEMORY INFORMATION ===\n");
    printf("Total RAM: %.2f GB\n", 
           (phys_pages * page_size) / (1073741824.0));
    printf("Available RAM: %.2f GB\n", 
           (avail_pages * page_size) / (1073741824.0));
    
    if (phys_pages > 0)  printf("RAM usage: %.1f%%\n", 100.0 * (1.0 - (double)avail_pages / phys_pages));
    
    printf("\n");
    



    printf("=== CACHE INFORMATION ===\n");
    
#ifdef _SC_LEVEL1_ICACHE_SIZE
    long l1i = sysconf(_SC_LEVEL1_ICACHE_SIZE);
    long l1d = sysconf(_SC_LEVEL1_DCACHE_SIZE);
    printf("L1 Instruction Cache: %s\n", l1i > 0 ? "yes" : "unknown");
    printf("L1 Data Cache:        %s\n", l1d > 0 ? "yes" : "unknown");
#endif
    
#ifdef _SC_LEVEL2_CACHE_SIZE
    long l2 = sysconf(_SC_LEVEL2_CACHE_SIZE);
    printf("L2 Cache: %s\n", l2 > 0 ? "yes" : "unknown");
#endif
    
#ifdef _SC_LEVEL3_CACHE_SIZE
    long l3 = sysconf(_SC_LEVEL3_CACHE_SIZE);
    printf("L3 Cache: %s\n", l3 > 0 ? "yes" : "unknown");
#endif
    printf("\n");
    


    printf("=== CPU CORE Information ===\n");
  int k;
    char path[256]="/sys/devices/system/cpu/cpu";
	path[28]='\0';	
   for(k=0;k<online_cpus;k++) {
	path[27]=k+48;
	printf("CPU CORE %d\n",k);
	int cpu_fd =  open(path, O_RDONLY | O_DIRECTORY);
	int min_freq_fd = openat(cpu_fd,"cpufreq/scaling_min_freq",O_RDONLY);

	int max_freq_fd = openat(cpu_fd,"cpufreq/scaling_max_freq",O_RDONLY);
	
	int bytes_read;
	 char buffer[32];
	 bytes_read = read(min_freq_fd, buffer, sizeof(buffer) - 1);
	 buffer[bytes_read - 1] = '\0';
        printf("min_freq: %.2f GHz -- ", (double)atoi(buffer)/1000000.0);

	bytes_read = read(max_freq_fd, buffer, sizeof(buffer) - 1);
	 buffer[bytes_read - 1] = '\0';
        printf("max_freq: %.2f GHz\n", (double)atoi(buffer)/1000000.0);
	printf("\n");
	
	
	}



    printf("=== SYSTEM LIMITS ===\n");
    printf("Max PID:          %ld\n", sysconf(_SC_CHILD_MAX));
    printf("Open files max:   %ld\n", sysconf(_SC_OPEN_MAX));
    printf("Login name max:   %ld\n", sysconf(_SC_LOGIN_NAME_MAX));
    printf("Host name max:    %ld\n", sysconf(_SC_HOST_NAME_MAX));
    printf("Streams max:      %ld\n", sysconf(_SC_STREAM_MAX));
    printf("\n");
    
  
 
	      printf("=== OTHER SYSCONF VALUES ===\n");
        printf("_SC_ARG_MAX:        %ld\n", sysconf(_SC_ARG_MAX));
        printf("_SC_NGROUPS_MAX:    %ld\n", sysconf(_SC_NGROUPS_MAX));
        printf("_SC_SIGQUEUE_MAX:   %ld\n", sysconf(_SC_SIGQUEUE_MAX));
        printf("_SC_RTSIG_MAX:      %ld\n", sysconf(_SC_RTSIG_MAX));
        printf("_SC_SEM_NSEMS_MAX:  %ld\n", sysconf(_SC_SEM_NSEMS_MAX));
        printf("_SC_SEM_VALUE_MAX:  %ld\n", sysconf(_SC_SEM_VALUE_MAX));
        printf("_SC_TIMER_MAX:      %ld\n", sysconf(_SC_TIMER_MAX));
        printf("_SC_MQ_OPEN_MAX:    %ld\n", sysconf(_SC_MQ_OPEN_MAX));
        printf("_SC_MQ_PRIO_MAX:    %ld\n", sysconf(_SC_MQ_PRIO_MAX));
        printf("_SC_VERSION:        %ld\n", sysconf(_SC_VERSION));
        printf("\n");


}
