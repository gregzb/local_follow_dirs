#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <string.h>

#define MAX_DIR_SIZE 2048

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

  if (dir_stream == NULL) {
    printf("Could not open %s\n", dir_name);
    printf("Error: %s\n", strerror(errno));
    return -1;
  }

  struct dirent * file;
  while ((file = readdir(dir_stream)) != NULL) {
    struct stat fileStats;
    char temp[MAX_DIR_SIZE];
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
        // char new_path[MAX_DIR_SIZE];
        // strcpy(new_path, dir_name);
        // strcat(new_path, file->d_name);
        strcat(temp, "/");

        total_size += traverse_dir(temp, level + 1);
      }
  	}
  }

  closedir(dir_stream);
  return total_size;
}

int main(int argc, char ** argv) {
  int max_dir_size = MAX_DIR_SIZE;
  char dir_name[MAX_DIR_SIZE];

  if (argc < 2) {
    printf("No directory was found from the command line options.\n");
    printf("Please enter one here: \n");
    fgets(dir_name, MAX_DIR_SIZE, stdin);
  } else {
    strncpy(dir_name, argv[1], MAX_DIR_SIZE);
  }

  int dir_len = strlen(dir_name);

  if (dir_len <= 1) {
    printf("Input directory cannot be empty.\n");
    return 1;
  }

  // https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
  if (dir_name[dir_len-1] == '\n') {
    dir_name[dir_len-1] = '\0';
  }

  if (dir_name[dir_len-2] != '/') {
    dir_name[dir_len-1] = '/';
  }



  printf("Root Directory Name: %s\n\n", dir_name);

  int total_size = traverse_dir(dir_name, 0);

  if (total_size < 0) {
    return 2;
  }

  printf("Total Size: ");
  print_size(total_size);
  printf("\n");

  return 0;
}
