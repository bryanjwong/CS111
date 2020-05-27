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
#include <time.h>
#include <stdint.h>
#include "ext2_fs.h"

#define SUPERBLOCK_OFFSET 1024

/* Inode Mode Constants */
#define EXT2_S_IFREG 0x8000
#define EXT2_S_IFDIR 0x4000
#define EXT2_S_IFLNK 0xA000

unsigned int BLOCK_SIZE;

void print_dir(int fd, int block_offset, int parent_inode) {
  int FIRST_DIR_OFFSET = SUPERBLOCK_OFFSET + (block_offset-1)*BLOCK_SIZE;
  unsigned int dir_offset = 0;

  struct ext2_dir_entry dir_entry;
  do {         
    int ret = pread(fd, &dir_entry, sizeof(struct ext2_dir_entry), FIRST_DIR_OFFSET + dir_offset);
    if (ret < 0) {
      fprintf(stderr, "Error while attempting to read inode #%d's directory entry: %s\n", parent_inode, strerror(errno));
      exit(1);
    }

    if (dir_entry.inode != 0) {       
      fprintf(stdout, "DIRENT,%d,%d,%d,%d,%d,'%s'\n", parent_inode, dir_offset, dir_entry.inode,
              dir_entry.rec_len, dir_entry.name_len, dir_entry.name);
    }
    dir_offset += dir_entry.rec_len;
  } while(dir_offset < BLOCK_SIZE && dir_entry.inode != 0);
}

