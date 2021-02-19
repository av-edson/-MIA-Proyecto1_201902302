#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

struct argumentos
{
    int function;
    int size;
    char fit[10];
    char units[2];
    char path[200];
    char partition_type[3];
    char name_disk[100];
    int add_size;
    char disk_ides[20];
    char format[3];
    char delet[15];
};

void cleanStruct(struct argumentos *entrace, int type){
    if (type != entrace->function){
        entrace->function = -1;
        entrace->size = -1;
        strcpy(entrace->fit, "");
        strcpy(entrace->units, "");
        strcpy(entrace->path, "");
        strcpy(entrace->partition_type, "");
        strcpy(entrace->name_disk, "");
        entrace->add_size = -1;
        strcpy(entrace->disk_ides, "");
        strcpy(entrace->format, "");
        strcpy(entrace->delet, "");
        printf(" estructura limpiada ");
    }
}

void addSize(int size,struct argumentos *info, int type){
    info->function = type;
    info->size = size;
    printf(" tamano agregado--%d", info->size);
}

void addFit(char *fitt, struct argumentos *info, int type){
    info->function = type;
    strcpy(info->fit, fitt);
    printf("  fit agregado--%s", info->fit);
}

void addUnits(char *units, struct argumentos *info, int type){
    strcpy(info->units, units);
    info->function = type;
    printf("    units added--%s", info->units);
}

void addPath(char *pathh, struct argumentos *info, int type){
    strcpy(info->path, pathh);
    info->function = type;
    printf("  added path--%s", info->path);
}

void addType(char *typee, struct argumentos *info, int type){
    strcpy(info->partition_type, typee);
    info->function = type;
    printf("    prtition type added--%s", info->partition_type);
}


void addNameDisk(char *names, struct argumentos *info, int type){
    strcpy(info->name_disk, names);
    info->function = type;
    printf("    nameDisk added--%s", info->name_disk);
}

void addSizeAdd(int size_ad,struct argumentos *info, int type){
    info->add_size = size_ad;
    info->function = type;
    printf(" add size agregado--%d", info->add_size);
}

void addDiskIde(char *id, struct argumentos *info, int type){
    strcpy(info->disk_ides, id);
    info->function = type;
    printf("    id disk added--%s", info->disk_ides);
}

void addDelete(char *del, struct argumentos *info, int type){
    strcpy(info->delet, del);
    info->function = type;
    printf("    delete size added--%s", info->delet);
}

void addFormat(char format);

void showArguments(struct argumentos *info){
    printf(" -----------  struc reporter -------------\n");
    printf(" fuction-- %d", info->function);
    printf(" size-- %d", info->size);
    printf(" fit-- %s", info->fit);
    printf(" units-- %s", info->units);
    printf(" path-- %s \n", info->path);
    printf(" partition type-- %s", info->partition_type);
    printf(" name disk-- %s", info->name_disk);
    printf(" add size-- %d", info->add_size);
    printf(" disk ides-- %s", info->disk_ides);
    printf(" format type-- %s", info->format);
    printf(" delete size-- %s", info->delet);
}

