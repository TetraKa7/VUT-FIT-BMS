/*
 * 1. projekt do predmetu BMS - 4 ASK modulator
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

//pozmeneny sample rate, hodnota se vzala ze vzoroveho signalu
#define SAMPLE_RATE 32000
#define CHANELS 1
#define FORMAT (SF_FORMAT_WAV | SF_FORMAT_PCM_24)
#define AMPLITUDE (1.0 * 0x7F000000)
#define FREQ (1000.0 / SAMPLE_RATE)

// Pocet vzorku na 1 dvojici symbolu, tahle hodnota se vzala ze vzoroveho signalu
#define SAMPLE_SYMBOL 35



using namespace std;

/*
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
    

    if (file == "txt" || file == "TXT")
        return argv[1]; //vse OK

    return ""; //spatne, není to soubor, který chci 
}

/*
 * Precte vstupni soubor, a v digiSignal vrati obsah souboru - 0 a 1 
 * @param file Nazev vstupniho souboru
 * @param digiSignal Obsah vstupniho souboru
 * @return TRUE - vse probehlo ok, FALSE - neco je spaatne
 */
bool readFile(string file, vector<int>* digiSignal) {

    ifstream souborStream(file);

    char c;
    if (souborStream.is_open()) {

        while (souborStream.get(c)) {
            if (c != '\n')
                digiSignal->push_back(c - '0'); //c - '0', prevod ASCII hodnoty charu 0/1 na cislo 0/1
        }
        
        souborStream.close();
        return true;
    }

    return false;
}

/*
 * Pred tim nez se do vectoru posloupnosti vlozi jednotlive hodnoty, tak se tam
 * vlozi synchronizacni posloupnost - 00110011 (synchronizacni posloupnost pak bude brana jako soucast posloupnoti)
 * @param digiSignal ukazatel na vector se vstupni posloupnosti
 */
void setSynch(vector<int> *digiSignal) {
    digiSignal->push_back(0);
    digiSignal->push_back(0);
    
    digiSignal->push_back(1);
    digiSignal->push_back(1);
    
    digiSignal->push_back(0);
    digiSignal->push_back(0);
    
    digiSignal->push_back(1);
    digiSignal->push_back(1);   
}

/**
 * Funkce ze vstupni posloupnosti vytvori novy vector, kde jedne dvojici odpovida 1 amplituda.
 * Vstupni posloupnost uz obsahuje synchrnonizacni posloupnost. 
 * @param digiSignal vector vstupnich dat
 * @param ampSignal zmena amplitudy podle kazde dvojice dat
 */
void changeAmpitude(vector<int>* digiSignal, vector<double>* ampSignal) {

    //pomocne promenne pro uchovani hodnot z iteratoru
    int a;
    int b;
    
    for (vector<int>::iterator it = digiSignal->begin(); it != digiSignal->end(); ++it) {
        
        a = *it; //1. testovany bit
        ++it;
        b = *it; //2. testovany bit
        
        
        if (a == 0 && b == 0)
            ampSignal->push_back((0.0/3) * AMPLITUDE);
        else if (a == 0 && b == 1)
            ampSignal->push_back((1.0/3) * AMPLITUDE);
        else if (a == 1 && b == 0)
            ampSignal->push_back((2.0/3) * AMPLITUDE);
        else
            ampSignal->push_back((3.0/3) * AMPLITUDE);
        
    }

}


/**
 * Generovani vystupniho signalu, synchronizacni sekvence je jiz zahrnuta ve vectoru 
 * posloupnosti. Prochazi jednotlive aplitudy - 1 amplituda = 1 dvojice. Kazda dvojice
 * se vzorkuje SAMPLE_SYMBOL poctem vzorku (dle vzoru - 35). Vzorkovaci frekvence je SAMPLE_RATE
 * @param ampSignal Vstupni vector posloupnosti - obsahuje jenom amplitudy - ktere se ziskaly ze dvojic  
 * @param sinSignal Vystupni vector hodnot signalu
 */
void generateSignal(vector<double>* ampSignal, vector<int>* sinSignal) {
    
    int vzorek = 0;
    
    int pocetVzorek = 0;
    
    // prochazet aplitudy
    for (vector<double>::iterator it = ampSignal->begin(); it != ampSignal->end(); ++it) {
        
        //vzorkovat podle teto vzorkovaci frekvence
        for (int i = pocetVzorek; i < SAMPLE_RATE; i++) {
            
            //pocet vzorku na 1 symbol -  dvojici = aby ta sinusovka vice mene pekne navazovala a nevynikaly tam zubate prechody
            if (vzorek < SAMPLE_SYMBOL) {
                sinSignal->push_back(*it * sin(FREQ * 2 * i * M_PI));
                vzorek++;
            }
            else {
                pocetVzorek += vzorek;
                vzorek = 0;
                break;
            }
        }
    }
}

/**
 * Zapis posloupnosti sinus hodnot do souboru - vytvoreni vystupniho WAV souboru
 * @param name
 * @param sinSignal
 */
void writeToFile(string name, vector<int>* sinSignal) {
    
    SndfileHandle outputFile;
    string st = name.substr(0, name.size() - 4); //odstraneni .txt z nazvu vstupniho souboru
    
    outputFile = SndfileHandle(st + ".wav", SFM_WRITE, FORMAT, CHANELS, SAMPLE_RATE);

    
    outputFile.write(sinSignal->data(), sinSignal->size()); 
}


/*
 * 
 */
int main(int argc, char** argv) {

    //ZPRACOVANI PRIKAZOVE RADKY
    string soubor = getParam(argc, argv);
    if (soubor == "") {
        cerr << "Chyba prikazove radky: ./bms1A nazev_souboru.txt" << endl;
        return 1;
    }

    //ZPRACOVANI SOUBORU
    vector<int> digiSignal;
    
    //jeste pred tim nez se precte vstupni posloupnost, tak se do vectoru vlozi
    //synchronizacni posloupnost
    setSynch(&digiSignal);

    bool ok = readFile(soubor, &digiSignal);
    if (!ok) {
        cerr << "Chyba pri cteni vstupniho souboru: " << soubor << endl;
        return 1;
    }

    //MODULACE
    vector<double> ampSignal;

    changeAmpitude(&digiSignal, &ampSignal);
    
    vector<int> sinSignal;
    
    generateSignal(&ampSignal, &sinSignal);
    
    //ZAPIS DO SOUBORU
    writeToFile(soubor, &sinSignal);

    return 0;
}

