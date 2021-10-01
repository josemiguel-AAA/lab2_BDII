#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

struct Registro
{
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;

    long next;
};


// Sequential file struct
template<typename Registro>
class SeqFile{
    private:
        string filename;
        string datfile;

        size_t validRecord;
        size_t auxRecord;
        size_t auxFactor;

    public:
        void insertAll(vector<Registro> &registros);
        void add(Registro registro);
        Registro search(string key);
        vector<Registro> rangeSearch(string begin, string end);

        void reconstructBF();
        friend bool binarySearch(string key, SeqFile<Registro>* file, long &pos);
};

bool binarySearch(string key, SeqFile<Registro>* file, long &pos) {
     fstream arc;
     arc.open(file->datfile, ios::in | ios::binary);
     Registro registro;
     long left = 0;
     long right = file->validRecord - 1;
     long mid;
     while (right >= left) {
         mid = floor((left+right)/2);
         arc.seekg(mid* sizeof(Registro));
         arc.read((char*)&registro, sizeof(Registro));
         if (key < registro.codigo || registro.next == -2){
             right = mid -1;
         } else if (key > registro.codigo) {
             left = mid + 1;
         } else {
             pos = mid;
             return true;
         }
     }
     arc.close();
     pos = right;
    return false;
}

struct compare{
    bool operator()(Registro r1, Registro r2){
        return atoi(r1.codigo) > atoi(r2.codigo);
    }
};

// ALGORITMO DE INSERCION EN BLOQUE
template <typename Registro>
void SeqFile<Registro>::insertAll(vector<Registro>& registros)
{
    //Sorting registers
    fstream file;
    file.open("salida.txt", ios::in | ios::out | ios::app);
    sort(registros.begin(), registros.end(), compare());
    for (const auto &x:registros){
        file << x << "\n";
    }
    file.close();
}

//ALGORITMO PARA AGREGAR NUEVO REGISTRO
template <typename Registro>
void SeqFile<Registro>::add(Registro registro)
{
    if (auxRecord >= auxFactor) reconstructBF();
    fstream file;
    file.open(datfile, ios::in | ios::out | ios::binary);
    long pos;
    binarySearch(registro.codigo,this,pos);

    Registro regPrev;
    if (pos < 0){
        file.seekg(0);
        file.read((char*)&regPrev, sizeof(Registro));
        registro.next = validRecord + auxRecord;
        file.seekg(0);
        file.write((char*) &registro, sizeof(Registro));
        file.seekg(0,file.end);
        file.write((char*) &regPrev, sizeof(Registro));
        auxRecord++;
        return;
    }
    //Reading previous record
    file.seekg(pos*sizeof(Registro));
    file.read((char*)&regPrev, sizeof(Registro));
    // new record will point to the next record of regPrev
    registro.next = regPrev.next;
    // regPrev will point to the last record on file
    regPrev.next = validRecord + auxRecord;

    file.seekp(0, file.end);
    file.write((char*)&regPrev, sizeof(Registro));
    file.seekp(pos*sizeof(Registro));
    file.write((char*)&regPrev, sizeof(Registro));
    auxRecord++;
}

//ALGORITMO DE BÚSQUEDA PUNTUAL
template <typename Registro>
Registro SeqFile<Registro>::search(string key){
    Registro registro;
    fstream file;
    file.open(datfile, ios::in | ios::binary);

    long pos;
    bool foundBegin = binarySearch(key,this,pos);
    if (!foundBegin){

        if(pos < 0){
            cout << "Key '" << key <<"' not found!!\n";
            return registro;
        } else { // If not found and pos>=0, read record
            file.seekg(pos* sizeof(Registro));
            file.read((char*)&registro, sizeof(Registro));
            // keep searching for key, while record belongs to aux. part
            while(registro.next >= validRecord && registro.next != -1){
                file.seekg(registro.next*sizeof(Registro));
                file.read((char*)&registro,sizeof(Registro));
                if (registro.key == key) {
                    return registro;
                }
            }
        }
        cout << "Key '" << key <<"' not found!!\n";
        return Registro();
    } else {
        // If found, return record at position
        file.seekg(pos* sizeof(Registro));
        file.read((char*)&registro,sizeof(Registro));
    }
    return registro();
}

//ALGORITMO DE BÚSQUEDA POR RANGO
template <typename Registro>
vector<Registro> SeqFile<Registro>::rangeSearch(string begin, string end)
{
    vector<Registro> regs;
    //First
    if((int)end < (int)begin){
        cout << "Key is lower than start-key.";
        return regs;
    }

    fstream file;
    file.open(datfile, ios::binary | ios::in);
    Registro registro;

    long start;
    bool found = binarySearch(begin,this, start);

    //If not found
    if(!found){
        if(start < 0){
            start = 0;
            file.seekg(0);
        } else {
            file.seekg(start* sizeof(Registro));
            file.read((char*)&registro, sizeof(registro));
            file.seekg(registro.next* sizeof(Registro));
        }
    } else {
        file.seekg(start*sizeof(Registro));
    }

    file.read((char*)&registro, sizeof(registro));
    string current = registro.codigo;

    while(current <= end && registro.next != -1) {
        regs.push_back(registro);
        file.seekg(registro.next* sizeof(Registro));
        file.read((char*)&registro, sizeof(registro));
        current = registro.codigo;
    }

    if (registro.codigo <= end) {
        regs.push_back(registro);
    }

    file.close();
    return regs;
}

template <typename Registro>
void SeqFile<Registro>::reconstructBF(){
    /*
     * RECONSTRUCT FUNCTION, NOT IMPLEMENTED (YET)
     */
};


int main(){
    return 0;
}
