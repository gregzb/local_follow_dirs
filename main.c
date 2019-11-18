#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>

void print_size(int size) {
  double f_size = size;
  char * size_names[] = {"B", "KB", "MB", "GB"};
  int num_names = 4;
  int i = 0;
  while (f_size >= 1000 && i < num_names) {
    f_size /= 1000.0;
    i++;
  }

  printf("%lf %s", f_size, size_names[i]);
}

int traverse_dir(char * dir_name, int level) {
  int total_size = 0;
  DIR * dir_stream = opendir(dir_name);

  struct dirent * file;
  while ((file = readdir(dir_stream)) != NULL) {
    struct stat fileStats;
    char temp[1024];
    strcpy(temp, dir_name);
    strcat(temp, file->d_name);
    stat(temp, &fileStats);

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

    int cnt;
    for (cnt = 0; cnt < level; cnt++) {
      printf("    ");
    }

  	printf("%s %s\n", perm_str, file->d_name);

    if ((fileStats.st_mode & S_IFDIR)) {
      if (strcmp(".", file->d_name) != 0 && strcmp("..", file->d_name) != 0) {
        char new_path[1024];
        strcpy(new_path, dir_name);
        strcat(new_path, file->d_name);
        strcat(new_path, "/");

        total_size += traverse_dir(new_path, level + 1);
      }
  	}
  }

  closedir(dir_stream);
  return total_size;
}

int main() {
  char * dir_name = "./";

  printf("Directory Name: %s\n", dir_name);

  int total_size = traverse_dir(dir_name, 0);

  printf("Total Size: ");
  print_size(total_size);
  printf("\n");

  return 0;
}
