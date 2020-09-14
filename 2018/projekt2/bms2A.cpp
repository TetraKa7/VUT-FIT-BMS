/*
 * 2. projekt do predmetu BMS - koder
 * Sara Skutova, xskuto00@stud.fit.vutbr.cz
 * 2018/2019
 * 
 * V tomto projektu se pouziva prevzata knihova implementujici Reed-Solomon kodovani:
 * https://github.com/pjkundert/ezpwd-reed-solomon
 * presneji se pouzivaji soubory rs a rs_base
 */

#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <fstream>

#include "rs"

#define CHUNK_SIZE 146

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
    else
        return argv[1];
}

/**
 * Funkce rozdeli vector s byty na mensi vectory
 * CHUNK_SIZE je velikost jednotlivych malych vectoru, tyto vectory se pak kazdy
 * samostatne zakoduje
 * 
 * @param byteVector vector se vsemi byty
 * @param littleVectors vector vectoru, vraceji se se vnem vectory, ktere vzniknou po rozdeleni
 */
void splitVector(vector<uint8_t> *byteVector, vector<vector<uint8_t>> *littleVectors) {
    
    vector<uint8_t> helpVector;
    int item = 0;
    
    //rozdelit na male vektorky, pokud je pocet uz prectenych mensi nez CHUNK_SIZE tak patri do stejneho vectoru
    for(vector<uint8_t>::iterator it = byteVector->begin(); it != byteVector->end(); ++it) {
        
        if (item < CHUNK_SIZE) {
            helpVector.push_back(*it);
            item++;
        }
        else //prekrocil se pocet, vlozime do vektoru vectoru, a vratime se o jeden prvek abychom mohli trnto prvek znovu precist
        {
            item = 0;
            littleVectors->push_back(helpVector);
            helpVector.clear();
            --it; //musim se vratit o prvek zpet
            
        }
        
        //casto se stane,ze posledni vector nebude mi plny pocet prvku, proto se musi testovat zda dalsi prvek jiz neni koncem
        // a vlozit tento vector vlozit do vectoru vectoru a celou iteraci ukocin
        //v opacnem pripade se vrat o jeden prvek a pokracuj
        if ((++it) == byteVector->end()) { // test jestli dalsi prvek je uz konec
            littleVectors->push_back(helpVector); 
            break;
        }
        else
            --it;
    }
    
}

/**
 * Pomoci knihovny rs, zakoduje jednotlive chunky dat
 * 
 * @param littleVectors Vector se jednotlivymi chunky dat
 * @param endocedLittleVectors Vector s jednotlivymi zakodovanymi chunky dat
 */
void encode(vector<vector<uint8_t>> *littleVectors, vector<vector<uint8_t>> *endocedLittleVectors) {
    //Prevzata knhovna
    ezpwd::RS<255,146> rs;
    
    for(vector<vector<uint8_t>>::iterator it = littleVectors->begin(); it != littleVectors->end(); ++it) {
        
        vector<uint8_t> helpVector = *it;
        rs.encode(helpVector);
        endocedLittleVectors->push_back(helpVector);
        
    }  
}

/**
 * Pri ukladani do celkoveho vystupniho souboru poprahazi jednotlive bity ve stylu
 * 1 2 3
 * 4 5 6
 * na 1 4 2 5 3 6
 * 
 * Pokud by se nezprehazely, tak by nesla opravovat burst chyba
 * 
 * @param endocedLittleVectors zakodovane chunky dat
 * @param celkovyVector vector s poprehazovanymi bity
 */
void interleaving(vector<vector<uint8_t>> *endocedLittleVectors, vector<uint8_t> *celySoubor) {
    
    //posledni vektorek tam nebude - muze mit jinou velikost nez zbytek a spatne by se to pak rozdelovalo zpet
    for (int zacatek = 0; zacatek < 255; zacatek++) {
        for(unsigned int i = 0; i < endocedLittleVectors->size() - 1; i++) {
            celySoubor->push_back(endocedLittleVectors->at(i).at(zacatek));
        }
    }
    
}

/**
 * Zapise vector do souboru
 * 
 * @param soubor nazev souboru do kterho se ma zapisovat
 * @param celySoubor zapisovany vector
 * @param endocedLittleVectors Vector s jednotlivymi zakodovanymi chunky dat
 * @return TRUE pokud vse ok nebo FALSE pri neuspechu
 */
bool saveToFile(string soubor, vector<uint8_t> *celySoubor, vector<vector<uint8_t>> *endocedLittleVectors) {
    
    ofstream out(soubor + ".out", ios::out | ios::binary);
    
    if(!out.is_open())
        return false;
    
    copy(celySoubor->begin(), celySoubor->end(), ostreambuf_iterator<char>(out));
    copy(endocedLittleVectors->back().begin(), endocedLittleVectors->back().end(), ostreambuf_iterator<char>(out));
    
    out.close();
    
    return true;
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
    //TODO prepis na funkci
    //https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator
    
    ifstream souborStream(soubor, ios::in | ios::binary);
    if (!souborStream.is_open()) {
        cerr << "Chyba pri cteni vstupniho souboru: " << soubor << endl;
        return 1;
    }
    
    //prekopiruje obsah souboru do vectoru, NEHYBEJ S TIM!!!!!
    vector<uint8_t> byteVector( (istreambuf_iterator<char>(souborStream)), istreambuf_iterator<char>() );
    souborStream.close();
    
    //ROZDELENI NA MENSI VECTORY - CHUNKY DAT
    vector<vector<uint8_t>> littleVectors;   
    splitVector(&byteVector, &littleVectors);
    
    //ZAKODOVANI VECTORU OBSAHUJICI CHUNKY DAT
    vector<vector<uint8_t>> endocedLittleVectors;
    encode(&littleVectors, &endocedLittleVectors);
    
    //INTERLEAVING DAT A ULOZENI DO JEDNOTNEHO VECTORU
    vector<uint8_t> celySoubor;
    interleaving(&endocedLittleVectors, &celySoubor);
    
    //ZAPIS DO SOUBORU
    bool ok = saveToFile(soubor, &celySoubor, &endocedLittleVectors);
    
    if(!ok) {
        cerr << "Chyba pri zapisu do vystupniho souboru: " << soubor << ".out" << endl;
        return 1;
    }


    return 0;
}

