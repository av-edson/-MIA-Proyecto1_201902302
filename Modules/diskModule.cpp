#include "diskModule.h"
#include "../Estructs/diskStructs.h"
#include "../Estructs/fileSistemStructs.h"
#include <array>
using namespace std;

bool creeateLogicalPart(int _size, std::string _path, char _fit,std::string _name, int _add, std::string _delete);
bool deletePart(std::string name, std::string path , std::string opcion);

bool existFile(std::string path){
    try{
        if (FILE *archivo = fopen(path.c_str(), "rb+"))
        {
            fclose(archivo);
            return true;
        }else{
            return false;
        }
    }catch (const std::exception& e){
        cout << " conflicto al abrir archivo " << e.what() << endl;
        return false;
    }
}

MBR getMRBDisk(std::string path){
    FILE *archivo = fopen(path.c_str(), "rb+");
    MBR temp;
    fread(&temp, sizeof(temp), 1, archivo);
    fclose(archivo);
    return temp;
}

int getFit(std::string path, char fit, int partSize, std::string type);

// ------------------------------------      MKDISK
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
        FILE *archivo = fopen(path.c_str(), "w+");
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
// --------------------------------------   RMDISK
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
// ----------------------------------      FDISK
bool fdiskF(int _size, std::string _units, std::string _path, std::string _type, std::string _fit,std::string _name, int _add, std::string _delete ){
    bool aux = false;
    // si exisye el disco
    if (existFile(_path))
    {
        // parametro obligatorios
        if (_size > 0 && _name!= "")
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
                    string partName (part.part_name);
                    if (partName == _name){
                        cout << "  - Ya esxiste particion con el nombre ingresado"<< endl;
                        return false;
                    }
                }
                // si no existe
                if (!existPatitionSpace && type != 'l')
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
                    bool existeExtendida = false;
                    for(PARTITION pr: tempDisk.mbr_partition){
                        if (pr.part_type == 'e')
                        {
                         existeExtendida = true;
                         break;   
                        }
                    }
                    if (!existeExtendida && type=='l')
                    {
                        cout << "   -- No existe particion Extendida para crear una Logica --" << endl;
                        return false;
                    }
                    
                    if (existeExtendida && _type=="e")
                    {
                        cout << "   -- Ya existe una particion Extendida dentro del disco --" << endl;
                        return false;
                    }
                    
                }

                if (type == 'l'){
                    return creeateLogicalPart( _size, _path, fit, _name,  _add,  _delete);
                }
                // luego de las validaciones logicas pedimos ubicacion y validamos espacios del disco
                int lugar = getFit(_path, fit, _size, _type);
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
            else if (_delete != ""){
                return deletePart(_name,  _path , _delete);
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
    fclose(archivo);
    return temp;
}

int getFitLogicPartition(PARTITION extendida, char fit, int partSize, std::string path){
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
    int anterior=extendida.part_start + 1;
    // jalamos el EBR
    EBR temp = getEbrLogicalPartition(path, extendida.part_start+1);
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
            if (matriz[i][0]+partSize > extendida.part_start + extendida.part_size){
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
        int grande = 0; int res=0;
        for (int i = 0; i < 6; i++)
        {
            if (matriz[i][0]+partSize > extendida.part_size){
                return 0;
            }
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (tamano > grande)
                {
                    grande = tamano;
                    res = matriz[i][0];
                }

            }
        }
        return res;
    }
    if (fit == 'b'){
        int mejor = 0;
        for (int i = 0; i < 6; i++)
        {
            if (matriz[i][0]+partSize > extendida.part_size){
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


int getFit(std::string path, char fit, int partSize, std::string type){
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
            }
            if (i==3)
            {
                matriz[numeroLista][0] =  anterior;
                matriz[numeroLista][1] = disk.mbr_tamano;
                numeroLista++;
            }
        }
    // SI SE TRATAN DE LOGICAS O PRIMARIAS ES EL MISMO PROCESO
    // -----------------------  FIST FIT --------------
    if (fit == 'f')
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
    if (fit =='w')
    {
        int grande = 0;
        int resultado=0;
        for (int i = 0; i < 6; i++)
        {
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (tamano > grande)
                {
                    grande = tamano;
                    resultado = matriz[i][0];
                }
                
            }  
        }
        return resultado;
    }
    if (fit == 'b'){
        int mejor = 0; int resultado;
        for (int i = 0; i < 6; i++)
        {
            int tamano = matriz[i][1] - matriz[i][0];
            if (tamano > partSize)
            {
                if (i==0) {
                    mejor = tamano;
                    resultado = matriz[i][0];
                }
                else if(tamano < mejor){
                    mejor = tamano;
                    resultado = matriz[i][0];
                }
            }
        }
        return resultado;
    }
    
    cout << "       -- Espacio insuficiente en el disco --"<< endl;
    return 0;
}

