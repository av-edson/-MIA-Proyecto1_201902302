//
// Created by edson on 12/03/21.
//

#ifndef PROYECTO_FILESISTEMSTRUCTS_H
#define PROYECTO_FILESISTEMSTRUCTS_H
#include <stdio.h>
#include <time.h>

struct SUPERBLOQUE{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_firts_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
};

struct INODO{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
};
struct content{
    char b_name[12];
    int b_inodo;
};
struct CARPETABLOCK{
    content b_content[4];
};

struct FILEBLOCK{
    char b_content[64];
};

struct POINTERBLOCK{
    int b_pointers[16];
};

#endif //PROYECTO_FILESISTEMSTRUCTS_H
