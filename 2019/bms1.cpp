/**
 * 1. projekt do predmetu BMS - 4 ASK modulator
 * Sara Skutova, xskuto00@stud.fit.vutbr.cz
 * 2019/2020
 *
 * Cast funkci je prevzata z mych projektu do BMS z minuleho roku
 *
 */


#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>


using namespace std;


#define MPEG_PACKET_SIZE 188
#define FREQ 27000000

enum TYP {NO_NEED, PAT, NIT, SDT};

/**
 * Trida reprezentujici element programu neboli audio, video, a technicke informace daneho programu.
 * Obsahuje inforamce o PID daného elementu, pocet jeho paketu a jeho bitrate.
 * */
class ProgramElem {
public:
    ProgramElem(uint16_t elemPID);
    void setElemPid(uint16_t elemPID);
    uint16_t getElemPID();
    void setPocetPacketu(int pocet);
    uint64_t getPocetPacketu();
    void setBitrate(double bitrate);
    double getBitrate();
private:
    uint16_t elemPid;
    uint64_t pocetPacketu;
    double bitarate;
};

ProgramElem::ProgramElem(uint16_t elemPID) {
    this->elemPid = elemPID;
}


void ProgramElem::setElemPid(uint16_t elemPID) {
    this->elemPid = elemPID;
}

uint16_t ProgramElem::getElemPID() {
    return this->elemPid;
}

void ProgramElem::setPocetPacketu(int pocet) {
    this->pocetPacketu = pocet;
}

uint64_t ProgramElem::getPocetPacketu() {
    return this->pocetPacketu;
}

void ProgramElem::setBitrate(double bitrate) {
    this->bitarate = bitrate;
}

double ProgramElem::getBitrate() {
    return this->bitarate;
}


/**
 * Trida reprezentujici program.
 * Obsahuje jeho servisni inforamce, PID, seznam jeho elementu.
 * */
class Program {
public:
    Program();
    void setpMumber(uint16_t pNumber);
    uint16_t getpNumber();
    void setpmPID(uint16_t pmPID);
    uint16_t getpmPID();
    void setproviderName(string providerName);
    string getproviderName();
    void setserviceName(string serviceName);
    string getserviceName();
    void insertPID(ProgramElem PID);
    vector<ProgramElem> getElemPID();
    void saveToElem(int pocet, int elem, double bitrate);
    void setpPCRPID(uint16_t pPCRPID);
    uint16_t getpPCRPID();
    void setDone();
    bool getDone();
    void setProgBitrate(double bitrate);
    double getProgBitrate();


private:
    uint16_t pNumber;
    uint16_t pmPID;
    uint16_t pPCRPID;
    string providerName;
    string serviceName;
    vector<ProgramElem> elemPID;
    bool isDone;
    uint64_t progBitrate;

};

Program::Program() {
    this->isDone = false;
}

void Program::setpMumber(uint16_t pNumber) {
    this->pNumber = pNumber;
}

uint16_t Program::getpNumber() {
    return this->pNumber;
}

void Program::setpmPID(uint16_t pmPID) {
    this->pmPID = pmPID;
}

uint16_t Program::getpmPID() {
    return this->pmPID;
}

void Program::setproviderName(string providerName) {
    this->providerName = providerName;
}

string Program::getproviderName() {
    return this->providerName;
}

void Program::setserviceName(string serviceName) {
    this->serviceName = serviceName;
}

string Program::getserviceName() {
    return this->serviceName;
}

void Program::insertPID(ProgramElem PID) {
    this->elemPID.push_back(PID);
}

vector<ProgramElem> Program::getElemPID() {
    return this->elemPID;
}

void Program::setDone() {
    this->isDone = true;
}

bool Program::getDone() {
    return this->isDone;
}

void Program::saveToElem(int pocet, int elem, double bitrate) {
    this->elemPID.at(elem).setPocetPacketu(pocet);
    this->elemPID.at(elem).setBitrate(bitrate);
}

void Program::setpPCRPID(uint16_t pPCRPID) {
    this->pPCRPID = pPCRPID;
}

uint16_t Program::getpPCRPID() {
    return this->pPCRPID;
}

void Program::setProgBitrate(double bitrate) {
    this->progBitrate = bitrate;

}

double Program::getProgBitrate() {
    return this->progBitrate;
}

/**
 * Trida reprezentujici servisni inforamce zkoumaneho streamu
 * */
class NITInfo {
public:
    void setnName(string nName);
    string getnName();
    void setnID(string nID);
    string getnID();
    void setBandwidth(string bandwidth);
    string getBandwidth();
    void setConstellation(string constellation);
    string getConstellation();
    void setCRate(string cRate);
    string getCRate();
    void setGInterval(string gInterval);
    string getGInterval();
private:
    string nName;
    string nID;
    string bandwidth;
    string constellation;
    string gInterval;
    string cRate;
};

