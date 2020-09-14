/*
 * 2. projekt do predmetu BMS - dekoder
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

#define SYMBOLS 255

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
 * Popresklada vstupni vector s interleavingem do spravneho poradi. Samostane vyznaci
 * posledni vector, ktery neprosel interleavingem.
 * 
 * @param byteVector Vstupni vector
 * @param posledniVector Samostany podledni vector
 * @param bezInterleaving Vstupni vetor bez interleavingu
 */
void deIntreleaving(vector<uint8_t> *byteVector, vector<uint8_t> *posledniVector, vector <uint8_t> *bezInterleaving) {
    
    //zpetne preskladani na spravne poradi po interleavingu
    int celkovaVeclikost = byteVector->size();
    unsigned int pocetCelkovychVectorku = celkovaVeclikost / SYMBOLS; // kolik tam bude celkovych mini vectoru - chunku o velikosti 255
    int zbytek = celkovaVeclikost % SYMBOLS; // test zda je tam nejaky ceuplny vector
    
    // dostat z posloupnosti bytu posledni vector
    if (zbytek != 0) { // neco tam je, dostaneme to z konce
        //vector<uint8_t>::iterator it = byteVector.begin() + (pocetCelkovychVectorku * SYMBOLS); bude ukazovat na 1 prvek toho posledniho vectoru
        for (vector<uint8_t>::iterator it = byteVector->begin() + (pocetCelkovychVectorku * SYMBOLS); it != byteVector->end(); ++it) {
            posledniVector->push_back(*it);
        }
    }
    
    //odstranit posledni vetor, uz ho mame a nepotrebujeme ho
    byteVector->erase(byteVector->begin() + (pocetCelkovychVectorku * SYMBOLS), byteVector->end());
    
    for (unsigned int dalsi = 0; dalsi < pocetCelkovychVectorku; dalsi++) {
        for (unsigned int i = dalsi; i < byteVector->size(); i = i + pocetCelkovychVectorku) {
            //vim ze jsou rozdelene o pocetCelkovychVectorku, o pocetCelkovychVectorku od aktuali pozice je muj dalsi znak
            bezInterleaving->push_back(byteVector->at(i));
        }
    }    
}

/**
 * Rozdeluje celkovy vector na mensi vectory - zakodovane chunky dat, vime ze 
 * maji velikost 255 = SYMBOLS, nakonec prida na konec posledni vector
 * 
 * @param bezInterleaving vstupni posloupnout bytu s jiz odstranenym interleavingem
 * @param posledniVector posledni chunk dat, patri nakonec posloupnosti
 * @param littleVectors vector vectoru - jednotlive chunky dat
 */
void splitToVectors(vector<uint8_t> *bezInterleaving, vector<uint8_t> *posledniVector, vector<vector<uint8_t>> *littleVectors) {
        
    int item = 0;
    vector<uint8_t> helpVector;
    
    //ROZDELENI
    //rozdelit na male vektorky
    for(vector<uint8_t>::iterator it = bezInterleaving->begin(); it != bezInterleaving->end(); ++it) {
        if (item < SYMBOLS) { // rozdeluje na chunky dat o velikost 255
            helpVector.push_back(*it);
            item++;
        }
        else //precetli jsme o 1 prvek navice, musime se vratit
        {
            item = 0;
            littleVectors->push_back(helpVector);
            helpVector.clear();
            --it; //musim se vratit o prvek zpet
            
        }
        
        //test zda uz jsme nakonci - po pripdy, kdy se nedostaneme do pres=dchoziho else
        if ((++it) == bezInterleaving->end()) { // test jestli dalsi prvek je uz konec
            littleVectors->push_back(helpVector); 
            break;
        }
        else
            --it;
    }
    
    //dodatecne vlozit posledni vector
    littleVectors->push_back(*posledniVector);
    
}

/**
 * Pomoci knihovny rs decoduje jednotlive chunky dat
 * 
 * @param littleVectors zakodovane chunky dat
 * @param decodedLittleVectors dekodovane chunky dat
 */
void decode(vector<vector<uint8_t>> *littleVectors, vector<vector<uint8_t>> *decodedLittleVectors) {
    
    ezpwd::RS<255,146> rs;
       
    for(vector<vector<uint8_t>>::iterator it = littleVectors->begin(); it != littleVectors->end(); ++it) {
        vector<uint8_t> helpVector = *it;
        rs.decode(helpVector);
        helpVector.resize(helpVector.size() - rs.nroots()); // odstraneni partinich bitu
        decodedLittleVectors->push_back(helpVector);
    }
    
}

/**
 * Zapise vectory do souboru
 * 
 * @param soubor nazev souboru do kterho se ma zapisovat
 * @param decodedLittleVectors Vector s jednotlivymi dekodovanymi chunky dat
 * @return TRUE pokud vse ok nebo FALSE pri neuspechu
 */
bool saveToFile(string soubor, vector<vector<uint8_t>> *decodedLittleVectors) {
    
    ofstream out(soubor + ".ok", ios::out | ios::binary);
    
    if(!out.is_open())
        return false;
    
    for(vector<vector<uint8_t>>::iterator it = decodedLittleVectors->begin(); it != decodedLittleVectors->end(); ++it) {
        vector<uint8_t> helpVector = *it;
        copy(helpVector.begin(), helpVector.end(), ostreambuf_iterator<char>(out));
        
    }
    
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
    
    //ODSTRANENI INTERLEAVINGU
    vector<uint8_t> posledniVector;
    vector <uint8_t> bezInterleaving;
    deIntreleaving(&byteVector, &posledniVector, &bezInterleaving);
    
    //ROZDELENI NA MENSI VECTORY - CHUNKY DAT
    vector<vector<uint8_t>> littleVectors;
    splitToVectors(&bezInterleaving, &posledniVector, &littleVectors);
    
    //DECODOVANI
    vector<vector<uint8_t>> decodedLittleVectors;
    decode(&littleVectors, &decodedLittleVectors);
    
    //ZAPIS DO SOUBORU
    bool ok = saveToFile(soubor, &decodedLittleVectors);
    
    if(!ok) {
        cerr << "Chyba pri zapisu do vystupniho souboru: " << soubor << ".ok" << endl;
        return 1;
    }

    return 0;
}

