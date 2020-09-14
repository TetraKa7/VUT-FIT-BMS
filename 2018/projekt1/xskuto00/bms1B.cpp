/*
 * 1. projekt do predmetu BMS - 4 ASK demodulator
 * Sara Skutova, xskuto00@stud.fit.vutbr.cz
 * 2018/2019
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <iomanip> 

#include "sndfile.hh"

//frekvenci nosne i amplitudu mame danou
#define AMPLITUDE (1.0 * 0x7F000000)
#define FREQ 1000.0

using namespace std;

/**
 * Trida ktera bude uchovavat maximalni hodnuty (presneji hodtoty i s odchylkou) 
 * vzorku pro dany symbol.
 */
class MaxValue {
    
public:
    MaxValue(int sampleRate, int vzorku);
    int getMaxValue(int sampleRate, int vzorku, double dvojice, int pocatek);
    int percent(int number);
    int getV00();
    int getVM01();
    int getVM10();
    int getVM11();
    int getVV01();
    int getVV10();
    int getVV11();
private:
    int v00;
    int vM01;
    int vM10;
    int vM11;
    int vV01;
    int vV10;
    int vV11;
};

    /**
     * Kontruktor
     */
    MaxValue::MaxValue(int sampleRate, int vzorku) {
        
        v00 = 0;
        
        int v01 = this->getMaxValue(sampleRate, vzorku, 1.0/3.0, vzorku);
        int procento = percent(v01);
        vM01 = v01 - procento;
        vV01 = v01 + procento;
        
        int v10 = this->getMaxValue(sampleRate, vzorku, 2.0/3.0, vzorku*2);
        procento = percent(v10);
        vM10 = v10 - procento;
        vV10 = v10 + procento;
        
        int v11 = this->getMaxValue(sampleRate, vzorku, 3.0/3.0, vzorku*3);
        procento = percent(v11);
        vM11 = v11 - procento;
        vV11 = v11 + procento;
    }
    
    /**
     * Metoda vypocita pro dane cislo jeho jedno procentu.
     * Bude potreba pro vypocet mozneho rozsahu pro dany maximalni vzorek,
     * nasledne se urci jaky symbol to tak muze byt.
     * 
     * @param number Cislo pro ktere se pocita jedno procento
     * @return  Vypocitane cislo
     */
    int MaxValue::percent(int number){
        
        return number / 100;
    }
    
    /**
     * Metoda pocita sinus signal a vraci maximalni hodnotu vzorku
     * 
     * @param sampleRate Samplerate signalu
     * @param vzorku Pocet vzorku na jednu dvojici
     * @param dvojice deleni amplitudy
     * @param pocatek kde skoncil predchazejici symbol
     * @return maximalni hodnota vzorku
     */
    int MaxValue::getMaxValue(int sampleRate, int vzorku, double dvojice, int pocatek) {
        
        int max = 0;
        double amp = dvojice * AMPLITUDE;
        double frek = FREQ/sampleRate;
        int pomoc;
        
        
        for (int i = pocatek; i < vzorku + pocatek; i++) {
            
            pomoc = amp * sin(frek * 2 * i * M_PI); 
            
            if (pomoc > max)
                max = pomoc;
        }
        
        return max;
        
    }
    
 //SERIE GETU, VRACI HODNOTY ATRIBUTU OBJEKTU TRIDY MAXVALUE  
    int MaxValue::getV00(){
        return v00;
    }

    int MaxValue::getVM01(){
        return vM01;
    }

    int MaxValue::getVM10() {
        return vM10;
    }

    int MaxValue::getVM11(){
        return vM11;
    }

    int MaxValue::getVV01(){
        return vV01;
    }

    int MaxValue::getVV10(){
        return vV10;
    }

    int MaxValue::getVV11(){
        return vV11;
    }
    
    
    

/**
 * Zkontroluje parametry, pokud vse probehne v poradku, tak vrati nazev souboru,
 * v opacenm pripade vrati prazdny retezec
 *  
 * @param argc Pocet parametru
 * @param argv Pole parametru
 * @return Nazev souboru nebo prazdny retezec
 */
string getParam(int argc, char** argv) {

    //pocet parametru bude vzdy 2
    if (argc != 2)
        return "";

    //test koncovky, musi byt txt
    string file = argv[1];
    int poradi = file.find(".");
    file.erase(0, poradi + 1);
    

    if (file == "wav" || file == "WAV")
        return argv[1]; //vse OK

    return ""; //spatne, není to soubor, který chci 
}

/**
 * Precte zdrojovy wav soubor a vratí ve vectoru + v parametru vraci sampleRate
 * 
 * @param signal reference na vector so ktereho se ulozi
 * @param soubor nazev vstupniho souboru
 * @param sampleRate druha hodnota, kterou funkce vraci, vzorkovaci frekvenci
 * @return pocet vzorku v signalu
 */
int readFile(vector<int>* signal, string soubor, int *sampleRate)
{
    int *bufffer;
    SndfileHandle inputFile;
    
    inputFile = SndfileHandle(soubor);
    
    *sampleRate = inputFile.samplerate();
    int frames = inputFile.frames();
    
    bufffer = new int[frames];

    inputFile.read(bufffer, frames);
    
    //pole na vector
    for (int i = 0; i < frames; i++) {
        signal->push_back(bufffer[i]);
    }
    
    delete [] bufffer;
    return frames;
    
}


/**
 * Funkce nalezne pocet vzorku na jednu dovjici v posloupnosti
 * 
 * @param signal vstupni signal v poli int
 * @param frames celkovy pocet vzorku
 * @return pocet vzroku na jednu dvojici
 */