void NITInfo::setnName(string nName) {
    this->nName = nName;
}

string NITInfo::getnName() {
    return this->nName;
}

void NITInfo::setnID(string nID) {
    this->nID = nID;
}

string NITInfo::getnID() {
    return this->nID;
}

void NITInfo::setBandwidth(string bandwidth) {
    this->bandwidth = bandwidth;
}

string NITInfo::getBandwidth() {
    return this->bandwidth;
}

void NITInfo::setConstellation(string constellation) {
    this->constellation = constellation;
}

string NITInfo::getConstellation() {
    return this->constellation;
}

void NITInfo::setCRate(string cRate) {
    this->cRate = cRate;
}

string NITInfo::getCRate() {
    return this->cRate;
}

void NITInfo::setGInterval(string gInterval) {
    this->gInterval = gInterval;
}

string NITInfo::getGInterval() {
    return this->gInterval;
}



//-------------------------------------------------------------------------------------------//

/**
 * Trida predstavujici MPEG PACKET
 */
class MPEGPacket {
public:

    static bool isNIT;
    static bool isPAT;
    static bool isSDT;
    static bool isPMT;
    static uint64_t pocetPacketu;
    static NITInfo nit;
    static vector<Program> programy;

    MPEGPacket(vector<uint8_t> bytePacketu);
    void analyseAFC(int actualIndex);
    void analysePacket();
    void createTelicko(uint32_t zacatek);
    int getPacketSize();
    uint32_t getHlavicka();
    int getPid();
    int getVelikostTelicka();
    void analyseSDTPacket();
    void analysePATPacket();
    void analysePMTPacket(int index, uint32_t pid);
    void analyseNITPacket();
    int getPUSI();
    vector<uint8_t> getTelicko();
    void setTelisko(vector<uint8_t> telicko);
    bool isAFC();
    void setPCR(uint64_t PCR);
    uint64_t getPCR();
    bool isPCR();

private:
    vector<uint8_t> bytePacketu;
    uint32_t hlavicka;
    uint8_t TEI;  //1b, nastaveno, pokud je packet neopravitelny
    uint8_t PUSI; //1b, nastaveno pokud je telo paketu hned za hlavickou
    uint32_t PID; //13b, urcuje jaka data jsou v tele paketu
    uint8_t AFC; //2b, oznacuje jestli za hlavickou jsou dodatecna data hlavickz nebo uz telo, nebo kombinace
    uint64_t PCR;

    bool noData;
    bool haveAFC;
    bool havePCR;
    uint64_t indexPacketu;

    vector<uint8_t> telicko;

};

bool MPEGPacket::isNIT = false;
bool MPEGPacket::isPAT = false;
bool MPEGPacket::isSDT = false;
bool MPEGPacket::isPMT = false;
NITInfo MPEGPacket::nit = NITInfo();
vector<Program> MPEGPacket::programy;
uint64_t MPEGPacket::pocetPacketu = 0;


MPEGPacket::MPEGPacket(vector<uint8_t> bytePacketu) {
    this->bytePacketu = bytePacketu;
    this->noData = false;
    this->haveAFC = false;
    this->havePCR = false;
    indexPacketu = MPEGPacket::pocetPacketu;
    MPEGPacket::pocetPacketu++;
}

/**
 * Analyzuje Adaptation field, analyzuje se kvli ziskani hodnoty PCR, diky ktere se pozdeji vypocita
 * bitrate.
 *
 * @param actualIndex indedx pole packetu, na kterem se nachazi zacatek Adaptation field
 * */
void MPEGPacket::analyseAFC(int actualIndex) {
    uint8_t delkaAFC = this->bytePacketu.at(actualIndex);
    if (delkaAFC == 0)
        return;

    uint8_t PCRFlag = (this->bytePacketu.at(actualIndex + 1) >> 4) & 1;
    if (!PCRFlag) //zda se tam nachazi PCR informace
        return;

    //PCR hodnota se sklada ze dvou casti, ktere pak dosadi vzorce - Base a Exponent
    uint8_t PCRBaseA = (this->bytePacketu.at(actualIndex + 6) >> 7) & 1;
    uint64_t PCRBase = PCRBaseA | (this->bytePacketu.at(actualIndex + 5) << 1) | (this->bytePacketu.at(actualIndex + 4) << 9) |
            (this->bytePacketu.at(actualIndex + 3) << 17) | ((uint64_t)(this->bytePacketu.at(actualIndex + 2)) << 25);

    uint8_t PCRExA = this->bytePacketu.at(actualIndex + 6) & 1;
    uint16_t PCREx = this->bytePacketu.at(actualIndex + 7) | (PCRExA << 8);


    uint64_t pcr = (PCRBase * 300) + PCREx;
    this->havePCR = true;
    setPCR(pcr);
}



