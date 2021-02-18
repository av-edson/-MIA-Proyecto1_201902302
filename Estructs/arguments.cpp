#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct argumentos
{
    int function;
    int size;
    char fit[12];
    char units[2];
    char path[200];
    char partition_type[3];
    char delete[50];
    char name_disk[100];
    int add_size;
    char disk_ides[70];
    char format[3];
};

void cleanStruct(struct argumentos *entrace, int type){
    if (type != entrace->function){
        entrace->function = -1;
        entrace->size = -1;
        strcpy(entrace->fit, "");
        strcpy(entrace->units, "");
        strcpy(entrace->path, "");
        strcpy(entrace->partition_type, "");
        strcpy(entrace->delete, "");
        strcpy(entrace->name_disk, "");
        entrace->add_size = -1;
        strcpy(entrace->disk_ides, "");
        strcpy(entrace->format, "");
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

void addType(char type);

void addDeleteSize(int size);

void addNameDisk(char name);

void addSizeAdd(int size);

void addDiskIde(char ide);

void addFormat(char format);

void showArguments(struct argumentos *info){
    printf(" -----------  struc reporter -------------\n");
    printf(" fuction-- %d", info->function);
    printf(" size-- %d", info->size);
    printf(" fit-- %s", info->fit);
    printf(" units-- %s", info->units);
    printf(" path-- %s \n", info->path);
    printf(" partition type-- %s", info->partition_type);
    printf(" delete size-- %s", info->delete);
    printf(" name disk-- %s", info->name_disk);
    printf(" add size-- %d", info->add_size);
    printf(" disk ides-- %s", info->disk_ides);
    printf(" format type-- %s", info->format);
    printf("\n");
}