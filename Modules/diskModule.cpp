#include "diskModule.h"
#include "../Estructs/diskStructs.h"
#include <array>
using namespace std;

bool creeateExtendedPart(int _size, std::string _path, char _fit,std::string _name, int _add, std::string _delete);

bool existFile(std::string path){
    if (FILE *archivo = fopen(path.c_str(), "r"))
    {
        return true;
    }else{
        return false;
    }
}

MBR getMRBDisk(std::string path){
    FILE *archivo = fopen(path.c_str(), "r");
    MBR temp;
    fread(&temp, sizeof(temp), 1, archivo);
    return temp;
}

int getFit(std::string path, std::string fit, int partSize, std::string type);

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
        
        //cout << "   ->>> " << tempDisk.mbr_tamano << " -- " << " -- " << tempDisk.mbr_disk_signature << " -- " << tempDisk.disk_fit << " -- " << asctime(gmtime(&tempDisk.mbr_fecha_creacion));
        // creando el archivo
        char bufer[1024];
        FILE *archivo = fopen(path.c_str(), "wb");
        for (int i = 0; i < size/1024; i++)
        {
            fwrite(&bufer, sizeof(bufer), 1, archivo);
        }
        

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
        if (existFile(path))
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

bool fdiskF(int _size, std::string _units, std::string _path, std::string _type, std::string _fit,std::string _name, int _add, std::string _delete ){
    bool aux = false;
    // si exisye el disco
    if (existFile(_path))
    {
        // parametro obligatorios
        if (_size >1 && _name!= "")
        {
            char fit;
            char type;
            // transformacion a numero de bytes de la particion
            if (_units == "m") _size = _size * 1024 * 1024;
            else if (_units == "b") _size = _size;
            else _size = _size * 1024;
            // fit de la particion
            if (_fit == "ff") fit = 'f';
            else if (_fit == "bf") fit = 'b';
            else fit = 'w';
            // tipo de particion
            if (_type == "e" || _type == "E") type = 'e';
            else if(_type == "l"|| _type == "L") type = 'l';
            else type = 'p';
            
            // crear particion
            if (_add ==-1 && _delete=="")
            {
                MBR tempDisk = getMRBDisk(_path);
                bool existPatitionSpace=false;
                // ver si existe espacio disponibel dentro de las 4
                for(PARTITION part: tempDisk.mbr_partition){
                    //cout << part.part_status << "-" << part.part_name<<"-"<< part.part_start <<"-" << part.part_size << "-" << part.part_type << endl;
                    if (part.part_status == '0')
                    {
                        existPatitionSpace = true;
                        break;
                    }
                    
                }
                // si no existe
                if (!existPatitionSpace && type != 'e')
                {
                    cout << "  -- Ya se alcanzó el límite máximo de particiones -- " << endl;
                    return false;
                }

                // metadatos de la nueva particion
                PARTITION nuevaPart{};
                nuevaPart.part_fit = fit;
                strcpy(nuevaPart.part_name, _name.c_str());
                nuevaPart.part_size = _size;
                nuevaPart.part_status = '1';
                nuevaPart.part_type = type;
                // validando logicas
                if (type == 'l' || type == 'e')
                {
                    bool existeLigica = false;
                    for(PARTITION pr: tempDisk.mbr_partition){
                        if (pr.part_type == 'l')
                        {
                         existeLigica = true;
                         break;   
                        }
                    }
                    if (!existeLigica && type=='e')
                    {
                        cout << "   -- No existe particion logica para crear una extendida --" << endl;
                        return false;
                    }
                    
                    if (existeLigica && _type=="l")
                    {
                        cout << "   -- Ya existe una particion logica dentro del disco --" << endl;
                        return false;
                    }
                    
                }

                if (type == 'e'){
                    return creeateExtendedPart( _size, _path, fit, _name,  _add,  _delete);
                }
                // luego de las validaciones logicas pedimos ubicacion y validamos espacios del disco
                int lugar = getFit(_path, _fit, _size, _type);  
                nuevaPart.part_start = lugar;
                if (nuevaPart.part_start == 0) return false;
                
                for(int i=0;i<4;i++){
                    if (tempDisk.mbr_partition[i].part_status=='0')
                    {
                        tempDisk.mbr_partition[i] = nuevaPart;
                        break;
                    }
                    
                }

                //cout << " inicio part " << nuevaPart.part_start << endl;
                // ESCRIBIENDO PARTICION
                FILE *archivo = fopen(_path.c_str(), "rb+");
                // escribiendo nuevo el mbr en el archivo
                fseek(archivo, 0, SEEK_SET);
                fwrite(&tempDisk, sizeof(tempDisk), 1, archivo);
                for(PARTITION prt: tempDisk.mbr_partition){
                    if (prt.part_status=='1')
                    {
                        fseek(archivo, prt.part_start, SEEK_SET);
                        for (int i = 0; i < prt.part_size; i++)
                        {
                            fwrite("\1",1,1,archivo);
                        }
                    }
                }
                fclose(archivo);

                aux = true; 
            }
        }else{
            cout << "       -- No se enviaron los parametro obligatorios -- " << endl;
            aux = false;
        }
        
    }else{
        cout << "       --- El disco a modificar particion no existe --- " << endl;
        aux = false;
    }
    return aux;
}