/**
 * Analyzuje packet, vzbere z neho hlavicku a telisko, vybere potrebne informace z hlavicky
 */
void MPEGPacket::analysePacket() {
    //prevod 4B na hlavicku, ktera ma 32b
    hlavicka = this->bytePacketu.at(3) | (this->bytePacketu.at(2) << 8) | (this->bytePacketu.at(1) << 16) | (this->bytePacketu.at(0) << 24);

    //prace s hlavickou, nastavovani hodnot atributu podle hlavicky
    this->TEI = (hlavicka >> 23) & 1;
    if (this->TEI) {
        cerr << "V Multiplexu je poskozeny paket, TEI bit byl nastaven, paket nebude zpracovan" << endl;
        return;
    }
    this->PUSI = (hlavicka >> 22) & 1;
    this->PID = (hlavicka >> 8) & 8191; //0b1111111111111, 0x1FFF, 13b zaznam
    this->AFC = (hlavicka >> 4) & 3; //0b11, 0x3, 2b zaznam

    //ziskani telicka packetu, musi se zohlednovat PUSI a AFC
    // PUSI == 1, PES nebo PSI tam zacina, bude tam pointer_field, 8b, urcuje za kolik BYTU za p_f zacina telicko, u PES
    // bude hodnota na zacatku 000001, takze se nic nepreskoci
    // PUSI == 0, nic tam nezacina, nebo tam neni pointer_field, nic se nepreskakuje
    // dale zalezi na AFC:
    // AFC == 1 je tam jenom telicko s daty
    // AFC == 2 je tam jenom adaptacni pole s dodatecnymi informacemi
    // AFC == 3, adp pole + data, adp pole obsahuje informaci jak je velke, na zacatku, 8b, urcuje za kolik BYTU za nim je telicko
    if (this->PUSI) {
        if (this->AFC == 0b01) { //je tam jenom telicko, + hodnota z pointer_field
            //hodnota na pointer_filedu mi rika, za kolik BYTU za pointer_fieldem se nachazi data
            uint32_t zacatekDAt = 4 + this->bytePacketu.at(4) + 1;  //4B velikost hlavicky, 1B abychom se dostali za pointer_field
            createTelicko(zacatekDAt);
        }
        else if (this->AFC == 0b10) {  // je tam jenom adaptacni pole, nejsou tam data
            this->haveAFC = true;
            analyseAFC(4);
        }
        else if (this->AFC == 0b11) { //adaptacni pole + data, adaptacni pole je hned za hlavickou, jeho 1. BYTE urcuje kolik BYTU za nim je jeste AP, za AP je pak je pointer_field
            this->haveAFC = true;
            analyseAFC(4);
            uint32_t afDelka = this->bytePacketu.at(4);
            uint32_t pointerField = this->bytePacketu.at(4 + 1 + afDelka); //4B hlavicka, 1B adaptation_field_length, hodnota adaptation_field_length
            uint32_t zacatekDAt = 4 + 1 + afDelka + 1 + pointerField; // 4B hlavicka, 1B adaptation_field_length, hodnota adaptation_field_length, 1B pointerField, hodnota pointerField
            createTelicko(zacatekDAt);
        }
        else
            cerr << "Nedovolena hodnota AFC pri zpracovani hlavicky packetu" << endl;
    }
    else { //PUSI == 0, PES tam nezacina, neni tam pointer field
        if (this->AFC == 0b01)
            createTelicko(4); //za hlavickou jsou hned data
        else if (this->AFC == 0b10) {  // je tam jenom adaptacni pole, nejsou tam data, TODO ignorovat?
            this->haveAFC = true;
            analyseAFC(4);
        }
        else if (this->AFC == 0b11) {
            this->haveAFC = true;
            analyseAFC(4);
            uint32_t afDelka = this->bytePacketu.at(4);
            uint32_t zacatekDAt = this->bytePacketu.at(4 + 1 + afDelka); //4B hlavicka, 1B adaptation_field_length, hodnota adaptation_field_length
            createTelicko(zacatekDAt);
        }
        else
            cerr << "Nedovolena hodnota AFC pri zpracovani hlavicky packetu" << endl;
    }

}

/**
 * Analyzuje informace z SDT tabulky
 */
