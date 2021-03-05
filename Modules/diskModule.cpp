#include "diskModule.h"
#include "../Estructs/diskStructs.h"
using namespace std;

bool mkdiskF(int size, std::string fit, std::string units, std::string path){
    try
    {
        // transformando a bytes
        if (units == "k")
        {
            size = size * 1024;
        }
        else{
            size = size * 1024 * 1024;
        }
        
        MBR tempDisk;
        tempDisk.mbr_tamano = size;
        tempDisk.mbr_fecha_creacion = time(0);
        tempDisk.mbr_disk_signature = rand()%1000000 + 1;
        
        // agregando el fit
        if (fit == "ff" || fit == "FF")
        {
            tempDisk.disk_fit = 'f';
        }
        else if (fit == "wf" || fit == "WF")
        {
            tempDisk.disk_fit = 'w';
        }
        else{
            tempDisk.disk_fit = 'b';
        }
        
        // inicializando las particiones
        for (int i = 0; i < 4; i++)
        {
            tempDisk.mbr_partition[i].part_status = '0';
            tempDisk.mbr_partition[i].part_type = '0';
            tempDisk.mbr_partition[i].part_fit = '0';
            tempDisk.mbr_partition[i].part_start = -1;
            tempDisk.mbr_partition[i].part_size = -1;
            strcpy(tempDisk.mbr_partition[i].part_name, "");
        }
        
        cout << "   ->>> " << tempDisk.mbr_tamano << " -- " << " -- " << tempDisk.mbr_disk_signature << " -- " << tempDisk.disk_fit << " -- " << asctime(gmtime(&tempDisk.mbr_fecha_creacion));
        // creando el archivo
        FILE *archivo = fopen(path.c_str(), "wb");
        fwrite("\0", 1, 1, archivo);
        fseek(archivo, size -1, SEEK_SET);
        fwrite("\0", 1, 1, archivo);

        // escribiendo el mbr en el archivo
        fseek(archivo, 0, SEEK_SET);
        fwrite(&tempDisk, sizeof(tempDisk), 1, archivo);
        fclose(archivo);
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() <<'\n';
        return false;
    }
    
}

bool rmdiskF(std::string path){
    try
    {   
        if (FILE *archivo = fopen(path.c_str(), "r"))
        {
            cout << "  Se eliminara: " << path << " Confirmar(Y/N)? ";
            string respuesta;
            cin >> respuesta;
            if (respuesta == "Y" | respuesta == "y")
            {
                remove(path.c_str());
            }
            else{
                cout << "   ------ se cancelo la eliminacion --------\n";
            }
            return true;
            
        }
        else{
            cout << "       -> El Disco No Existe Para Eliminar <-  ";
            return false;
        }
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}
