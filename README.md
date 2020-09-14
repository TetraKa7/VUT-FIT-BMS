# VUT FIT BMS

Projekty do předmětu Bezdrátové a mobilní sítě na VUT FIT 3. semestr, zima, magisterské studium

## 2018

Projekty z roku 2018

### Projekt 1

Naprogramujte v jazyku C/C++ dva konzolové programy (bms1A – modulátor, bms1B – demodulátor), které budou modulovat vstupní resp. demodulovat výstupní postupnost bitů za pomoci čtyřstavového ASK.

Vstupní posloupnost bitů modulátoru bude zadaná v textovém souboru, který může obsahovat jen znaky 0 a 1. Jako přenosové médium analogového signálu bude použit WAV soubor s jedním kanálem, přičemž amplituda nosného signálu bude 0x7f000000.

Parametry modulace zvolte následující:

- Frekvence nosné: 1000 Hz.
- Přenosová rychlost minimálně: 1000 bit/s.
- Na začátek přenášených dat přidejte synchronizační sekvenci pro demodulátor: 00110011.

Parametry demodulace zvolte následující:

- ASK s frekvencí nosné 1000 Hz.
Baudovou/přenosovou rychlost určete na základě synchronizační sekvence příchozího signálu.
Výstup demodulátoru tvoří opět textový soubor obsahující znaky 0 a 1 bez synchronizační sekvence.

Poznámky:

Při implementaci použijte knihovnu libsndfile na jednoduchou práci s WAV soubory.
Můžete použít připravenou kostru, která generuje analogový signál o frekvenci 1 kHz.
Demo vstup/výstup dostupný zde.
Vaše programy musí být schopné pracovat s referenčními vstupy, které jsou vytvořeny v souladu s definovanými parametry.
Při implementaci je možnost použít libovolnou knihovnu dostupnou na serveru merlin.fit.vutbr.cz, na kterém se budou vaše programy testovat. Při přejímání kódu z internetu uveďte zdroj alespoň v komentáři zdrojového kódu.

### Projekt 2

V jazyce C/C++ naprogramujte dva konzolové programy (bms2A - kodér, bms2B - dekodér), které budou implementovat vhodné zabezpečovací kódy (Error Correction Code). Parametry zabezpečení budou následující:

- Maximální velikost zabezpečeného souboru: 175% původní velikosti.
- Poškozená data můžou být náhodně rozložená v celém souboru – dekodér musí být schopen soubor opravit, i když je náhodně poškozeno X% symbolů z velikosti zabezpečeného souboru.
- Poškození může být i shlukové - dekodér musí zrekonstruovat původní soubor i při poškození X% z velikosti po sobě jdoucích bitů zabezpečeného souboru.
- Hodnota X je definovaná následovně:
  - 17 ≤ X ≤ 21 => plný počet bodů za projekt.
  - 21 < X < 25 => plný počet bodů za projekt + 1 bod ke zkoušce.
  - X ≥ 25 => plný počet bodů za projekt + 3 body ke zkoušce.
  
Další poznámky:
- Počítejte, že vstupní soubor bude mít minimálně 1MB.
- Je zakázáno používat knihovnu schifra.
- Použitý algoritmus: libovolný.
- Maximalní doba běhu programu při kódovaní/dekódovaní 1MB souboru je stanovená na 10 sekund.
Na vlastní testovaní použijte referenční program, vkladající chyby do souboru, dostupný zde, spustitelný na serveru merlin.
Při implementaci je možnost použít libovolnou knihovnu dostupnou na serveru merlin.fit.vutbr.cz, na kterém se budou vaše programy testovat. Předpokládejte pořadí bajtů Little-endian. Při přejímání kódu z internetu, uveďte zdroj alespoň v komentáři zdrojového kódu.


## 2019

Naprogramujte v jazyce C++/Python jednoduchou konzolovou aplikaci (bms1), která bude realizovat demultiplexing a analýzu transportního streamu vysílání digitální televize.

Parametry programu:
 - ./bms1 nazev_souboru.ts
 
Funkce programu:
Načítá vstupní soubor (nazev_souboru.ts), který obsahuje data transportního streamu DVB-T vysílání. Výstupem aplikace bude soubor s názvem nazev_souboru.txt, která bude obsahovat detaily pro každý vyextrahovaný kanál ze zpracovávaného transportního streamu.

Obsah a formát výstupu:

- Soubor bude obsahovat v hlavičce informace o zpracovávaném multiplexu získané z NIT servisní tabulky, detaily o jednotlivých programech získané z tabulek PAT a SDT, které budou doplněny o souhrnnou statistiku přenosové rychlosti pro všechny kanály, které patří k jednomu programu.
- Každý řádek popisující jeden program multiplexu bude mít následující formát:
```
PID-service_provider-service_name: <bitrate> Mbps
```
- Jednotlivé řádky budou seřazeny podle hodnoty PID.
- Bitrate počítejte na základě počtu paketů daného programu vzhledem k celkovému počtu paketů. Teda podle vzorce:
```
bitrate_programu = pocet_paketu_pro_program/celkovy_pocet_paketu*bitrate_streamu
```
- V případě, že program obsahuje více video/audio stop případně servisní informace, sčítejte bitrate všech těchto kanálu do společné hodnoty.
- Přenosovou rychlost zaokrouhlete na 2 desetinné místa.
Příklad:
```

              Network name: SIT 1 CESKA TELEVIZE
              Network ID: 12345
              Bandwidth: 8 MHz
              Constellation: 64-QAM
              Guard interval: 1/4
              Code rate: 2/3
              
              0x0100-Ceska televize-CT 1 JM: 10.50 Mbps
              ...
```             
Další poznámky
Při implementaci je možnost použít libovolnou knihovnu dostupnou na serveru merlin.fit.vutbr.cz, na kterém se budou vaše programy testovat.
Vzorový TS soubor naleznete zde.