// TODO: Fix Logical Block Offset, Print out directory info if part inode is a dir
int search_indirect_block(int fd, int block_offset, int parent_inode, int indirection_layer, int log_offset, char isdir) {
  
  uint32_t * data = malloc(BLOCK_SIZE);
  if (data == NULL) {
    fprintf(stderr, "Error, malloc failed\n");
    exit(1);
  }
  const int INDIR_OFFSET = SUPERBLOCK_OFFSET + (block_offset-1)*BLOCK_SIZE;
  int ret = pread(fd, data, BLOCK_SIZE, INDIR_OFFSET);
  if (ret < 0) {
    fprintf(stderr, "Error while attempting to read indirect (%d) block table for inode #%d: %s\n", indirection_layer, parent_inode, strerror(errno));
    exit(1);
  }

  const unsigned int NUM_REFS = BLOCK_SIZE / sizeof(uint32_t);
  unsigned int i;
  unsigned int logical_blk_offset;
  for (i = 0; i < NUM_REFS; i++) {
    if (data[i] != 0) {  
      if (indirection_layer == 1) {
        if (isdir) {
          print_dir(fd, data[i], parent_inode);
        }
        logical_blk_offset = data[i];
      } else {
        logical_blk_offset = log_offset + i;
        search_indirect_block(fd, data[i], parent_inode, indirection_layer - 1, log_offset, isdir);
      }
      fprintf(stdout, "INDIRECT,%d,%d,%d,%d,%d\n", parent_inode, indirection_layer, logical_blk_offset, block_offset, data[i]);
    }
  }
  free(data);
  return logical_blk_offset;
}

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

  /* Superblock */
  struct ext2_super_block superblock;
  int ret = pread(fd, &superblock, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
  if (ret < 0) {
    fprintf(stderr, "Error while attempting to read superblock: %s\n", strerror(errno));
    exit(1);
  }

  if (superblock.s_magic != EXT2_SUPER_MAGIC) {
    fprintf(stderr, "Error, read block is not superblock\n");
    exit(1);
  }

  BLOCK_SIZE = EXT2_MIN_BLOCK_SIZE << superblock.s_log_block_size;

  fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", superblock.s_blocks_count, 
          superblock.s_inodes_count, BLOCK_SIZE, 
          superblock.s_inode_size, superblock.s_blocks_per_group,
          superblock.s_inodes_per_group, superblock.s_first_ino);


  /* Block Group Descriptor */
  unsigned int NUM_GROUPS = superblock.s_blocks_count / BLOCK_SIZE;
  if (superblock.s_blocks_count % BLOCK_SIZE) NUM_GROUPS++;

  const int GROUP_DESC_OFFSET = SUPERBLOCK_OFFSET + BLOCK_SIZE;
  struct ext2_group_desc group;
  unsigned int i, j, k;
  for (i = 0; i < NUM_GROUPS; i++) {
    ret = pread(fd, &group, sizeof(struct ext2_group_desc), GROUP_DESC_OFFSET + i*sizeof(struct ext2_group_desc));
    if (ret < 0) {
      fprintf(stderr, "Error while attempting to read group desc #%d: %s\n", i, strerror(errno));
      exit(1);
    }

    fprintf(stdout, "GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n", i, superblock.s_blocks_count,
            superblock.s_inodes_count, group.bg_free_blocks_count,
            group.bg_free_inodes_count, group.bg_block_bitmap,
            group.bg_inode_bitmap, group.bg_inode_table);

    /* Free Block Bitmap */
    const int BLOCK_BITMAP_OFFSET = SUPERBLOCK_OFFSET + (group.bg_block_bitmap-1)*BLOCK_SIZE;
    char byte;
    for (j = 0; j < superblock.s_blocks_count; j++) {
      if (j % 8 == 0) {
        ret = pread(fd, &byte, sizeof(char), BLOCK_BITMAP_OFFSET + j/8);
        if (ret < 0) {
          fprintf(stderr, "Error while attempting to read block bitmap for group #%d, block #%d: %s\n", i+1, j+1, strerror(errno));
          exit(1);
        }
      }
      if (!(byte & (1 << (j % 8))) )
        fprintf(stdout, "BFREE,%d\n", j+1);
    }

    /* Inode Bitmap */
    const int INODE_BITMAP_OFFSET = SUPERBLOCK_OFFSET + (group.bg_inode_bitmap-1)*BLOCK_SIZE;
    for (j = 0; j < superblock.s_inodes_count; j++) {
      if (j % 8 == 0) {
        ret = pread(fd, &byte, sizeof(char), INODE_BITMAP_OFFSET + j/8);
        if (ret < 0) {
          fprintf(stderr, "Error while attempting to read inode bitmap for group #%d, inode#%d: %s\n", i+1, j+1, strerror(errno));
          exit(1);
        }
      }
      if (!(byte & (1 << (j % 8))) )
        fprintf(stdout, "IFREE,%d\n", j+1);
    }

    /* Inode Table */
    const int INODE_TABLE_OFFSET = SUPERBLOCK_OFFSET + (group.bg_inode_table-1)*BLOCK_SIZE;

    struct ext2_inode inode;
    for (j = 0; j < superblock.s_inodes_per_group; j++) {
      ret = pread(fd, &inode, sizeof(struct ext2_inode), INODE_TABLE_OFFSET + j*sizeof(struct ext2_inode));
      if (ret < 0) {
        fprintf(stderr, "Error while attempting to read inode #%d: %s\n", j+1, strerror(errno));
        exit(1);
      }

      if (inode.i_mode != 0 && inode.i_links_count !=  0) {
        fprintf(stdout, "INODE,%d,", j+1);
        if ((inode.i_mode & 0xF000) == EXT2_S_IFREG)
          fprintf(stdout, "f,"); 
        else if ((inode.i_mode & 0xF000) == EXT2_S_IFDIR)
          fprintf(stdout, "d,");
        else if ((inode.i_mode & 0xF000) == EXT2_S_IFLNK)
          fprintf(stdout, "s,"); 
        else
          fprintf(stdout, "?,");
       
        fprintf(stdout, "%o,%d,%d,%d,", inode.i_mode & 0xFFF, inode.i_uid, inode.i_gid, inode.i_links_count);
        
        char time_string[80];
        struct tm ts;

        /* Time of last inode change */
        ts = *gmtime((time_t *)&inode.i_ctime);
        strftime(time_string, sizeof(time_string), "%m/%d/%y %H:%M:%S", &ts);
        fprintf(stdout, "%s,", time_string);
        
        /* Time of last modification */
        ts = *gmtime((time_t *)&inode.i_atime);
        strftime(time_string, sizeof(time_string), "%m/%d/%y %H:%M:%S", &ts);
        fprintf(stdout, "%s,", time_string);

        /* Time of last access */
        ts = *gmtime((time_t *)&inode.i_mtime);
        strftime(time_string, sizeof(time_string), "%m/%d/%y %H:%M:%S", &ts);
        fprintf(stdout, "%s,", time_string);

        fprintf(stdout, "%d,%d", inode.i_size, inode.i_blocks);

        /* Block Number Addresses */
        if ((inode.i_mode & 0xF000) == EXT2_S_IFREG || (inode.i_mode & 0xF000) == EXT2_S_IFDIR ||
            (((inode.i_mode & 0XF000) == EXT2_S_IFLNK) && inode.i_size > 60)) {
          for (k = 0; k < 15; k++) {
            fprintf(stdout, ",%d", inode.i_block[k]);
          }      
        }
        fprintf(stdout, "\n");

        /* Directory Entries */
        if ((inode.i_mode & 0xF000) == EXT2_S_IFDIR) {
          for (k = 0; k < 12; k++) {
            print_dir(fd, inode.i_block[k], j+1);
          }    
        }

        /* Singly, Doubly, Triply Indirect Lists */
        if (((inode.i_mode & 0xF000) == EXT2_S_IFDIR || (inode.i_mode & 0xF000) == EXT2_S_IFREG)) {
          char isdir = ((inode.i_mode & 0xF000) == EXT2_S_IFDIR);
          if (inode.i_block[12] != 0)
            search_indirect_block(fd, inode.i_block[12], j+1, 1, 12, isdir);
          if (inode.i_block[13] != 0)
            search_indirect_block(fd, inode.i_block[13], j+1, 2, 256 + 12, isdir);
          if (inode.i_block[14] != 0)
            search_indirect_block(fd, inode.i_block[14], j+1, 3, 65536 + 256 + 12, isdir);
        }
      }
    }
  }
  
  if (close(fd) != 0) {
    fprintf(stderr, "Error while attempting to close file %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  return 0;
}