void MPEGPacket::analyseSDTPacket() {

    if (MPEGPacket::isSDT) // nejaky SDT packet uz byl zpracovan
        return;

    if (this->telicko.at(0) != 0x42 || !this->getPUSI())
        return;

    MPEGPacket::isSDT = true;

    uint8_t secLenghtA = this->telicko.at(1) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
    uint16_t secLenght = this->telicko.at(2) | (secLenghtA << 8); //delka od policka az dokonce + je tam i crc

    if (this->telicko.size() < secLenght) { //ochrana proti packetum, ktere se spatne spojily, TODO oprav to
        MPEGPacket::isSDT = false;
        return;
    }

    int actualIndex = 11;
    secLenght -= 12; // 4B CRC, 8B nez zacne cyklus
    for (int i = 0; i < secLenght; i++) {
        uint16_t serviceID = this->telicko.at(actualIndex + 1 + i) | (this->telicko.at(actualIndex + i) << 8);

        int indexHelp = 0;
        for (int program = 0; program < this->programy.size(); program++) {
            if (this->programy.at(program).getpNumber() == serviceID) {
                indexHelp = program;
                break;
            }
        }

        uint8_t desLenghtA = this->telicko.at(actualIndex + 3 + i) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
        uint16_t desLenght = this->telicko.at(actualIndex + 4 + i) | (desLenghtA << 8); //delka cylku s decriptorama

        for (int j = 0; j < desLenght; j++) {

            uint8_t tag = this->telicko.at(actualIndex + 5 + i + j);
            uint8_t desLen = this->telicko.at(actualIndex + 6 + i + j); //delka 1 decriptoru

            if (tag == 0x48) { // Service descriptor

                string providerName = "";
                uint8_t providerNameLen = this->telicko.at(actualIndex + 8 + i + j); //delka jmena proviceru
                for (int k = 0; k < providerNameLen; k++) {
                    providerName += (char) this->telicko.at(actualIndex + 9 + i + j + k);
                }

                uint8_t serviceNameLen = this->telicko.at(actualIndex + 9 + i + j + providerNameLen);
                string serviceName = "";
                for (int l = 0; l < serviceNameLen; l++) {
                    serviceName += (char) this->telicko.at(actualIndex + 10 + i + j + providerNameLen + l);
                }

                this->programy.at(indexHelp).setproviderName(providerName);
                this->programy.at(indexHelp).setserviceName(serviceName);
            }
            j += 1 + 1 + desLen - 1;
        }

        i += 2 + 1 + 2 + desLenght - 1;
    }





}
/**
 * Analyzuje informace z PMT tabulky pro kazdy program
 */
void MPEGPacket::analysePMTPacket(int index, uint32_t pid) {

    if(this->programy.at(index).getDone())
        return;

    if (this->telicko.at(0) != 0x02)
        return;

    this->programy.at(index).insertPID(ProgramElem(pid));

    uint8_t secLenghtA = this->telicko.at(1) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
    uint16_t secLenght = this->telicko.at(2) | (secLenghtA << 8);


    uint8_t pcrPIDA = this->telicko.at(8) & 31; //potrebuji jenom 5 spodni bity, and 0001 1111
    uint16_t pcrPID = this->telicko.at(9) | (pcrPIDA << 8);

    this->programy.at(index).setpPCRPID(pcrPID);

    uint8_t progLenghtA = this->telicko.at(10) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
    uint16_t progLenght = this->telicko.at(11) | (progLenghtA << 8);


    int actualIndex = 12 + progLenght;
    secLenght -= (13 + progLenght);

    for (int i = 0; i < secLenght; ++i) {

        uint8_t elemPIDA = this->telicko.at(actualIndex + 1 + i) & 31; //potrebuji jenom 5 spodni bity, and 0001 1111
        uint16_t elemPID = this->telicko.at(actualIndex + 2 + i) | (elemPIDA << 8);

        this->programy.at(index).insertPID(ProgramElem(elemPID));

        uint8_t esA = this->telicko.at(actualIndex + 3 + i) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
        uint16_t es = this->telicko.at(actualIndex + 4 + i) | (esA << 8);

        i += 5 + es - 1;
    }

    this->programy.at(index).setDone();

}



/**
 * Analyzuje informace z PAT tabulky, zjisti se info o Programu
 */
void MPEGPacket::analysePATPacket() {

    if (MPEGPacket::isPAT) // nejaky PAT packet uz byl zpracovan
        return;

    if (this->telicko.at(0) != 0x00)
        return;

    MPEGPacket::isPAT = true;

    uint8_t secLenghtA = this->telicko.at(1) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
    uint16_t secLenght = this->telicko.at(2) | (secLenghtA << 8);
    // je to delka od daneho policka az dokonce, k cyklu to jsou 5B, na konci jsou 4B CRC
    secLenght -= 9;

    int actualIndex = 8;

    for (int i = 0; i < secLenght; i++) {
        uint16_t programNumber = this->telicko.at(actualIndex + 1 + i) | (this->telicko.at(actualIndex + i) << 8);

        if (programNumber != 0) {
            uint8_t pmIDA = this->telicko.at(actualIndex + 2 + i) & 31;
            uint16_t pmID = this->telicko.at(actualIndex + 3 + i) | (pmIDA << 8);
            Program program;
            program.setpMumber(programNumber);
            program.setpmPID(pmID);
            this->programy.push_back(program);
        }

        i += 3; //dostat se za telo 1 cyklu, nasledne se jeste provede i++; cim ze dostane na zacatek
    }
}

