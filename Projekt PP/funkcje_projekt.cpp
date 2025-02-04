#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;
namespace funkcje {
const int MAX_INTERESTS = 10;
const int MAX_PEOPLE = 100;
const string FILENAME = "adresy.dat";

struct Interest {
    char name[50];
    char category[50];
};

struct Person {
    char firstName[50];
    char lastName[50];
    char street[100];
    char city[50];
    char postalCode[10];
    char email[100];
    Interest interests[MAX_INTERESTS];
    int interestCount;
};

Person people[MAX_PEOPLE];
int peopleCount = 0;

void wczytajZPliku() {
    ifstream file(FILENAME, ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&peopleCount), sizeof(peopleCount));
        file.read(reinterpret_cast<char*>(people), peopleCount * sizeof(Person));
        file.close();
    }
}

void zapiszDoPliku() {
    ofstream file(FILENAME, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&peopleCount), sizeof(peopleCount));
        file.write(reinterpret_cast<char*>(people), peopleCount * sizeof(Person));
        file.close();
    }
}
void wstep(){
    system("CLS");
    cout << "========================================================================================" << endl;
    cout << "                System wspomagającegy zarządzanie zbiorem adresów osób.                 " << endl;
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
    return wybor;
}

void dodawanie() {
    if (peopleCount >= MAX_PEOPLE) {
        cout << "Osiągnięto maksymalną liczbę osób!\n";
        return;
    }

    Person& newPerson = people[peopleCount];
    cout << "Podaj imię: ";
    cin >> newPerson.firstName;
    cout << "Podaj nazwisko: ";
    cin >> newPerson.lastName;
    cout << "Podaj ulicę: ";
    cin.ignore();
    cin.getline(newPerson.street, 100);
    cout << "Podaj miasto: ";
    cin.getline(newPerson.city, 50);
    cout << "Podaj kod pocztowy: ";
    cin >> newPerson.postalCode;
    cout << "Podaj email: ";
    cin >> newPerson.email;

    cout << "Ile zainteresowań chcesz dodać? ";
    cin >> newPerson.interestCount;
    newPerson.interestCount = min(newPerson.interestCount, MAX_INTERESTS);

    for (int i = 0; i < newPerson.interestCount; i++) {
        cout << "Podaj nazwę zainteresowania " << i+1 << ": ";
        cin >> newPerson.interests[i].name;
        cout << "Podaj kategorię zainteresowania " << i+1 << ": ";
        cin >> newPerson.interests[i].category;
    }

    peopleCount++;
    zapiszDoPliku();
    cout << "Dodano nową osobę.\n";
}
void wyswietlGrupy() {
    if (peopleCount == 0) {
        cout << "Brak osób w bazie!\n";
        return;
    }

    struct GrupaZainteresowania {
        char nazwa[50];
        int czlonkowie[MAX_PEOPLE];
        int liczbaOsob;
    } grupy[MAX_INTERESTS * MAX_PEOPLE];
    int liczbaGrup = 0;


    for (int i = 0; i < peopleCount; i++) {
        for (int j = 0; j < people[i].interestCount; j++) {
            bool znalezionoGrupe = false;


            for (int k = 0; k < liczbaGrup; k++) {
                if (strcmp(grupy[k].nazwa, people[i].interests[j].name) == 0) {
                    grupy[k].czlonkowie[grupy[k].liczbaOsob++] = i;
                    znalezionoGrupe = true;
                    break;
                }
            }

            if (!znalezionoGrupe) {
                strcpy(grupy[liczbaGrup].nazwa, people[i].interests[j].name);
                grupy[liczbaGrup].czlonkowie[0] = i;
                grupy[liczbaGrup].liczbaOsob = 1;
                liczbaGrup++;
            }
        }
    }

    cout << "\nGrupy zainteresowań:\n";
    for (int i = 0; i < liczbaGrup; i++) {
        cout << "\nGrupa: " << grupy[i].nazwa << "\n";
        cout << "Członkowie:\n";
        for (int j = 0; j < grupy[i].liczbaOsob; j++) {
            int index = grupy[i].czlonkowie[j];
            cout << "- " << people[index].firstName << " "
                     << people[index].lastName << "\n";
        }
    }
    system("pause");
}
void usuwanie() {
    if (peopleCount == 0) {
        cout << "Brak osób w bazie!\n";
        return;
    }

    cout << "Podaj numer osoby do usunięcia (1-" << peopleCount << "): ";
    int index;
    cin >> index;
    index--;

    if (index < 0 || index >= peopleCount) {
        cout << "Nieprawidłowy numer!\n";
        return;
    }

    for (int i = index; i < peopleCount - 1; i++) {
        people[i] = people[i + 1];
    }
    peopleCount--;
    zapiszDoPliku();
    cout << "Usunięto osobę.\n";
}

void modyfikowanie() {
    if (peopleCount == 0) {
        cout << "Brak osób w bazie!\n";
        return;
    }

    cout << "Podaj numer osoby do modyfikacji (1-" << peopleCount << "): ";
    int index;
    cin >> index;
    index--;

    if (index < 0 || index >= peopleCount) {
        cout << "Nieprawidłowy numer!\n";
        return;
    }

    Person& person = people[index];
    cout << "Podaj nowe imię (lub . aby zostawić): ";
    string temp;
    cin >> temp;
    if (temp != ".") strcpy(person.firstName, temp.c_str());

    cout << "Podaj nowe nazwisko (lub . aby zostawić): ";
    cin >> temp;
    if (temp != ".") strcpy(person.lastName, temp.c_str());

    cout << "Podaj nowy email (lub . aby zostawić): ";
    cin >> temp;
    if (temp != ".") strcpy(person.email, temp.c_str());

    zapiszDoPliku();
    cout << "Zmodyfikowano dane osoby.\n";
}

void wyswietlanie() {
    if (peopleCount == 0) {
        cout << "Brak osób w bazie!\n";
        return;
    }

    for (int i = 0; i < peopleCount; i++) {
        const Person& person = people[i];
        cout << "\nOsoba " << i+1 << ":\n";
        cout << "Imię i nazwisko: " << person.firstName << " " << person.lastName << "\n";
        cout << "Adres: " << person.street << ", " << person.city << " " << person.postalCode << "\n";
        cout << "Email: " << person.email << "\n";
        cout << "Zainteresowania:\n";
        for (int j = 0; j < person.interestCount; j++) {
            cout << "  - " << person.interests[j].name
                     << " (kategoria: " << person.interests[j].category << ")\n";
        }
    }
     system("pause");
}

}
