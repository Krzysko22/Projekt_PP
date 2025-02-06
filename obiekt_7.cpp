#include <iostream>
#include <string>
using namespace std;

class Film {
private:
    string tytul;
    string rezyser;
    int czasTrwania;
    string aktorzy[5];
    int liczbaAktorow;

public:
    Film(string t = "", string r = "", int czas = 0) {
        tytul = t;
        rezyser = r;
        czasTrwania = czas;
        liczbaAktorow = 0;
    }

    void dodajAktora(string nazwisko) {
        if (liczbaAktorow < 5) {
            aktorzy[liczbaAktorow] = nazwisko;
            liczbaAktorow++;
        }
    }

    string getTytul() { return tytul; }
    int getCzasTrwania() { return czasTrwania; }

    int sprawdzAktora(string nazwisko) {
        for (int i = 0; i < liczbaAktorow; i++) {
            if (aktorzy[i] == nazwisko) return 1;
        }
        return 0;
    }

    void wyswietlDane() {
        cout << "\nTytu³: " << tytul << endl;
        cout << "Re¿yser: " << rezyser << endl;
        cout << "Czas trwania: " << czasTrwania << " minut" << endl;
        cout << "Aktorzy pierwszoplanowi:" << endl;
        for (int i = 0; i < liczbaAktorow; i++) {
            cout << "- " << aktorzy[i] << endl;
        }
    }
};

void wczytajFilmy(Film* filmy, int n) {
    string tytul, rezyser, aktor;
    int czas;

    for (int i = 0; i < n; i++) {
        cout << "\nWprowadŸ dane filmu nr " << i+1 << endl;
        cout << "Tytu³: ";
        cin.ignore();
        getline(cin, tytul);
        cout << "Re¿yser: ";
        getline(cin, rezyser);
        cout << "Czas trwania (minuty): ";
        cin >> czas;

        filmy[i] = Film(tytul, rezyser, czas);

        cout << "Ilu aktorów chcesz dodaæ? (max 5): ";
        int liczbaAktorow;
        cin >> liczbaAktorow;
        if (liczbaAktorow > 5) liczbaAktorow = 5;

        cin.ignore();
        for (int j = 0; j < liczbaAktorow; j++) {
            cout << "Podaj nazwisko aktora " << j+1 << ": ";
            getline(cin, aktor);
            filmy[i].dodajAktora(aktor);
        }
    }
}

void szukajFilmu(Film* filmy, int n, string szukanyTytul) {
    for (int i = 0; i < n; i++) {
        if (filmy[i].getTytul() == szukanyTytul) {
            cout << "\nZnaleziono film:" << endl;
            filmy[i].wyswietlDane();
            return;
        }
    }
    cout << "\nNie znaleziono filmu o tytule: " << szukanyTytul << endl;
}

void wyswietlWszystkieFilmy(Film* filmy, int n) {
    cout << "\nLista wszystkich filmów:" << endl;
    for (int i = 0; i < n; i++) {
        filmy[i].wyswietlDane();
    }
}

double obliczSredniCzas(Film* filmy, int n) {
    if (n == 0) return 0;

    int sumaCzasu = 0;
    for (int i = 0; i < n; i++) {
        sumaCzasu += filmy[i].getCzasTrwania();
    }
    return (double)sumaCzasu / n;
}

int policzWystapieniaAktora(Film* filmy, int n, string nazwisko) {
    int licznik = 0;
    for (int i = 0; i < n; i++) {
        if (filmy[i].sprawdzAktora(nazwisko)) {
            licznik++;
        }
    }
    return licznik;
}

void menu(Film* filmy, int n) {
    int wybor;
    string szukanyTytul, szukanyAktor;

    do {
        cout << "\nMenu:" << endl;
        cout << "1. Szukaj filmu po tytule" << endl;
        cout << "2. Wyœwietl wszystkie filmy" << endl;
        cout << "3. Oblicz œredni czas trwania filmów" << endl;
        cout << "4. SprawdŸ wystêpowanie aktora" << endl;
        cout << "5. Zakoñcz program" << endl;
        cout << "Wybór: ";
        cin >> wybor;

        switch(wybor) {
            case 1:
                cout << "Podaj tytu³ filmu: ";
                cin.ignore();
                getline(cin, szukanyTytul);
                szukajFilmu(filmy, n, szukanyTytul);
                break;
            case 2:
                wyswietlWszystkieFilmy(filmy, n);
                break;
            case 3:
                cout << "\nŒredni czas trwania filmów: " << obliczSredniCzas(filmy, n) << " minut" << endl;
                break;
            case 4:
                cout << "Podaj nazwisko aktora: ";
                cin.ignore();
                getline(cin, szukanyAktor);
                cout << "\nAktor wystêpuje w " << policzWystapieniaAktora(filmy, n, szukanyAktor) << " filmach" << endl;
                break;
        }
    } while (wybor != 5);
}

int main() {
    system("chcp 1250>>null");
    int n;
    cout << "Podaj liczbê filmów: ";
    cin >> n;

    Film* filmy = new Film[n];

    wczytajFilmy(filmy, n);
    menu(filmy, n);

    delete[] filmy;
    return 0;
}