/**
 * Analyzuje informace z NIT tabulky, zjisti servisni informace o danem transportnim streamu
 */
void MPEGPacket::analyseNITPacket() {

    if (MPEGPacket::isNIT) // NIT uz jednou bylo zpracovane, nemelo by se snad menit
        return;

    //je to NIT, ktere chceme? musi mit 0x40, 0x41 je odkaz na jinaci medium, urcuje to table_id, 1B na zacatku dat
    if (this->telicko.at(0) != 0x40)
        return;

    MPEGPacket::isNIT = true;

    //NETWORK ID
    uint16_t networkID = this->telicko.at(4) | (this->telicko.at(3) << 8);
    MPEGPacket::nit.setnID(to_string(networkID));

    //prochazeni descriptorama a hledani potrebnych informaci
    uint8_t networkName = 0x40;
    uint8_t TDS = 0x5a; //terrestrial_delivery_system_descriptor

    //vypocet kolik descriptory zabiraji mista v BYTECH, zacina se tedy od indexu 10
    uint8_t descLenghtA = this->telicko.at(8) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
    uint16_t descLenght = this->telicko.at(9) | (descLenghtA << 8);


    //CYKLU PRO NETWORK DESCRIPTORY, HLEDA SE V NEM NETWORK NAME
    int actualIndex = 10;

    for (int i = 0; i < descLenght; i++) {

        uint32_t descTag = this->telicko.at(actualIndex + i);

        //ziskat velikost desciptoru, presneji pocej BYTU do konce od tohoto policka, 1B
        uint32_t nMDlenght = this->telicko.at(actualIndex + i + 1);

        if (descTag == networkName) { // je to network_name_descriptor
            //ziskat data, co BYTE to je jeden char znak
            string name = "";
            for (int j = 0; j < nMDlenght; j++) {
                name += (char) this->telicko.at(actualIndex + i + 1 + 1 + j); // actualIndex + i je TAG, +1 = delka, +1 = zacatek dat, +j = postupovani v datech
            }
            this->nit.setnName(name);
        }

        i += 1 + 1 + nMDlenght - 1; //Dostat se za descriptor, 1B tag, 1B delka, xB data descriptoru, -1 protoze se provede i++
        // v dalsim cykllu tedy bude hodnota ukazovat na dalsi tag
    }

    actualIndex += descLenght;
    uint8_t TSLLenghtA = this->telicko.at(actualIndex) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
    uint16_t TSLLenght = this->telicko.at(actualIndex + 1) | (TSLLenghtA << 8);

    actualIndex += 2; //za TSLL, v 1.B cyklu
    for (int i = 0; i < TSLLenght; i++) {


        actualIndex += 4 + i; // 2 za transport_stream_id , 2 za original_network_id

        uint8_t TDLenghtA = this->telicko.at(actualIndex) & 15; //potrebuji jenom 4 spodni bity, and 0000 1111
        uint16_t TDLenght = this->telicko.at(actualIndex + 1) | (TDLenghtA << 8); //transport_descriptors_length

        actualIndex += 2 + i; //dostat se za transport_descriptors_length
        for (int j = 0; j < TDLenght; j++) {

            uint32_t descTag = this->telicko.at(actualIndex + j);

            //ziskat velikost desciptoru, presneji pocej BYTU do konce od tohoto policka, 1B
            uint32_t nMDlenght = this->telicko.at(actualIndex + j + 1);

            if (descTag == TDS) {
                // BANDWIDTH
                uint8_t bandwidth = this->telicko.at(actualIndex + j + 6); // actualIndex + j = TAG, +1 = delka, +4 frekvence, +1 bandwidth, 3b chci
                bandwidth = (bandwidth >> 5) & 7; //0b111, chci 3 bity
                if (bandwidth == 0b000)
                    this->nit.setBandwidth("8 MHz");
                else if (bandwidth == 0b001)
                    this->nit.setBandwidth("7 MHz");
                else if (bandwidth == 0b010)
                    this->nit.setBandwidth("6 MHz");
                else if (bandwidth == 0b011)
                    this->nit.setBandwidth("5 MHz");
                else
                    cerr << "Pri zpracovani NIT packetu se vyskystka zakazana hodnota v bandwidth" << endl;

                //constellation
                uint8_t constellation = this->telicko.at(actualIndex + j + 7); // actualIndex + j = TAG, +1 = delka, +4 frekvence, +1 bandwidth a dalsi, +1 constellation
                constellation = (constellation >> 6) & 3; //0b11, chci 2b
                if (constellation == 0b00)
                    this->nit.setConstellation("QPSK");
                else if (constellation == 0b01)
                    this->nit.setConstellation("16-QAM");
                else if (constellation == 0b10)
                    this->nit.setConstellation("64-QAM");
                else
                    cerr << "Pri zpracovani NIT packetu se vyskystka zakazana hodnota v constellation" << endl;

                //code_rate
                uint8_t codeRate = this->telicko.at(actualIndex + j + 7); // actualIndex + j = TAG, +1 = delka, +4 frekvence, +1 bandwidth a dalsi, +1 constellation - je tam i codeRade
                codeRate = codeRate & 7; //0b111 chci spodni 3 bity
                if (codeRate == 0b000)
                    this->nit.setCRate("1/2");
                else if (codeRate == 0b001)
                    this->nit.setCRate("2/3");
                else if (codeRate == 0b010)
                    this->nit.setCRate("3/4");
                else if (codeRate == 0b011)
                    this->nit.setCRate("5/6");
                else if (codeRate == 0b100)
                    this->nit.setCRate("7/8");
                else
                    cerr << "Pri zpracovani NIT packetu se vyskystka zakazana hodnota v codeRate" << endl;

                //guard_interval
                uint8_t guardInterval = this->telicko.at(actualIndex + j + 8); // to co predtim, ale v 8. BYTU, je uprostred, 2b
                guardInterval = (guardInterval >> 3) & 3; //0b11, chci 2b
                if (guardInterval == 0b00)
                    this->nit.setGInterval("1/32");
                else if (guardInterval == 0b01)
                    this->nit.setGInterval("1/16");
                else if (guardInterval == 0b10)
                    this->nit.setGInterval("1/8");
                else if (guardInterval == 0b11)
                    this->nit.setGInterval("1/4");
                else
                    cerr << "Pri zpracovani NIT packetu se vyskystka zakazana hodnota v guardInterval" << endl;

            }
            //pochazeni descriptoru
            j += 1 + 1 + nMDlenght - 1; //Dostat se za descriptor, 1B tag, 1B delka, xB data descriptoru, -1 protoze se provede j++
        }

        i += 2 + 2 + 2 + TDLenght - 1; //dostat se na konec cyklu, 2B id, 2B id, 2B rezerva + velikost decxripotru (vsech), -1 protoze i++
    }

}

