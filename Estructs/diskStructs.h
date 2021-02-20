#ifndef _DISK_STRUCTS
#define _DISK_STRUCTS

#include <stdio.h>
#include <time.h>

struct EBR
{
    char part_status;
    char part_fit;
    int part_star;
    int part_size;
    int part_next;
    char part_name[16];
};

struct PARTITION
{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
};

struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    PARTITION mbr_partition[4];
};




#endif