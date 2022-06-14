//POP 2021-01-22 projekt 2 Nowogrodzki Adrian AIR 1 184332
//CodeBlocks 20.03

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct BITMAPFILEHEADER
{
  short bfType;
  int bfSize;
  short bfReserved1;
  short bfReserved2;
  int bfOffBits;
};

struct BITMAPINFOHEADER
{
  int biSize;
  int biWidth;
  int biHeight;
  short biPlanes;
  short biBitCount;
  int biCompression;
  int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  int biClrUsed;
  int biClrImportant;
};

struct RGB{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

void ilosc_niewaznych_intow(int &il_niew_int, BITMAPFILEHEADER& bfh){               //dane po naglowku i przed tablica pixelii
    if((bfh.bfOffBits-54) % 4) il_niew_int = (bfh.bfOffBits-54)/4 + 1;
    else il_niew_int = (bfh.bfOffBits-54)/4;
}

void czytanie_header(int niewazne[], int &il_niew_int, fstream& plik, BITMAPFILEHEADER& bfh, BITMAPINFOHEADER& bih){
    plik.read((char*)(&bfh.bfType), sizeof(bfh.bfType));
    plik.read((char*)(&bfh.bfSize), sizeof(bfh.bfSize));                //odczytuje rozmiar pliku w bajtach
    plik.read((char*)(&bfh.bfReserved1), sizeof(bfh.bfReserved1));
    plik.read((char*)(&bfh.bfReserved2), sizeof(bfh.bfReserved2));
    plik.read((char*)(&bfh.bfOffBits), sizeof(bfh.bfOffBits));          //odczytuje adres pierwszego pixela

    plik.read((char*)(&bih.biSize), sizeof(bih.biSize));
    plik.read((char*)(&bih.biWidth), sizeof(bih.biWidth));              //odczytuje szerokosc pliku w px
    plik.read((char*)(&bih.biHeight), sizeof(bih.biHeight));            //odczytuje wysokosc pliku w px
    plik.read((char*)(&bih.biPlanes), sizeof(bih.biPlanes));
    plik.read((char*)(&bih.biBitCount), sizeof(bih.biBitCount));        //ilosc bitow na jeden pixel
    plik.read((char*)(&bih.biCompression), sizeof(bih.biCompression));
    plik.read((char*)(&bih.biSizeImage), sizeof(bih.biSizeImage));
    plik.read((char*)(&bih.biXPelsPerMeter), sizeof(bih.biXPelsPerMeter));
    plik.read((char*)(&bih.biYPelsPerMeter), sizeof(bih.biYPelsPerMeter));
    plik.read((char*)(&bih.biClrUsed), sizeof(bih.biClrUsed));
    plik.read((char*)(&bih.biClrImportant), sizeof(bih.biClrImportant));

    ilosc_niewaznych_intow(il_niew_int, bfh);

    for(int i=0; i<il_niew_int; i++)
    plik.read((char*)(&niewazne[i]), sizeof(int));
}

void pisanie_header(int niewazne[], int &il_niew_int, fstream& plik_z, BITMAPFILEHEADER& bfh, BITMAPINFOHEADER& bih){
    plik_z.write((char*)(&bfh.bfType), sizeof(bfh.bfType));
    plik_z.write((char*)(&bfh.bfSize), sizeof(bfh.bfSize));
    plik_z.write((char*)(&bfh.bfReserved1), sizeof(bfh.bfReserved1));
    plik_z.write((char*)(&bfh.bfReserved2), sizeof(bfh.bfReserved2));
    plik_z.write((char*)(&bfh.bfOffBits), sizeof(bfh.bfOffBits));

    plik_z.write((char*)(&bih.biSize), sizeof(bih.biSize));
    plik_z.write((char*)(&bih.biWidth), sizeof(bih.biWidth));
    plik_z.write((char*)(&bih.biHeight), sizeof(bih.biHeight));
    plik_z.write((char*)(&bih.biPlanes), sizeof(bih.biPlanes));
    plik_z.write((char*)(&bih.biBitCount), sizeof(bih.biBitCount));
    plik_z.write((char*)(&bih.biCompression), sizeof(bih.biCompression));
    plik_z.write((char*)(&bih.biSizeImage), sizeof(bih.biSizeImage));
    plik_z.write((char*)(&bih.biXPelsPerMeter), sizeof(bih.biXPelsPerMeter));
    plik_z.write((char*)(&bih.biYPelsPerMeter), sizeof(bih.biYPelsPerMeter));
    plik_z.write((char*)(&bih.biClrUsed), sizeof(bih.biClrUsed));
    plik_z.write((char*)(&bih.biClrImportant), sizeof(bih.biClrImportant));

    for(int i=0; i<il_niew_int; i++)
    plik_z.write((char*)(&niewazne[i]), sizeof(int));
}

void pisanie_px(int il_px, fstream& plik_z, RGB pixel[], BITMAPFILEHEADER& bfh){            //zapisywanie pixeli (po jednym)
        plik_z.write((char*)(&pixel[0].B), sizeof(pixel[0].B));
        plik_z.write((char*)(&pixel[0].G), sizeof(pixel[0].G));
        plik_z.write((char*)(&pixel[0].R), sizeof(pixel[0].R));
        plik_z.write((char*)(&pixel[0].A), sizeof(pixel[0].A));
}

void pisanie_px_2d(fstream& plik_z, unsigned char pixel_2d_1[], BITMAPINFOHEADER& bih){          //zapisywanie pixeli z 3 akcji (po jednym)

            plik_z.write((char*)(&pixel_2d_1[0]), 1);
            plik_z.write((char*)(&pixel_2d_1[1]), 1);
            plik_z.write((char*)(&pixel_2d_1[2]), 1);
            plik_z.write((char*)(&pixel_2d_1[3]), 1);
}

void odczytywanie_nowego_pliku(fstream& plik_z, BITMAPFILEHEADER& bfh_new, BITMAPINFOHEADER& bih_new){      //czytanie wagi i wymiarow  nowego pliku
    plik_z.seekg(2);
    plik_z.read((char*)(&bfh_new.bfSize), sizeof(bfh_new.bfSize));                //odczytuje rozmiar pliku w bajtach
    plik_z.seekg(12, ios::cur);
    plik_z.read((char*)(&bih_new.biWidth), sizeof(bih_new.biWidth));              //odczytuje szerokosc pliku w px
    plik_z.read((char*)(&bih_new.biHeight), sizeof(bih_new.biHeight));            //odczytuje wysokosc pliku w px
}

void ilosc_px(int &il_px, BITMAPINFOHEADER& bih){             //ilosc wszystkich pixeli
    il_px = bih.biWidth * bih.biHeight;
}

void czytanie_px_i_akcje(int il_px, int akcja, fstream& plik, RGB pixel[], BITMAPFILEHEADER& bfh, fstream& plik_z);     //deklaracja funkcji
void czytanie_px_i_akcja_trzecia(BITMAPINFOHEADER& bih, RGB **pixel_2d, fstream& plik, fstream& plik_z);                //deklaracja funckji

void wybor_akcji(int il_px, fstream& plik, RGB pixel[], BITMAPFILEHEADER& bfh, fstream& plik_z, BITMAPINFOHEADER& bih, RGB **pixel_2d){  //wybieranie akcji
    int akcja;
    cout << "Co chcesz zrobic? " << endl;
    cout << "1. Przejsc do skali szarosci." << endl
         << "2. Wyodrebnic jeden kolor." << endl
         << "3. Rozmyc obraz." << endl
         << " Twoj wybor: ";
    do{
        cin >> akcja;
    }while(akcja>3 || akcja<1);

    switch(akcja)
    {
        case 1: czytanie_px_i_akcje(il_px, akcja, plik, pixel, bfh, plik_z) ;break;
        case 2: czytanie_px_i_akcje(il_px, akcja, plik, pixel, bfh, plik_z) ;break;
        case 3: czytanie_px_i_akcja_trzecia(bih, pixel_2d, plik, plik_z);break;
        default: ;break;
    }
}

void akcja_1(RGB pixel[]);                                           //deklaracja funkcji
void akcja_2(RGB pixel[], int kolor);                                           //deklaracja funkcji
void wybierz_kolor(int& kolor);                                             //deklaracja funkcji

void czytanie_px_i_akcje(int il_px, int akcja, fstream& plik, RGB pixel[], BITMAPFILEHEADER& bfh, fstream& plik_z){   //wczytywanie danych RGB o kazdym pixelu
    plik.seekg(bfh.bfOffBits);                                                                                        //        i wykonywanie akcji
    int kolor;
    if(akcja==2){
        wybierz_kolor(kolor);
    }
    for(int i=0; i<il_px; i++){
        plik.read((char*)(&pixel[0].B), sizeof(pixel[0].B));        //czyta skladowe pixela, zapisuje je do jedynej zmiennej w talbicy
        plik.read((char*)(&pixel[0].G), sizeof(pixel[0].G));
        plik.read((char*)(&pixel[0].R), sizeof(pixel[0].R));
        plik.read((char*)(&pixel[0].A), sizeof(pixel[0].A));

        if(akcja==1){
            akcja_1(pixel);                  //wlacza 1 akcje
            pisanie_px(il_px, plik_z, pixel, bfh);                  //zapisuje dane tego pixela do nowego pliku
        }
        else if(akcja==2){
            akcja_2(pixel, kolor);           //wlacza 2 akcje
            pisanie_px(il_px, plik_z, pixel, bfh);                  //zapisuje dane tego pixela do nowego pliku
        }
    }
}

void akcja_1(RGB pixel[]){
    int szary_px = (pixel[0].B+ pixel[0].G+ pixel[0].R)/3;  //srednia wartosc koloru
    pixel[0].B = szary_px;              //zapis szarosci do RGB
    pixel[0].G = szary_px;              //
    pixel[0].R = szary_px;              //
}

void wybierz_kolor(int& kolor){                             //wybieranie koloru dla akcji nr2
    cout << "Podaj kolor do wyodrebnienia: " << endl
         << "1. niebieski" << endl
         << "2. zielony" << endl
         << "3. czerwony" << endl
         << "Twoj wybor: ";
    do{
        cin >> kolor;
    }while(kolor<1 || kolor>3);
}

void akcja_2(RGB pixel[], int kolor){
    if(kolor==1){                               //kolor niebieski
        pixel[0].G=0;
        pixel[0].R=0;
    }
    else if (kolor==2){                         //kolor zielony
        pixel[0].B=0;
        pixel[0].R=0;
    }
    else{                                       //kolor czerwony
        pixel[0].G=0;
        pixel[0].B=0;
    }
}

void czytanie_px_i_akcja_trzecia(BITMAPINFOHEADER& bih, RGB **pixel_2d, fstream& plik, fstream& plik_z){        //rozmycie
    for(int j=0; j<bih.biHeight; j++){
        for(int i=0; i<bih.biWidth; i++){
            plik.read((char*)(&pixel_2d[i][j].B), sizeof(pixel_2d[i][j].B));            //czyta wszystkie pixele do talicy 2d
            plik.read((char*)(&pixel_2d[i][j].G), sizeof(pixel_2d[i][j].G));            //
            plik.read((char*)(&pixel_2d[i][j].R), sizeof(pixel_2d[i][j].R));            //
            plik.read((char*)(&pixel_2d[i][j].A), sizeof(pixel_2d[i][j].A));            //
        }
    }
    unsigned char b[5];           //do zapisania sasiednich pixeli
    unsigned char r[5];           //
    unsigned char g[5];           //
    unsigned char rozmycie[4];
    unsigned char pixel_2d_1[4];
    for(int j=0; j<bih.biHeight; j++){
        for(int i=0; i<bih.biWidth; i++){
            if(i==0 && j==0){                               //lewy gorny
                b[0]=pixel_2d[0][0].B;
                b[1]=pixel_2d[1][0].B;
                b[2]=pixel_2d[0][1].B;
                g[0]=pixel_2d[0][0].G;
                g[1]=pixel_2d[1][0].G;
                g[2]=pixel_2d[0][1].G;
                r[0]=pixel_2d[0][0].R;
                r[1]=pixel_2d[1][0].R;
                r[2]=pixel_2d[0][1].R;
                rozmycie[0]=(b[0]+b[1]+b[2])/3;
                rozmycie[1]=(g[0]+g[1]+g[2])/3;
                rozmycie[2]=(r[0]+r[1]+r[2])/3;
            }
            else if(i==0 && j==(bih.biHeight-1)){            //lewy dolny
                b[0]=pixel_2d[0][bih.biHeight-1].B;
                b[1]=pixel_2d[1][bih.biHeight-1].B;
                b[2]=pixel_2d[0][bih.biHeight-2].B;
                g[0]=pixel_2d[0][bih.biHeight-1].G;
                g[1]=pixel_2d[1][bih.biHeight-1].G;
                g[2]=pixel_2d[0][bih.biHeight-2].G;
                r[0]=pixel_2d[0][bih.biHeight-1].R;
                r[1]=pixel_2d[1][bih.biHeight-1].R;
                r[2]=pixel_2d[0][bih.biHeight-2].R;
                rozmycie[0]=(b[0]+b[1]+b[2])/3;
                rozmycie[1]=(g[0]+g[1]+g[2])/3;
                rozmycie[2]=(r[0]+r[1]+r[2])/3;
            }
            else if(i==(bih.biWidth-1) && j==0){            //prawy gorny
                b[0]=pixel_2d[bih.biWidth-1][0].B;
                b[1]=pixel_2d[bih.biWidth-2][0].B;
                b[2]=pixel_2d[bih.biWidth-1][1].B;
                g[0]=pixel_2d[bih.biWidth-1][0].G;
                g[1]=pixel_2d[bih.biWidth-2][0].G;
                g[2]=pixel_2d[bih.biWidth-1][1].G;
                r[0]=pixel_2d[bih.biWidth-1][0].R;
                r[1]=pixel_2d[bih.biWidth-2][0].R;
                r[2]=pixel_2d[bih.biWidth-1][1].R;
                rozmycie[0]=(b[0]+b[1]+b[2])/3;
                rozmycie[1]=(g[0]+g[1]+g[2])/3;
                rozmycie[2]=(r[0]+r[1]+r[2])/3;
            }
            else if(i==(bih.biWidth-1) && j==(bih.biHeight-1)){     //prawy dolny
                b[0]=pixel_2d[bih.biWidth-1][bih.biHeight-1].B;
                b[1]=pixel_2d[bih.biWidth-2][bih.biHeight-1].B;
                b[2]=pixel_2d[bih.biWidth-1][bih.biHeight-2].B;
                g[0]=pixel_2d[bih.biWidth-1][bih.biHeight-1].G;
                g[1]=pixel_2d[bih.biWidth-2][bih.biHeight-1].G;
                g[2]=pixel_2d[bih.biWidth-1][bih.biHeight-2].G;
                r[0]=pixel_2d[bih.biWidth-1][bih.biHeight-1].R;
                r[1]=pixel_2d[bih.biWidth-2][bih.biHeight-1].R;
                r[2]=pixel_2d[bih.biWidth-1][bih.biHeight-2].R;
                rozmycie[0]=(b[0]+b[1]+b[2])/3;
                rozmycie[1]=(g[0]+g[1]+g[2])/3;
                rozmycie[2]=(r[0]+r[1]+r[2])/3;
            }
            else if(i==0 && j<(bih.biHeight-1) && j>0){            //lewy bok
                b[0]=pixel_2d[i][j].B;
                b[1]=pixel_2d[i][j-1].B;
                b[2]=pixel_2d[i][j+1].B;
                b[3]=pixel_2d[i+1][j].B;
                g[0]=pixel_2d[i][j].G;
                g[1]=pixel_2d[i][j-1].G;
                g[2]=pixel_2d[i][j+1].G;
                g[3]=pixel_2d[i+1][j].G;
                r[0]=pixel_2d[i][j].R;
                r[1]=pixel_2d[i][j-1].R;
                r[2]=pixel_2d[i][j+1].R;
                r[3]=pixel_2d[i+1][j].R;
                rozmycie[0]=(b[0]+b[1]+b[2]+b[3])/4;
                rozmycie[1]=(g[0]+g[1]+g[2]+g[3])/4;
                rozmycie[2]=(r[0]+r[1]+r[2]+r[3])/4;
            }
            else if(i==(bih.biWidth-1) && j<(bih.biHeight-1) && j>0){            //prawy bok
                b[0]=pixel_2d[i][j].B;
                b[1]=pixel_2d[i][j-1].B;
                b[2]=pixel_2d[i][j+1].B;
                b[3]=pixel_2d[i-1][j].B;
                g[0]=pixel_2d[i][j].G;
                g[1]=pixel_2d[i][j-1].G;
                g[2]=pixel_2d[i][j+1].G;
                g[3]=pixel_2d[i-1][j].G;
                r[0]=pixel_2d[i][j].R;
                r[1]=pixel_2d[i][j-1].R;
                r[2]=pixel_2d[i][j+1].R;
                r[3]=pixel_2d[i-1][j].R;
                rozmycie[0]=(b[0]+b[1]+b[2]+b[3])/4;
                rozmycie[1]=(g[0]+g[1]+g[2]+g[3])/4;
                rozmycie[2]=(r[0]+r[1]+r[2]+r[3])/4;
            }
            else if(i<(bih.biWidth-1) && i>0 && j==0){            //gora
                b[0]=pixel_2d[i][j].B;
                b[1]=pixel_2d[i-1][j].B;
                b[2]=pixel_2d[i+1][j].B;
                b[3]=pixel_2d[i][j+1].B;
                g[0]=pixel_2d[i][j].G;
                g[1]=pixel_2d[i-1][j].G;
                g[2]=pixel_2d[i+1][j].G;
                g[3]=pixel_2d[i][j+1].G;
                r[0]=pixel_2d[i][j].R;
                r[1]=pixel_2d[i-1][j].R;
                r[2]=pixel_2d[i+1][j].R;
                r[3]=pixel_2d[i][j+1].R;
                rozmycie[0]=(b[0]+b[1]+b[2]+b[3])/4;
                rozmycie[1]=(g[0]+g[1]+g[2]+g[3])/4;
                rozmycie[2]=(r[0]+r[1]+r[2]+r[3])/4;
            }
            else if(i<(bih.biWidth-1) && i>0 && j==(bih.biHeight-1)){            //dol
                b[0]=pixel_2d[i][j].B;
                b[1]=pixel_2d[i-1][j].B;
                b[2]=pixel_2d[i+1][j].B;
                b[3]=pixel_2d[i][j-1].B;
                g[0]=pixel_2d[i][j].G;
                g[1]=pixel_2d[i-1][j].G;
                g[2]=pixel_2d[i+1][j].G;
                g[3]=pixel_2d[i][j-1].G;
                r[0]=pixel_2d[i][j].R;
                r[1]=pixel_2d[i-1][j].R;
                r[2]=pixel_2d[i+1][j].R;
                r[3]=pixel_2d[i][j-1].R;
                rozmycie[0]=(b[0]+b[1]+b[2]+b[3])/4;
                rozmycie[1]=(g[0]+g[1]+g[2]+g[3])/4;
                rozmycie[2]=(r[0]+r[1]+r[2]+r[3])/4;
            }
            else{                                           //reszta
                b[0]=pixel_2d[i][j].B;
                b[1]=pixel_2d[i-1][j].B;
                b[2]=pixel_2d[i+1][j].B;
                b[3]=pixel_2d[i][j-1].B;
                b[4]=pixel_2d[i][j+1].B;
                g[0]=pixel_2d[i][j].G;
                g[1]=pixel_2d[i-1][j].G;
                g[2]=pixel_2d[i+1][j].G;
                g[3]=pixel_2d[i][j-1].G;
                g[4]=pixel_2d[i][j+1].G;
                r[0]=pixel_2d[i][j].R;
                r[1]=pixel_2d[i-1][j].R;
                r[2]=pixel_2d[i+1][j].R;
                r[3]=pixel_2d[i][j-1].R;
                r[4]=pixel_2d[i][j+1].R;
                rozmycie[0]=(b[0]+b[1]+b[2]+b[3]+b[4])/5;
                rozmycie[1]=(g[0]+g[1]+g[2]+g[3]+g[4])/5;
                rozmycie[2]=(r[0]+r[1]+r[2]+r[3]+r[4])/5;
            }
            pixel_2d_1[0]=rozmycie[0];          //zamiana int w BYTE
            pixel_2d_1[1]=rozmycie[1];
            pixel_2d_1[2]=rozmycie[2];
            pixel_2d_1[3]=pixel_2d[0][0].A;

            pisanie_px_2d(plik_z, pixel_2d_1, bih);
        }
    }
}

int main()
{
    int il_px;                  //ilosc pixeli
    int il_niew_int;            //wielokrotnosc 4bajtow(int) pomiedzy naglowkiem DIB i tablica pixeli
    int niewazne[1000];         //zapis nieistotnych danych pomiedzy naglowkiem DIB i tablica pixeli
    fstream plik, plik_z;       //plik do odczytu, plik do zapisu
    BITMAPFILEHEADER bfh, bfh_new;
    BITMAPINFOHEADER bih, bih_new;
    RGB *pixel = new RGB [1];      //dane RGB kazdego pixela

    RGB **pixel_2d = new RGB* [5000];      //dane RGB kazdego pixela w tablicy 2d
    for(int i=0; i<5000; i++){             //
        pixel_2d[i] = new RGB [5000];      //
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    string nazwa, nazwa2;
    cout << "Podaj nazwe pliku BMP (nazwa.bmp): ";
    cin >> nazwa;

    plik.open(nazwa.c_str(), ios::in | ios::binary);
    if(!plik){
        cout << "Blad przy otwieraniu";
        return 1;
    }

    cout << "Podaj nazwe docelowego pliku BMP (nazwa.bmp): ";
    cin >> nazwa2;

    plik_z.open(nazwa2.c_str(), ios::out | ios::binary);
    if(!plik_z){
        cout << "Blad przy otwieraniu";
        return 2;
    }
/////////
    czytanie_header(niewazne, il_niew_int, plik, bfh, bih);
    pisanie_header(niewazne, il_niew_int, plik_z, bfh, bih);
/////////
    ilosc_px(il_px, bih);
/////////
    wybor_akcji(il_px, plik, pixel, bfh, plik_z, bih, pixel_2d);
/////////
    plik.close();
    plik_z.close();
/////////
    plik_z.open(nazwa2.c_str(), ios::in | ios::binary);
    if(!plik_z){
        cout << "Blad przy otwieraniu";
        return 3;
    }
    odczytywanie_nowego_pliku(plik_z, bfh_new, bih_new);        //odczytywanie wielkosci i wymiarow
    plik_z.close();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << "------------------------------" << endl;
    cout << "Komwersja przebiegla pomyslnie" << endl;
    cout  << "Wielkosc pliku: " << bfh_new.bfSize/1024 << " KB" << endl;
    cout << "Wymiary obrazka: "<<   bih_new.biWidth  << "x" << bih_new.biHeight << endl;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delete[] pixel;
    for(int i = 0; i < 5000; i++){
        delete[] pixel_2d[i];
        pixel_2d[i] = NULL;
    }
    delete[] pixel_2d;
    return 0;
}
