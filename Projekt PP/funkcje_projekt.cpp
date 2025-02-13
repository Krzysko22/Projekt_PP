#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

namespace funkcje {
    const int MAKS_ZAINTERESOWAN = 10;
    const int MAKS_OSOB = 100;
    const string NAZWA_PLIKU = "adresy.dat";

    struct Zainteresowanie {
        char nazwa[50];
    };

    struct Osoba {
        char imie[50];
        char nazwisko[50];
        char ulica[100];
        char miasto[50];
        char kodPocztowy[10];
        char email[100];
        Zainteresowanie zainteresowania[MAKS_ZAINTERESOWAN];
        int liczbaZainteresowan;
    };

    struct GrupaZainteresowania {
        char nazwa[50];
        int czlonkowie[MAKS_OSOB];
        int liczbaOsob;
    };

    Osoba osoby[MAKS_OSOB];
    int liczbaOsob = 0;

    void wczytajZPliku() {
        ifstream plik(NAZWA_PLIKU, ios::binary);
        if (plik.is_open()) {
            plik.read(reinterpret_cast<char*>(&liczbaOsob), sizeof(liczbaOsob));
            plik.read(reinterpret_cast<char*>(osoby), liczbaOsob * sizeof(Osoba));
            plik.close();
        }
    }

    void zapiszDoPliku() {
        ofstream plik(NAZWA_PLIKU, ios::binary);
        if (plik.is_open()) {
            plik.write(reinterpret_cast<char*>(&liczbaOsob), sizeof(liczbaOsob));
            plik.write(reinterpret_cast<char*>(osoby), liczbaOsob * sizeof(Osoba));
            plik.close();
        }
    }

    void ustawMenuKodowanie() {
        system("chcp 65001");
    }

    void ustawOperacyjneKodowanie() {
        system("chcp 1250");
    }

    void wstep() {
        system("CLS");
        ustawMenuKodowanie();
        cout << "========================================================================================" << endl;
        cout << "                System wspomagający zarządzanie zbiorem adresów osób.                 " << endl;
        cout << "========================================================================================" << endl;
    }

    int menu() {
        wstep();
        int wybor;
        cout << "\nSystem zarządzania adresami\n";
        cout << "1. Dodaj osobę\n";
        cout << "2. Usuń osobę\n";
        cout << "3. Modyfikuj dane osoby\n";
        cout << "4. Wyświetl wszystkie osoby\n";
        cout << "5. Wyświetl grupy zainteresowań\n";
        cout << "0. Wyjście\n";
        cout << "Wybierz opcję: ";
        cin >> wybor;
        ustawOperacyjneKodowanie();
        return wybor;
    }

    void wczytajDaneOsobowe(Osoba& osoba) {
        cout << "Podaj imi\352: ";
        cin >> osoba.imie;
        cout << "Podaj nazwisko: ";
        cin >> osoba.nazwisko;
    }

    void wczytajAdres(Osoba& osoba) {
        cout << "Podaj ulice: ";
        cin.ignore();
        cin.getline(osoba.ulica, 100);
        cout << "Podaj miasto: ";
        cin.getline(osoba.miasto, 50);
        cout << "Podaj kod pocztowy: ";
        cin >> osoba.kodPocztowy;
    }

    void wczytajKontakt(Osoba& osoba) {
        cout << "Podaj email: ";
        cin >> osoba.email;
    }

    void wczytajZainteresowania(Osoba& osoba) {
        cout << "Ile zainteresowa\361 chcesz doda\346? ";
        cin >> osoba.liczbaZainteresowan;
        osoba.liczbaZainteresowan = min(osoba.liczbaZainteresowan, MAKS_ZAINTERESOWAN);

        for (int i = 0; i < osoba.liczbaZainteresowan; i++) {
            cout << "Podaj nazwe zainteresowania " << i+1 << ": ";
            cin >> osoba.zainteresowania[i].nazwa;
        }
    }

    void dodawanie() {
        system("CLS");
        ustawOperacyjneKodowanie();

        if (liczbaOsob >= MAKS_OSOB) {
            cout << "Osiągnięto maksymalną liczbę osób!\n";
            return;
        }

        Osoba& nowaOsoba = osoby[liczbaOsob];
        wczytajDaneOsobowe(nowaOsoba);
        wczytajAdres(nowaOsoba);
        wczytajKontakt(nowaOsoba);
        wczytajZainteresowania(nowaOsoba);

        liczbaOsob++;
        zapiszDoPliku();
        cout << "Dodano now\271 osob\352.\n";
        system("pause");
    }

    void inicjalizujGrupe(GrupaZainteresowania& grupa, const char* nazwa, int pierwszyCzlonek) {
        strcpy(grupa.nazwa, nazwa);
        grupa.czlonkowie[0] = pierwszyCzlonek;
        grupa.liczbaOsob = 1;
    }

    void dodajCzlonkaDoGrupy(GrupaZainteresowania& grupa, int indeksOsoby) {
        grupa.czlonkowie[grupa.liczbaOsob++] = indeksOsoby;
    }