EBR getEbrLogicalPartition(std::string path, int inicioExtendida){
    FILE *archivo = fopen(path.c_str(), "rb+");
    EBR temp{};
    fseek(archivo, inicioExtendida , SEEK_SET);
    fread(&temp, sizeof(temp), 1, archivo);
    return temp;
}

int getFitLogicPartition(PARTITION logica, char fit, int partSize, std::string path){
    // ---------------------- LENAMOS UNA MATRIZ CON ESPACIOS VACIOS ----------
    int matriz[6][2];
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            matriz[i][j] = 0;
        }

    }
    int indexMatriz = 0;
    int anterior=logica.part_start + 1;
    // jalamos el EBR
    EBR temp = getEbrLogicalPartition(path, logica.part_start+1);
    if (temp.part_fit != 'b' && temp.part_fit != 'f' && temp.part_fit != 'w'){
        matriz[indexMatriz][0] = anterior;
        matriz[indexMatriz][1] = partSize+anterior+1;
        anterior = matriz[indexMatriz][1];
        indexMatriz++;
    }
    while (temp.part_star != 0 && (temp.part_fit=='b' || temp.part_fit=='f' || temp.part_fit == 'w')){
        if (temp.part_status=='1' && temp.part_next==-1){
            matriz[indexMatriz][0] = temp.part_star + temp.part_size;
            matriz[indexMatriz][1] = matriz[indexMatriz][0] + partSize +1;
        }
        temp = getEbrLogicalPartition(path, temp.part_next);
    }

    if (fit == 'f'){
        for (int i = 0; i < 6; i++)
        {
            if (matriz[i][0]+partSize > logica.part_size){
                return 0;
            }
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                return matriz[i][0];
            }
        }
    }
    if (fit == 'w'){
        int grande = 0;
        for (int i = 0; i < 6; i++)
        {
            if (matriz[i][0]+partSize > logica.part_size){
                return 0;
            }
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (tamano > grande)
                {
                    grande = tamano;
                }

            }
        }
        return grande;
    }
    if (fit == 'b'){
        int mejor = 0;
        for (int i = 0; i < 6; i++)
        {
            if (matriz[i][0]+partSize > logica.part_size){
                return 0;
            }
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (i==0) mejor = tamano;
                else if(tamano < mejor){
                    mejor = tamano;
                }
            }
        }
        return mejor;
    }

    return 0;
}


