#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

unsigned long randstate = 1;
unsigned int
rand()
{
  randstate = randstate * 1664525 + 1013904223;
  return randstate;
}

void 
durstenfeld_shuffle(int *arr, int n) {
  int end = n - 1;
  int candidate;
  int temp;
  while(end > 0) {
    candidate = rand() % n;
    temp = arr[end];
    arr[end] = arr[candidate];
    arr[candidate] = temp;
    end -= 1;
  }
}


int
main(int argc, char *argv[]) {

  int fp, fo;
  int i, n;
  int chunk_size;
  int random_order[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
  int bytes_per_section[10] = {0,};
  int prefix_sum[10] = {10, };
  char *buffer = 0;
  if (argc != 2) {
    printf(1, "usage: ./lseek1 <filename>\n");
    exit();
  }

  if ((fp = open(argv[1], O_RDONLY)) < 0) {
    printf(1, "unable to open file %s\n", argv[1]);
    exit();
  }

  if ((fo = open("copy.txt", O_WRONLY | O_CREATE)) < 0) {
    printf(1, "unable to open output file\n");
    exit();
  }

  // produce random ordering
  durstenfeld_shuffle(random_order, 10);

  n = lseek(fp, SEEK_END, 0);
  lseek(fp, SEEK_SET, 0);

  printf(1, "n: %d\n", n);

  for (i = 0; i < 10; i++)
    bytes_per_section[i] = n / 10;

  // chunks won't be equal always
  for (i = 0; i < n % 10; i++) 
    bytes_per_section[i] += 1;

  prefix_sum[0] = 0;
  for (i = 1; i < 10; i++)
    prefix_sum[i] = prefix_sum[i - 1] + bytes_per_section[i - 1];
  

  chunk_size = (n + 9) / 10;
  buffer = (char *)malloc(sizeof(char) * chunk_size);

  for(i = 0; i < 10; i++) {
    lseek(fp, SEEK_SET, prefix_sum[random_order[i]]);
    read(fp, buffer, bytes_per_section[random_order[i]]);
    write(fo, buffer, bytes_per_section[random_order[i]]);
  }
  close(fp);
  close(fo);
  exit();
}