    void wyswietlCzlonkowGrupy(const GrupaZainteresowania& grupa) {
        cout << "\nGrupa: " << grupa.nazwa << "\n";
        cout << "Cz\263onkowie:\n";
        for (int j = 0; j < grupa.liczbaOsob; j++) {
            int indeks = grupa.czlonkowie[j];
            cout << "- " << osoby[indeks].imie << " " << osoby[indeks].nazwisko << "\n";
        }
    }

    void przygotujGrupy(GrupaZainteresowania* grupy, int& liczbaGrup) {
        liczbaGrup = 0;

        for (int i = 0; i < liczbaOsob; i++) {
            for (int j = 0; j < osoby[i].liczbaZainteresowan; j++) {
                int znalezionoGrupe = 0;

                for (int k = 0; k < liczbaGrup; k++) {
                    if (strcmp(grupy[k].nazwa, osoby[i].zainteresowania[j].nazwa) == 0) {
                        dodajCzlonkaDoGrupy(grupy[k], i);
                        znalezionoGrupe = 1;
                        break;
                    }
                }

                if (!znalezionoGrupe) {
                    inicjalizujGrupe(grupy[liczbaGrup], osoby[i].zainteresowania[j].nazwa, i);
                    liczbaGrup++;
                }
            }
        }
    }

    int pobierzIndeksOsoby(const string& akcja) {
        cout << "Podaj numer osoby do " << akcja << " (1-" << liczbaOsob << "): ";
        int indeks;
        cin >> indeks;
        return indeks - 1;
    }

    int czyIndeksPoprawny(int indeks) {
        if (indeks < 0 || indeks >= liczbaOsob) {
            cout << "Nieprawidłowy numer!\n";
            return 0;
        }
        return 1;
    }

    int czyZostawicStaraWartosc(const string& nowaWartosc) {
        return nowaWartosc == "." ? 1 : 0;
    }

    void modyfikujPodstawoweDane(Osoba& osoba) {
        string temp;
        cout << "Podaj nowe imi\352 (lub . aby zostawic): ";
        cin >> temp;
        if (!czyZostawicStaraWartosc(temp)) strcpy(osoba.imie, temp.c_str());

        cout << "Podaj nowe nazwisko (lub . aby zostawic): ";
        cin >> temp;
        if (!czyZostawicStaraWartosc(temp)) strcpy(osoba.nazwisko, temp.c_str());
    }

    void modyfikujKontakt(Osoba& osoba) {
        string temp;
        cout << "Podaj nowy email (lub . aby zostawic): ";
        cin >> temp;
        if (!czyZostawicStaraWartosc(temp)) strcpy(osoba.email, temp.c_str());
    }

    void wyswietlanie() {
        system("CLS");
        ustawOperacyjneKodowanie();

        if (liczbaOsob == 0) {
            cout << "Brak osob w bazie!\n";
            return;
        }

        for (int i = 0; i < liczbaOsob; i++) {
            const Osoba& osoba = osoby[i];
            cout << "\nOsoba " << i+1 << ":\n";
            cout << "Imi\352 i nazwisko: " << osoba.imie << " " << osoba.nazwisko << "\n";
            cout << "Adres: " << osoba.ulica << ", " << osoba.miasto << " " << osoba.kodPocztowy << "\n";
            cout << "Email: " << osoba.email << "\n";
            cout << "Zainteresowania:\n";
            for (int j = 0; j < osoba.liczbaZainteresowan; j++) {
                cout << "  - " << osoba.zainteresowania[j].nazwa << "\n";
            }
        }
        system("pause");
    }

    void wyswietlGrupy() {
        system("CLS");
        ustawOperacyjneKodowanie();

        if (liczbaOsob == 0) {
            cout << "Brak osób w bazie!\n";
            return;
        }

        GrupaZainteresowania grupy[MAKS_ZAINTERESOWAN * MAKS_OSOB];
        int liczbaGrup;

        przygotujGrupy(grupy, liczbaGrup);

        cout << "\nGrupy zainteresowa\361:\n";
        for (int i = 0; i < liczbaGrup; i++) {
            wyswietlCzlonkowGrupy(grupy[i]);
        }
        system("pause");
    }

    void modyfikujAdres(Osoba& osoba) {
        string temp;
        cin.ignore();
        cout << "Podaj now\271 ulic\352 (lub . aby zostawi\346): ";
        getline(cin, temp);
        if (!czyZostawicStaraWartosc(temp)) strcpy(osoba.ulica, temp.c_str());

        cout << "Podaj nowe miasto (lub . aby zostawi\346): ";
        getline(cin, temp);
        if (!czyZostawicStaraWartosc(temp)) strcpy(osoba.miasto, temp.c_str());

        cout << "Podaj nowy kod pocztowy (lub . aby zostawi\346): ";
        cin >> temp;
        if (!czyZostawicStaraWartosc(temp)) strcpy(osoba.kodPocztowy, temp.c_str());
    }