bool creeateLogicalPart(int _size, std::string _path, char _fit, std::string _name, int _add, std::string _delete){
    EBR logica{};
    logica.part_status = '1';
    logica.part_fit = _fit;
    logica.part_size = _size;
    strcpy(logica.part_name, _name.c_str());
    logica.part_next = -1;

    PARTITION extendida{};
    EBR anterior{};
    for(PARTITION pat: getMRBDisk(_path).mbr_partition){
        if (pat.part_status == '1' && pat.part_type == 'e'){
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
        fclose(archivo);
        return true;
    }
    cout << "  - No hay espacio en el disco -"<< endl;
    return false;
}

// -----------------------------          FDISK - DELETE
bool deletePart(std::string name, std::string path , std::string opcion){
    MBR disk = getMRBDisk(path);
     bool encontrado=false;
    // buscando la particion
    for(int i=0; i<4; i++){
        string partName(disk.mbr_partition[i].part_name);
        if (partName == name){
            FILE *archivo = fopen(path.c_str(), "rb+");
            disk.mbr_partition[i].part_status = '0';
            fseek(archivo, 0, SEEK_SET);
            fwrite(&disk, sizeof(disk), 1, archivo);
            if (opcion=="full"){
                fseek(archivo, disk.mbr_partition[i].part_start, SEEK_SET);
                for (int i = 0; i < disk.mbr_partition[i].part_size; ++i) {
                    fwrite("\1", 1,1,archivo);
                }
            }
            fclose(archivo);
            return true;
        }
    }
    cout << " No Existe la particion que decea eliminar"<< endl;
    return false;
}

// ----------------------------------             MONTAR ------------------------
bool montar(std::string path,std::string namePart,list<montadas> *listaMontadas){
    for (montadas mt: *listaMontadas){
        if (mt.getName() == namePart){
            cout << "   - Ya se monto la particion antes-"<< endl;
            return false;
        }
    }
    montadas aux;
    aux.savePartition(path, listaMontadas, namePart);
    MBR tempDisk = getMRBDisk(path); bool encontrado=false;
    // buscammos el nombre de la particion
    for(PARTITION part: tempDisk.mbr_partition){
        string tempName = part.part_name;
        if (tempName == namePart && part.part_status == '1'){
            encontrado = true;
            break;
        }
    }
    if (!encontrado){
        cout << "   - No existe la particion que decea montar-";
        listaMontadas->pop_back();
        return false;
    }
    // falta agregarle las clases necesarias para montar el sistema de archivos
    return true;
}

bool desmontar(std::string idPart,list<montadas> *listaMontadas){
    list<montadas>::const_iterator index = listaMontadas->cbegin();
    for (montadas mt: *listaMontadas){
        list<montadas>::const_iterator actual = index++;
        if (mt.getId() == idPart){
            listaMontadas->erase(actual);
            return true;
        }
    }
    cout << "       - No se encontro " << endl;
    return false;
}

// -------------------------------          MKFS ----------------------------
int getNoInodos(int tamanoDisco){
    return (tamanoDisco-80)/309;
}

bool makefilesystem(string id, string type, string tipo, list<montadas> *listaMontadas){
    bool encontrado=false;
    montadas montada; int tipoSistema;
    for (montadas mt: *listaMontadas){
        if (mt.getId() == id){
            encontrado = true;
            montada = mt;
            break;
        }
    }
    if (!encontrado) {
        cout << "   - No existe pariticon montada con el id ingresado-" << endl;
        return false;
    }
    if (tipo.empty()) tipo = "2fs";
    if (type.empty()) type="full";
    if (tipo=="3fs"){
        tipoSistema = 3;
    }else{
        tipoSistema = 2;
    }
    MBR tempDisk = getMRBDisk(montada.getPath());
    int noInodos = getNoInodos(tempDisk.mbr_tamano);
    int noBloques = noInodos*3;
    //int total = noInodos + noBloques + sizeof(INODO)*noInodos + sizeof(FILEBLOCK)*noBloques + sizeof(tempDisk)+5+
            //sizeof(SUPERBLOQUE);
    //int res = tempDisk.mbr_tamano - total;
    int inicioTodo,inicioAnterior;
    for (PARTITION part: tempDisk.mbr_partition){
        if (part.part_name == montada.getName()){
            inicioAnterior = part.part_start+2+sizeof(SUPERBLOQUE);
            inicioTodo = part.part_start+1;
            break;
        }
    }
    // llenando datos del superbloque
    SUPERBLOQUE sb{};
    sb.s_filesystem_type = tipoSistema;
    sb.s_free_inodes_count = noInodos;
    sb.s_inodes_count = noInodos;
    sb.s_blocks_count = noInodos*3;
    sb.s_free_inodes_count = noInodos;
    sb.s_free_blocks_count = noInodos*3;
    sb.s_mtime = time(0);
    sb.s_umtime= time(0);
    sb.s_mnt_count = 1;
    sb.s_magic = 0xEF53;
    sb.s_inode_size = sizeof(INODO);
    sb.s_block_size = sizeof(FILEBLOCK);
    sb.s_first_blo = 0;
    sb.s_firts_ino = 0;
    sb.s_bm_inode_start = inicioAnterior;
    inicioAnterior += noInodos+1;
    sb.s_bm_block_start = inicioAnterior;
    inicioAnterior += noBloques+1;
    sb.s_inode_start = inicioAnterior;
    inicioAnterior += sizeof(INODO)*noInodos +1;
    sb.s_block_start = inicioAnterior;
    inicioAnterior += sizeof(FILEBLOCK)*noBloques +1;

    //-- creando inodo y bloqueCarpeta de la raiz
    // INODO
    INODO raiz{};
    raiz.i_uid = 1;
    raiz.i_gid = 1;
    raiz.i_size = 0;
    raiz.i_mtime = time(0);;
    raiz.i_atime = time(0);
    raiz.i_ctime = time(0);
    raiz.i_type = 0;
    raiz.i_perm = 664;
    for (int i = 0; i < 15; ++i) {
        if (i==0){
            raiz.i_block[i] = sb.s_first_blo;
        }else{
            raiz.i_block[i] = -1;
        }
    }
    sb.s_firts_ino+=1;
    // BLOQUE
    CARPETABLOCK bloqueRaiz{};
    sb.s_first_blo+=1;
    //-- creando archivo users.txt
    INODO inodoArchivo{};
    inodoArchivo.i_uid = 1;
    inodoArchivo.i_gid = 1;
    inodoArchivo.i_size = 0;
    inodoArchivo.i_mtime = time(0);;
    inodoArchivo.i_atime = time(0);
    inodoArchivo.i_ctime = time(0);
    inodoArchivo.i_type = 1;
    inodoArchivo.i_perm = 644;
    for (int i = 0; i < 15; ++i) {
        if (i==0){
            inodoArchivo.i_block[i] = sb.s_first_blo;
        }else{
            inodoArchivo.i_block[i] = -1;
        }
    }
    sb.s_firts_ino+=1;
    // bloque contenido
    FILEBLOCK usuarios{};
    strcpy(usuarios.b_content, "1,G,root\n1,U,root,root,123\n");
    sb.s_first_blo+=1;
    // actualizando bloque del inodo raiz
    bloqueRaiz.b_content[0].b_inodo = sb.s_firts_ino-1;
    strcpy(bloqueRaiz.b_content[0].b_name, "users.txt");
    //actualizando sb
    sb.s_free_blocks_count = sb.s_blocks_count - sb.s_first_blo;
    sb.s_free_inodes_count = sb.s_inodes_count-sb.s_firts_ino;

    // -------------------- ESCRIBIENDO EN EL ARCHIVO ----------------
    FILE *archivo = fopen(montada.getPath().c_str(), "rb+");
    // escribienso superbloque
    fseek(archivo, inicioTodo, SEEK_SET);
    fwrite(&sb, sizeof(sb), 1, archivo);
    inicioAnterior = inicioTodo + sizeof(SUPERBLOQUE)+1;
    fseek(archivo, inicioAnterior, SEEK_SET);
    // escribiento los bitmaps
    for (int i = 0; i < noInodos; ++i) {
        fwrite("0",1,1,archivo);
    }
    inicioAnterior+=noInodos+1;
    fseek(archivo, inicioAnterior, SEEK_SET);
    for (int i = 0; i < noBloques; ++i) {
        fwrite("0",1,1,archivo);
    }
    
    // actualizando bitmabs
    fseek(archivo, sb.s_bm_inode_start, SEEK_SET);
    for (int i = 0; i < sb.s_firts_ino; ++i) {
        fwrite("1", 1,1, archivo);
    }
    fseek(archivo, sb.s_bm_block_start, SEEK_SET);
    for (int i = 0; i < sb.s_first_blo; ++i) {
        fwrite("1", 1,1, archivo);
    }
    // escribiendo inodos
    fseek(archivo, sb.s_inode_start, SEEK_SET);
    fwrite(&raiz, sizeof(raiz), 1, archivo);
    fwrite(&inodoArchivo, sizeof(inodoArchivo),1,archivo);
    // escribiendo bloques
    fseek(archivo, sb.s_block_start, SEEK_SET);
    fwrite(&bloqueRaiz, sizeof(bloqueRaiz), 1, archivo);
    fwrite(&usuarios, sizeof(usuarios), 1, archivo);

    CARPETABLOCK aux{};
    fseek(archivo, sb.s_block_start, SEEK_SET);
    fread(&aux, sizeof(aux), 1, archivo);
    return true;
}