int MPEGPacket::getVelikostTelicka() {
    return this->telicko.size();
}

/**
 * Od daneho zacatku vybere z packetu telicko
 * @param zacatek od jakeho bytu se nachazi telicko packetu
 * */
void MPEGPacket::createTelicko(uint32_t zacatek) {
    for (int i = zacatek; i < this->bytePacketu.size(); i++)
        this->telicko.push_back(this->bytePacketu.at(i));
}

int MPEGPacket::getPacketSize() {
    return bytePacketu.size();
}

uint32_t MPEGPacket::getHlavicka() {
    return hlavicka;
}

int MPEGPacket::getPid() {
    return PID;
}

int MPEGPacket::getPUSI() {
    return PUSI;
}

vector<uint8_t> MPEGPacket::getTelicko() {
    return telicko;
}

void MPEGPacket::setTelisko(vector<uint8_t> telicko) {
    this->telicko.insert(this->telicko.end(), telicko.begin(), telicko.end());
}

bool MPEGPacket::isAFC() {
    return this->haveAFC;
}

void MPEGPacket::setPCR(uint64_t PCR) {
    this->PCR = PCR;

}

uint64_t MPEGPacket::getPCR() {
    return this->PCR;
}

bool MPEGPacket::isPCR() {
    return this->havePCR;
}

//--------------------------------------------------------------------------------------------//



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

    //test koncovky, musi byt ts
    string file = argv[1];
    int poradi = file.find(".");
    file.erase(0, poradi + 1);


    if (file == "ts" || file == "TS")
        return argv[1]; //vse OK

    return ""; //spatne, není to soubor, který chci
}

/**
 * Funkce rozdeli vector s byty na mensi vektory = Objekty tridy MPEGPacket
 * MPEG_PACKET_SIZE je stabdartni velikost packetu
 *
 * @param packety vector s packetama
 * @param byteVector vector se vsemi byty
 */
void createMPEGPPackets(vector<MPEGPacket> *packety, vector<uint8_t> *byteVector) {

    vector<uint8_t> helpVector;
    int item = 0;

    for(vector<uint8_t>::iterator it = byteVector->begin(); it != byteVector->end(); ++it) {

        if (item < MPEG_PACKET_SIZE) {
            helpVector.push_back(*it);
            item++;
        }
        else //prekocila se velikost MPEG packetu, vztvori se objekt packetu a vektor se vlozi do neho
        {
            item = 0;
            MPEGPacket packet = MPEGPacket(helpVector);
            packety->push_back(packet);
            helpVector.clear();
            --it; //musim se vratit o prvek zpet
        }

        //casto se stane,ze posledni packet nebude mit plny pocet bytu (188), proto se musi testovat zda dalsi prvek jiz neni koncem
        // pokud ani, tak vytvorit packet i pro neplny pocet prvku
        //v opacnem pripade se vrat o jeden prvek a pokracuj
        if ((++it) == byteVector->end()) { // test jestli dalsi prvek je uz konec
            MPEGPacket packet = MPEGPacket(helpVector);
            packety->push_back(packet);
            break;
        }
        else
            --it;

    }
}