    void modyfikujZainteresowania(Osoba& osoba) {
        char wybor;
        cout << "Czy chcesz zmodyfikowa\346 zainteresowania? (t/n): ";
        cin >> wybor;

        if (wybor == 't' || wybor == 'T') {
            cout << "Obecne zainteresowania:\n";
            for (int i = 0; i < osoba.liczbaZainteresowan; i++) {
                cout << i + 1 << ". " << osoba.zainteresowania[i].nazwa << "\n";
            }

            cout << "\nWybierz operacj\352:\n";
            cout << "1. Dodaj nowe zainteresowanie\n";
            cout << "2. Usu\361 zainteresowanie\n";
            cout << "3. Modyfikuj istniej\271ce zainteresowanie\n";
            cout << "4. Zast\271p wszystkie zainteresowania\n";
            int operacja;
            cin >> operacja;

            switch (operacja) {
                case 1: {
                    if (osoba.liczbaZainteresowan < MAKS_ZAINTERESOWAN) {
                        cout << "Podaj nazw\352 nowego zainteresowania: ";
                        cin >> osoba.zainteresowania[osoba.liczbaZainteresowan].nazwa;
                        osoba.liczbaZainteresowan++;
                    } else {
                        cout << "Osiągni\352to maksymalną liczbę zainteresowań!\n";
                    }
                    break;
                }
                case 2: {
                    cout << "Podaj numer zainteresowania do usuni\352cia (1-" << osoba.liczbaZainteresowan << "): ";
                    int indeks;
                    cin >> indeks;
                    indeks--;

                    if (indeks >= 0 && indeks < osoba.liczbaZainteresowan) {
                        for (int i = indeks; i < osoba.liczbaZainteresowan - 1; i++) {
                            strcpy(osoba.zainteresowania[i].nazwa, osoba.zainteresowania[i + 1].nazwa);
                        }
                        osoba.liczbaZainteresowan--;
                    }
                    break;
                }
                case 3: {
                    cout << "Podaj numer zainteresowania do modyfikacji (1-" << osoba.liczbaZainteresowan << "): ";
                    int indeks;
                    cin >> indeks;
                    indeks--;

                    if (indeks >= 0 && indeks < osoba.liczbaZainteresowan) {
                        cout << "Podaj now\271 nazw\352 zainteresowania: ";
                        cin >> osoba.zainteresowania[indeks].nazwa;
                    }
                    break;
                }
                case 4: {
                    cout << "Ile nowych zainteresowa\361 chcesz doda\346? ";
                    cin >> osoba.liczbaZainteresowan;
                    osoba.liczbaZainteresowan = min(osoba.liczbaZainteresowan, MAKS_ZAINTERESOWAN);

                    for (int i = 0; i < osoba.liczbaZainteresowan; i++) {
                        cout << "Podaj nazw\352 zainteresowania " << i + 1 << ": ";
                        cin >> osoba.zainteresowania[i].nazwa;
                    }
                    break;
                }
            }
        }
    }

    void modyfikowanie() {
        system("CLS");
        ustawOperacyjneKodowanie();

        if (liczbaOsob == 0) {
            cout << "Brak osób w bazie!\n";
            return;
        }

        int indeks = pobierzIndeksOsoby("modyfikacji");
        if (!czyIndeksPoprawny(indeks)) return;

        Osoba& osoba = osoby[indeks];

        cout << "\nAktualne dane osoby:\n";
        cout << "Imi\352 i nazwisko: " << osoba.imie << " " << osoba.nazwisko << "\n";
        cout << "Adres: " << osoba.ulica << ", " << osoba.miasto << " " << osoba.kodPocztowy << "\n";
        cout << "Email: " << osoba.email << "\n";
        cout << "Zainteresowania:\n";
        for (int i = 0; i < osoba.liczbaZainteresowan; i++) {
            cout << "  - " << osoba.zainteresowania[i].nazwa << "\n";
        }

        cout << "\nModyfikacja danych (wpisz . aby zostawi\346 bez zmian):\n";

        modyfikujPodstawoweDane(osoba);
        modyfikujAdres(osoba);
        modyfikujKontakt(osoba);
        modyfikujZainteresowania(osoba);

        zapiszDoPliku();
        cout << "Zmodyfikowano dane osoby.\n";
        system("pause");
    }

    void usuwanie() {
        system("CLS");
        ustawOperacyjneKodowanie();

        if (liczbaOsob == 0) {
            cout << "Brak osób w bazie!\n";
            return;
        }

        int indeks = pobierzIndeksOsoby("usuni\352cia");
        if (!czyIndeksPoprawny(indeks)) return;

        for (int i = indeks; i < liczbaOsob - 1; i++) {
            osoby[i] = osoby[i + 1];
        }
        liczbaOsob--;
        zapiszDoPliku();
        cout << "Usuni\352to osobe.\n";
        system("pause");
    }
}
