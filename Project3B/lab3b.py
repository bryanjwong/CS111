#!/usr/bin/python3

"""
NAME: Bryan Wong
EMAIL: bryanjwong@g.ucla.edu
ID: 805111517

lab3b.py
"""

import sys
import csv

def process_superblock(superblock, group):
    num_inodes = int(superblock[2])
    last_block = int(superblock[1])
    inodes_per_block = int(superblock[3])/int(superblock[4])
    first_block = int(group[8]) + int(superblock[6])/inodes_per_block
    return num_inodes, int(first_block), last_block

def check_block(bfree, visited, first_block, last_block, bnum, data, depth):
    type = ""
    offset = 0
    if data[0] == "INDIRECT":
        offset = data[3]
        if data[2] == "2":
            type = "DOUBLE INDIRECT "
        elif data[2] == "3":
            type = "TRIPLE INDIRECT "
        else:
            type = "INDIRECT "
    else:
        if depth == 1:
            type = "INDIRECT "
            offset = 12
        elif depth == 2:
            type = "DOUBLE INDIRECT "
            offset = 268
        elif depth == 3:
            type = "TRIPLE INDIRECT "
            offset = 65804
        else:
            type = ""

    if bnum == 0:
        return
    elif bnum < 0 or bnum > last_block:
        print("INVALID {}BLOCK {} IN INODE {} AT OFFSET {}".format(type, bnum, data[1], offset))
    elif bnum < first_block or bnum == 64:
        print("RESERVED {}BLOCK {} IN INODE {} AT OFFSET {}".format(type, bnum, data[1], offset))
    elif bfree.get(bnum):
        print("ALLOCATED {}BLOCK {} ON FREELIST".format(type, bnum))
    else:
        visited[bnum].append([depth, int(data[1]), offset])

    return