/**
 * Funkce spoji SDT pakety, ktere nasleduji za sebou do jednoho celku.
 * Nektere pakety se rozdeli z duvodu sve velikosti
 *
 * @param packety vector s packetama
 */
void mergeSDT(vector<MPEGPacket> *packety) {

    for (int i = 0; i < packety->size(); i++) {
        uint32_t pid = packety->at(i).getPid();
        if (pid == 0x11) { //zatim
            if (packety->at(i).getTelicko().at(0) == 0x42 && packety->at(i).getPUSI()) {// je to pocatecni packet, do jeho telicka se budou pridatav hodnoty
                int j = i + 1;
                pid = packety->at(j).getPid();
                while (pid == 0x11 && packety->at(j).getPUSI() != 1) { //hledam dalsi packety, ktere pripojim k predchazejicimu
                    packety->at(i).setTelisko(packety->at(j).getTelicko());
                    j++;
                    pid = packety->at(j).getPid();
                }
                i = j;
            }
        }
    }
}


/**
 * Funkce vypocita bitrate streamu.
 * Vyuziva k tomu pcr pakety kazdeho programu.
 *
 * @param packety vector s packetama
 * @return bitrate streamu
 */
double bitrate(vector<MPEGPacket> *packety) {

    uint64_t pcr1;
    uint64_t pcr2;
    uint64_t prvni;
    uint64_t druhy;

    //vypocet pro kazdy program pomoci jejich pcr
    for (int j = 0; j < MPEGPacket::programy.size(); j++) {
        uint32_t pcrPid = MPEGPacket::programy.at(j).getpPCRPID();
        int pocet = 0;
        for (int i = 0; i < packety->size(); i++) {
            uint32_t pidPacket = packety->at(i).getPid();
            if (pidPacket == pcrPid && packety->at(i).isPCR()) {
                pocet++;
                if (pocet == 1) {
                    pcr1 = packety->at(i).getPCR();
                    prvni = i;
                }
                else if (pocet > 1) {
                    pcr2 = packety->at(i).getPCR();
                    druhy = i;
                }
            }
        }



        //vypocet dle vzorce https://ecee.colorado.edu/~ecen5653/ecen5653/papers/iso13818-1.pdf, strana 11
        uint64_t programBitrate = (FREQ * (druhy - prvni)) / (pcr2 - pcr1);
        programBitrate = programBitrate * 188 * 8;
        MPEGPacket::programy.at(j).setProgBitrate(programBitrate);


    }

    uint64_t birateStreamu = 0;
    for (int j = 0; j < MPEGPacket::programy.size(); j++) {
        birateStreamu += MPEGPacket::programy.at(j).getProgBitrate();
    }


    birateStreamu = birateStreamu/MPEGPacket::programy.size();



    return birateStreamu;


}


/**
 * Funkce vypocita bitrate pro kazdy program pomoci pozadovaneho vzorce
 *
 * @param packety vector s packetama
 * @param treamBitrate bitrate streamu
 */
void programsBitrate(vector<MPEGPacket> *packety, double streamBitrate) {

    //vypocet bitratu pro jednotlive elementy programu
    for (int j = 0; j < MPEGPacket::programy.size(); j++) {
        vector<ProgramElem> elem = MPEGPacket::programy.at(j).getElemPID();
        for (int i = 0; i < elem.size(); i++) {
            uint16_t elemPID = elem.at(i).getElemPID();
            int pocet = 0;
            for (int k = 0; k < packety->size(); k++) {
                uint32_t pidPacket = packety->at(k).getPid();
                if (elemPID == pidPacket) {
                    pocet++;
                }
            }

            //vypocet dle dohodnuteho vzroce
            double bitrate = (((double) pocet) / ((double) packety->size())) * ((double) streamBitrate);
            MPEGPacket::programy.at(j).saveToElem(pocet, i, bitrate);
        }

    }

    //spocitani celkoveho bitratu programu (bitraty elementu se scitaji)
    for (int i = 0; i < MPEGPacket::programy.size(); i++) {
        vector<ProgramElem> elemPID = MPEGPacket::programy.at(i).getElemPID();
        double progBitrate = 0;
        for (int j = 0; j <elemPID.size() ; ++j)
            progBitrate += elemPID.at(j).getBitrate();


        MPEGPacket::programy.at(i).setProgBitrate(progBitrate);
    }

}

/**
 * Tisk vystupu do pozadovaneho souboru
 *
 * @param packety vector s packetama
 * @param name jmeno vystupniho souboru
 */

