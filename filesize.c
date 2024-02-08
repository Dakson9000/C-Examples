#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*
  File: filesize.c
  Author: Mason Miles
	Description: This program demonstrates three different ways to determine the size of a file.
  Usage: ./filesize filename
*/
int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Wrong number of args\n");
    exit(1);
  }

  // Solution using FILE pointer
  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    perror("File open failed");
    exit(2);
  }

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  printf("FILE pointer solution: %ld bytes\n", size);
  fclose(fp);

  // Solution using stat
  struct stat statbuf;
  if (stat(argv[1], &statbuf) != 0) {
    perror("Can't open file");
    exit(1);
  }
  size = statbuf.st_size;
  printf("stat solution: %ld bytes\n", size);

  // Solution using open and lseek
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("File open failed");
    exit(2);
  }
  size = lseek(fd, 0, SEEK_END);
  if (size == -1) {
    perror("Can't seek");
    exit(3);
  }
  printf("open and lseek solution: %ld bytes\n", size);
  close(fd);
}