def main():
    if len(sys.argv) != 2:
        sys.stderr.write("Error, expected 2 arguments but received {}\n".format(len(sys.argv)))
        sys.exit(1)

    ifree = {}
    bfree = {}
    inode = {}
    indirect = {}
    dirent = {}
    superblock = None
    group = None
    num_inodes = -1
    first_block = -1
    last_block = -1
    inconsistency_flag = False

    try:
        with open(sys.argv[1]) as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            for row in reader:
                if len(row) == 0:   # Blank Rows
                    continue
                elif row[0] == "SUPERBLOCK":
                    superblock = row
                elif row[0] == "GROUP":
                    group = row
                elif row[0] == "IFREE":
                    ifree.update({int(row[1]) : True})
                elif row[0] == "BFREE":
                    bfree.update({int(row[1]) : True})
                elif row[0] == "INODE":
                    inode.update({int(row[1]) : row})
                elif row[0] == "INDIRECT":
                    if indirect.get(int(row[1])):
                        indirect[(int(row[1]))].append(row)
                    else:
                        indirect.update({int(row[1]) : [row]})
                elif row[0] == "DIRENT":
                    if dirent.get(int(row[3])):
                        dirent[(int(row[3]))].append(row)
                    else:
                        dirent.update({int(row[3]) : [row]})
            num_inodes, first_block, last_block = process_superblock(superblock, group)

    except IOError:
        sys.stderr.write("Failed to open file '{}'\n".format(sys.argv[1]))
        sys.exit(1)

    visited = [[] for j in range(last_block)]
    link_count = [0] * num_inodes

    # Inode Inconsistencies
    for i in range(num_inodes):
        inode_alloc = inode.get(i)
        inode_free = ifree.get(i)

        # Used and free (i_mode != 0 and inode # is free on the bitmap)
        if inode_alloc and inode_free:
            print("ALLOCATED INODE {} ON FREELIST".format(i))
            inconsistency_flag = True

        # Unallocated and in use (no dedicated line in csv but in use on bitmap)
        if not inode_alloc and not inode_free and i > 10:
            print("UNALLOCATED INODE {} NOT ON FREELIST".format(i))
            inconsistency_flag = True

        if inode_alloc:
            link_count[i] = int(inode_alloc[6])

            # Scan blocks
            if (inode_alloc[2] != 's' or int(inode_alloc[10]) > 60):
                for j in range(12, 27):
                    if j > 23:
                        check_block(bfree, visited, first_block, last_block, int(inode_alloc[j]), inode_alloc, j-23)
                    else:
                        check_block(bfree, visited, first_block, last_block, int(inode_alloc[j]), inode_alloc, 0)

    # Indirect Block Inconsistencies
    for bnum in indirect:
        for entry in indirect[bnum]:
            check_block(bfree, visited, first_block, last_block, int(entry[5]), entry, int(entry[2]))

    # Check for duplicate and unreferenced blocks
    for i in range(first_block, last_block):
        block_free = bfree.get(i)
        if len(visited[i]) == 0 and not block_free:
            print("UNREFERENCED BLOCK {}".format(i))
            inconsistency_flag = True
        elif len(visited[i]) > 1:
            for duplicate in visited[i]:
                type = ""
                if duplicate[0] == 1:
                    type = "INDIRECT "
                elif duplicate[0] == 2:
                    type = "DOUBLE INDIRECT "
                elif duplicate[0] == 3:
                    type = "TRIPLE INDIRECT "
                print("DUPLICATE {}BLOCK {} IN INODE {} AT OFFSET {}".format(type, i, duplicate[1], duplicate[2]))
                inconsistency_flag = True

    dir_link_count = [0] * num_inodes
    dir_parent = [None] * num_inodes
    dir_parent_check = {}
    inode_names = [None] * num_inodes

    # Directory Inconsistencies
    for inode_num in dirent:
        for entry in dirent[inode_num]:
            parent_inode = int(entry[1])
            inode_name = entry[6]
            inode_names[inode_num] = inode_name
            if inode_num < 1 or inode_num > num_inodes:
                print("DIRECTORY INODE {} NAME {} INVALID INODE {}".format(parent_inode, inode_name, inode_num))
                inconsistency_flag = True
                continue
            elif inode_name == "'.'" and parent_inode != inode_num:
                print("DIRECTORY INODE {} NAME {} LINK TO INODE {} SHOULD BE {}".format(parent_inode, inode_name, inode_num, parent_inode))
                inconsistency_flag = True
            elif inode_name == "'..'":
                dir_parent_check[parent_inode] = inode_num
            elif inode_name != "'.'":
                dir_parent[inode_num] = parent_inode

            dir_link_count[inode_num] = dir_link_count[inode_num] + 1

    # Inconsistent Link Count
    for i in range(num_inodes):
        if dir_link_count[i] != link_count[i] and ifree.get(i) == None:
            print("INODE {} HAS {} LINKS BUT LINKCOUNT IS {}".format(i, dir_link_count[i], link_count[i]))
            inconsistency_flag = True
        if dir_parent[i] and ifree.get(i):
            print(i)
            print("DIRECTORY INODE {} NAME {} UNALLOCATED INODE {}".format(dir_parent[i], inode_names[i], i))
            inconsistency_flag = True

    for parent_inode_num in dir_parent_check:
        if parent_inode_num == 2:
            if int(dir_parent_check[parent_inode_num]) != 2: # Root directory edge case
                print("DIRECTORY INODE 2 NAME '..' LINK TO INODE {} SHOULD BE 2".format(dir_parent_check[parent_inode_num]))
                inconsistency_flag = True
        elif dir_parent_check[parent_inode_num] != dir_parent[parent_inode_num]:
            print("DIRECTORY INODE {} NAME '..' LINK TO INODE {} SHOULD BE {}".format(parent_inode_num, parent_inode_num, dir_parent[parent_inode_num]))
            inconsistency_flag = True
        elif dir_parent[parent_inode_num] == None:
            print("DIRECTORY INODE {} NAME '..' LINK TO INODE {} SHOULD BE {}".format(dir_parent_check[parent_inode_num], parent_inode_num, dir_parent_check[parent_inode_num]))
            inconsistency_flag = True

    if inconsistency_flag:
        exit(2)
    else:
        exit(1)

if __name__ == "__main__":
    main()
