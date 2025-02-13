#include "funkcje_projekt.cpp"
using namespace funkcje;

int main() {
    ustawMenuKodowanie();
    wczytajZPliku();

    int wybor = -1;
    while (wybor != 0) {
        wybor = menu();
        switch (wybor) {
            case 1:
                dodawanie();
                break;
            case 2:
                usuwanie();
                break;
            case 3:
                modyfikowanie();
                break;
            case 4:
                wyswietlanie();
                break;
            case 5:
                wyswietlGrupy();
                break;
        }
    }
    return 0;
}
