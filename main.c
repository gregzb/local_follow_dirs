#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
  char * dir_name = "./";

  int total_size = 0;

  printf("Directory Name: %s", dir_name);

  DIR * dir_stream = opendir(dir_name);

  struct dirent * file;
  while ((file = readdir(dir_stream)) != NULL) {
    struct stat fileStats;
    stat(file->d_name, &fileStats);

    int str_size = 11;
    char perm_str[] = "drwxrwxrwx";

  	int i;
  	for (i = 0; i < 9; i++) {
  		if (((fileStats.st_mode >> i) & 1) == 0) {
  			perm_str[str_size-2-i] = '-';
  		}
  	}

  	if (!(fileStats.st_mode & S_IFDIR)) {
      total_size += fileStats.st_size;
  		perm_str[0] = '-';
  	}

  	printf("%s %s\n", perm_str, file->d_name);
  }

  closedir(dir_stream);

  printf("Total Size: %d GB %d MB %d KB %d B\n", total_size/1000000000%1000, total_size/1000000%1000, total_size/1000%1000, total_size%1000);

  return 0;
}