bool printInfo(vector<MPEGPacket> *packety, string name) {

    string st = name.substr(0, name.size() - 3);

    ofstream outfile(st + ".txt");

    if(outfile.is_open()) {
        outfile << "Network name: " << MPEGPacket::nit.getnName() << endl;
        outfile << "Network ID: " << MPEGPacket::nit.getnID() << endl;
        outfile << "Bandwidth: " << MPEGPacket::nit.getBandwidth() << endl;
        outfile << "Constellation: " << MPEGPacket::nit.getConstellation() << endl;
        outfile << "Guard interval: " << MPEGPacket::nit.getGInterval() << endl;
        outfile << "Code rate: " << MPEGPacket::nit.getCRate()<< endl;

        outfile << endl;

        for (int i = 0; i < MPEGPacket::programy.size(); i++) {
            outfile << "0x" << right <<setfill('0') << setw(4) << hex << MPEGPacket::programy.at(i).getpmPID() << setfill(' ') <<
                    setw(0) <<  "-" <<  MPEGPacket::programy.at(i).getproviderName() << "-" << MPEGPacket::programy.at(i).getserviceName() << ": ";

            double progBitrate = MPEGPacket::programy.at(i).getProgBitrate() / 1000000;
            double rouded = progBitrate * 100;
            rouded = round(rouded);
            rouded = rouded / 100;
            outfile << dec << left << setfill('0') << setw(4) << rouded << " Mbps" << endl;
        }

        outfile.close();

        return true;
    }

    return false;
}



int main(int argc, char** argv) {

    //ZPRACOVANI PRIKAZOVE RADKY
    string soubor = getParam(argc, argv);
    if (soubor == "") {
        cerr << "Chyba prikazove radky: ./bms1 nazev_souboru.ts" << endl;
        return 1;
    }


    //ZPRACOVANI SOUBORU
    //TODO prepis na funkci
    //https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator

    ifstream souborStream(soubor, ios::binary| ios::in);
    if (!souborStream.is_open()) {
        cerr << "Chyba pri cteni vstupniho souboru: " << soubor << endl;
        return 1;
    }

    //prekopiruje obsah souboru do vectoru, NEHYBEJ S TIM!!!!!
    vector<uint8_t> byteVector( (istreambuf_iterator<char>(souborStream)), istreambuf_iterator<char>() );
    souborStream.close();

    //VYTVORENI MPEG PACKETU
    vector<MPEGPacket> packety;
    createMPEGPPackets(&packety, &byteVector);

    //ANALYZA HLAVICKY PACKETU, zjisti se strukturu paketu (jake data se tam mohou nachazet), zjisti se PDI daneho paketu
    for (int i = 0; i < packety.size(); i++) {
        packety.at(i).analysePacket();
    }


    //Analyzovat packety, dostavat z nich potrebne informace
    for (int i = 0; i < packety.size(); i++) {
        uint32_t pid = packety.at(i).getPid();
        if (pid == 0x10) //NIT TABULKA
            packety.at(i).analyseNITPacket();
        else if (pid == 0x00) //PAT TABULKA
            packety.at(i).analysePATPacket();
    }

    //zjistovani informaci o jednoltinotlivych kanalech, zpracovani PMT packetu
    for (int j = 0; j < MPEGPacket::programy.size(); j++) {
        if (MPEGPacket::programy.at(j).getDone()) // pro dany program uz se to provedlo
            continue;
        uint32_t pid = MPEGPacket::programy.at(j).getpmPID();
        for (int i = 0; i < packety.size(); i++) {
            uint32_t pidPacket = packety.at(i).getPid();
            if (pidPacket == pid)
                packety.at(i).analysePMTPacket(j, pid);
        }

    }

    //analyza PCR, zjisteni bitrate streamu
    double progBitrate = bitrate(&packety);

    //spojeni packetu (jejich telicek pro SDT) //TODO popremyslej jestli neudelat i pro ostatni
    mergeSDT(&packety);

    for (int i = 0; i < packety.size(); i++) {
        uint32_t pid = packety.at(i).getPid();
        if (pid == 0x11) //SDT TABULKA
            packety.at(i).analyseSDTPacket();
    }

    //spocitani bitaru pro kazdy program
    programsBitrate(&packety, progBitrate);


    if (!MPEGPacket::isNIT) {
        cerr << "NIT tabulka nenalezena" << endl;
        return 1;
    }

    if (!MPEGPacket::isPAT) {
        cerr << "PAT tabulka nenalezena" << endl;
        return 1;
    }

    if (!MPEGPacket::isSDT) {
        cerr << "SDT tabulka nenalezena" << endl;
        return 1;
    }

    bool ok = printInfo(&packety, soubor);
    if (!ok) {
        cerr << "Chyba pri zapisu vystupu do souboru" << endl;
        return 1;
    }

        return 0;
}