int pocetVzorku(vector<int>* signal, int frames) {
    
    int vzorku0 = 0;
    int vzorku1 = 0;
    bool zmena = false;
    
    //prvni znaky jsou 00 11 00 11, spocita pocet vzorku pro 00 a pro 11 - trochu magie,  teoretickz by to melo fungovat
    //pokud budou dvojice zacinat 0, tak to sice pricte k 00, ale pak v 11 by melo neco chybet
    //kdyz se to secte, tak by mel vyjit spravna hodnota
    for (vector<int>::iterator it = signal->begin(); it != signal->end(); ++it) {
        if (*it == 0 && !zmena) {
            vzorku0++;
        }
        else if (*it != 0) {
            zmena = true;
            vzorku1++;
        }
        else {
            ++it;
            if (*it == 0) // dalsi je 0, pokracuji jenom 0, konec
                break;
            else {
                vzorku1++;
                --it; //navrat
                continue;
            }
        }

    }
    
    int odhad = (vzorku0 + vzorku1) / 2;
    
    return odhad;
    
}

/**
 * Ze vstupniho signalu odstrani synchronizacni sekvenci.
 * Odstrani se 4*pocet vzorku
 * 
 * @param signal Vector se vstupnim signalem
 * @param vzorku Pocet vzorku na jeden symbol
 * @param frames Celkovy pocet vyorku
 */
void deleteSynch(vector<int>* signal, int vzorku, int frames) {
    
    int odstran = 4 * vzorku;
    
    if (frames > odstran)
        signal->erase(signal->begin(),signal->begin() + odstran);
    else
        signal->erase(signal->begin(), signal->end()); //kdyvy tam byla jenom synchronizacni sekvence.... nemelo by nastat  
}

/**
 * Funkce prochazi vstupni signal a vypocitava z nich na zaklade odhadu jakou
 * dvojici dany pocet vzorku moduluje.
 * 
 * @param signal Vstupni signal
 * @param symboly Hodnoty symbolu
 * @param maxValues maximalni hodnoty
 * @param vzorku Pocet vzorku na 1 symbol
 */
void getSymbols(vector<int>* signal, vector<string>* symboly, MaxValue *maxValues, int vzorku) {
    
    int pocet = 0;
    int maxVzorek = 0;
    int v00 = maxValues->getV00();
    int vM01 = maxValues->getVM01();
    int vM10 = maxValues->getVM10();
    int vV01 = maxValues->getVV01();
    int vV10 = maxValues->getVV10();
    
    //prochazet vstupni signal
    for (vector<int>::iterator it = signal->begin(); it != signal->end(); ++it) {
        
        if(*it >  maxVzorek)
            maxVzorek = *it;
        
        pocet++;
        
        //tohle jenom kdyz mam uz vsechny vzorky pro dany symbol
        if (pocet == vzorku) {
            pocet = 0;
          
            if (maxVzorek == v00) // 00
                symboly->push_back("00");
            else if (maxVzorek >= vM01 && maxVzorek <= vV01) //01
                symboly->push_back("01");
            else if (maxVzorek >= vM10 && maxVzorek <= vV10) //10
                symboly->push_back("10");
            else //
                symboly->push_back("11"); 
            
            maxVzorek = 0;
        }
        
    }
    
}

/**
 * Funkce zapise vektor retezcu - dvojic, do vystupnihi txt soubo
 * ru
 * @param soubor nazev souboru
 * @param symboly symboly ktere se budou zapisovat do souboru
 * @return T pokud vse ok, F pokud neco v neporadku
 */
bool writeFile(string soubor, vector<string>* symboly) {
    
    string st = soubor.substr(0, soubor.size() - 4);
    
    ofstream outfile(st + ".txt");
    
    if(outfile.is_open()) {
        
        for (vector<string>::iterator it = symboly->begin(); it != symboly->end(); ++it) {
            outfile << *it;
            outfile.flush();
        }
        
        outfile << endl; 
        
        outfile.close();
        return true;
    }
    
    return false;    
}


/*
 * 
 */
int main(int argc, char** argv) {
    
    //ZPRACOVANI PRIKAZOVE RADKY
    string soubor = getParam(argc, argv);
    if (soubor == "") {
        cerr << "Chyba prikazove radky: ./bms1B nazev_souboru.wav" << endl;
        return 1;
    }
    
    //ZPRACOVANI SOUBORU
    vector<int> signal;
    int sampleRate;
    int frames = readFile(&signal, soubor, &sampleRate);
    
    //NALEZENI POCTU VROZKU
    int vzorku = pocetVzorku(&signal, frames);
    
    //ODSTRANENI SYNCHRNONIZACNI FREKVENCE
    deleteSynch(&signal, vzorku, frames);
    
    //pro kazdou dvojici se vytvori maximalni hodnota amplitudy kterou signal muze nabyt
    MaxValue *max = new MaxValue(sampleRate, vzorku);
    
    //ODHAD DVOJICE
    //problem - aplitudy nejsou uplne stejne, pri tvorbe WAV souboru se zmeni!!!! - je to neco kolem 0.00002 asi hmmmm 1%?
    vector<string> symboly;
    getSymbols(&signal, &symboly, max, vzorku);
    
    //ZAPIS DO SOUBORU
    bool ok = writeFile(soubor, &symboly);
    if (!ok) {
        cerr << "Chyba pri zapisu vystupu do souboru" << endl;
        return 1;
    }

    return EXIT_SUCCESS;
}

