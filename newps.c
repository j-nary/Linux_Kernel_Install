#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int newps_tty;
int get_newps_tty(pid_t pid) {
	char path[256];
	char comm[256];
	int tty_nr;
	FILE *file;

	sprintf(path, "/proc/%d/stat", pid);
	file = fopen(path, "r");
	fscanf(file, "%*d (%255[^)]) %*c %*d %*d %*d %d", comm, &tty_nr);
	fclose(file);

	sprintf(path, "/proc/%d/stat", pid);
	file = fopen(path, "r");
	if (strcmp(comm, "newps") == 0) {
		newps_tty = tty_nr;
		return 1;
	}
	fclose(file);
	
	return 0;
}
void print_process_info(pid_t pid) {
	char path[256];
	char cmdline[256] = {0};
	char state;
	char tty_name[16];
	int tty_nr;
	unsigned long utime = 0, stime = 0;
	FILE *file;

	sprintf(path, "/proc/%d/stat", pid);
	file = fopen(path, "r");
	if (file) {
		fscanf(file, "%*d (%255[^)]) %*c %*d %*d %*d %d %*d %*d %*d %*d %*d %*d %ld %ld", &cmdline, &tty_nr, &utime, &stime);
		fclose(file);
	} else {
		return;
	}
	if (tty_nr != newps_tty) return; 
	
	snprintf(tty_name, sizeof(tty_name), "pts/%d", tty_nr%8);

	unsigned long total_second = (utime + stime) / sysconf(_SC_CLK_TCK);
	unsigned long hours = total_second / 3600;
	unsigned long minutes = (total_second % 3600) / 60;
	unsigned long seconds = total_second % 60;
	printf("%7d %-8s %02lu:%02lu:%02lu %s\n", pid, tty_name, hours, minutes, seconds, cmdline);
}

int main() {
	DIR *dir;
	struct dirent *entry;

	dir = opendir("/proc");
	if (dir == NULL) return 1;
	while((entry = readdir(dir)) != NULL) {
		int pid = atoi(entry -> d_name);
		if (pid > 0) {
			if (get_newps_tty(pid)) break;
		}
	}
	closedir(dir);

	dir = opendir("/proc");
	if (dir == NULL) return 1;
	printf("%7s %-12s %4s %s\n", "PID", "TTY", "TIME", "CMD");

	while((entry = readdir(dir)) != NULL) {
		int pid = atoi(entry -> d_name);
		if (pid > 0) print_process_info(pid);
	}

	closedir(dir);
	return 0;
}