int getFit(std::string path, std::string fit, int partSize, std::string type){
    MBR disk = getMRBDisk(path);
    // ---------------------- LENAMOS UNA MATRIZ CON ESPACIOS VACIOS ----------
    int matriz[6][2];
    for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                matriz[i][j] = 0;
            }
            
        }
    int numeroLista = 0;
    int anterior=sizeof(disk)+1;
    bool libre = false;
        // llenamos la matriz con los espacios vacios
    for (int i = 0; i < 4; i++)
        {   
            if (disk.mbr_partition[i].part_status == '1')
            {
                if (libre)
                {
                    matriz[numeroLista][0] =  anterior;
                    matriz[numeroLista][1] = disk.mbr_partition[i].part_start;
                    numeroLista++;
                    libre = false;
                }
                
                anterior = disk.mbr_partition[i].part_size + disk.mbr_partition[i].part_start +1;
            }else{
                libre = true;
                if (i==3)
                {
                    matriz[numeroLista][0] =  anterior;
                    matriz[numeroLista][1] = disk.mbr_tamano;
                    numeroLista++;
                }
            }
        }
    // SI SE TRATAN DE LOGICAS O PRIMARIAS ES EL MISMO PROCESO
    // -----------------------  FIST FIT --------------
    if (fit == "ff")
    {
        for (int i = 0; i < 6; i++)
        {
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                return matriz[i][0];
            }  
        }
        
    }
    if (fit =="wf")      
    {
        int grande = 0;
        for (int i = 0; i < 6; i++)
        {
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (tamano > grande)
                {
                    grande = tamano;
                }
                
            }  
        }
        return grande;
    }
    if (fit == "bf"){
        int mejor = 0;
        for (int i = 0; i < 6; i++)
        {
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (i==0) mejor = tamano;
                else if(tamano < mejor){
                    mejor = tamano;
                }
            }
        }
        return mejor;
    }
    
    cout << "       -- Espacio insuficiente en el disco --"<< endl;
    return 0;
}

bool creeateExtendedPart(int _size, std::string _path, char _fit, std::string _name, int _add, std::string _delete){
    EBR logica{};
    logica.part_status = '1';
    logica.part_fit = _fit;
    logica.part_size = _size;
    strcpy(logica.part_name, _name.c_str());
    logica.part_next = -1;

    PARTITION extendida{};
    EBR anterior{};
    for(PARTITION pat: getMRBDisk(_path).mbr_partition){
        if (pat.part_status == '1' && pat.part_type == 'l'){
            extendida = pat;
            break;
        }
    }
    if (_size > extendida.part_size){
        cout << "  - No hay espacio en el disco -"<< endl;
        return false;
    }
    anterior = getEbrLogicalPartition(_path, extendida.part_start +1);

    logica.part_star = getFitLogicPartition(extendida, _fit, _size, _path);

    bool  existeUna= false;
    while (anterior.part_star != 0 && (anterior.part_fit=='b' || anterior.part_fit=='f' || anterior.part_fit == 'w')){
        if (anterior.part_next == -1){
            anterior.part_next = logica.part_star;
            existeUna= true;
            break;
        }else{
            anterior = getEbrLogicalPartition(_path, anterior.part_next);
        }
    }
    if (!existeUna){
        anterior = logica;
    }

    if (logica.part_star != 0){
        FILE *archivo = fopen(_path.c_str(), "rb+");
        // escribiendo nuevo el mbr en el archivo
        fseek(archivo, anterior.part_star, SEEK_SET);
        fwrite(&anterior, sizeof(anterior), 1, archivo);
        EBR aux{};
        fseek(archivo, anterior.part_star, SEEK_SET);
        fread(&aux, sizeof(aux), 1, archivo);
        if (existeUna){
            fseek(archivo, logica.part_star, SEEK_SET);
            fwrite(&logica, sizeof(logica), 1, archivo);
            fseek(archivo, logica.part_star, SEEK_SET);
            fread(&aux, sizeof (aux), 1, archivo);
        }

        return true;
    }
    cout << "  - No hay espacio en el disco -"<< endl;
    return false;
}