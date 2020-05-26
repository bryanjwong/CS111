/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab3a.c
***/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "ext2_fs.h"

#define SUPERBLOCK_OFFSET 1024

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: ./lab3a [file system image]\n");
    exit(1);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "Error while attempting to open file %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  struct ext2_super_block superblock;
  int ret = pread(fd, &superblock, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
  if (ret < 0) {
    fprintf(stderr, "Error while attempting to read superblock: %s\n", strerror(errno));
  }

  fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", superblock.s_blocks_count, 
          superblock.s_inodes_count, EXT2_MIN_BLOCK_SIZE << superblock.s_log_block_size, 
          superblock.s_inode_size, superblock.s_blocks_per_group,
          superblock.s_inodes_per_group, superblock.s_first_ino);

  if (close(fd) != 0) {
    fprintf(stderr, "Error while attempting to close file %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  return 0;
}
