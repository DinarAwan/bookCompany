#ifndef MENU_OB_H
#define MENU_OB_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// ===================== ABSENSI OB =====================
class AbsenOb {
public:
    struct Absen {
        int id;
        string nama;
        string tanggal;
        string waktuMasuk;
        string waktuKeluar;
        Absen* next;
    };

    Absen* head;

    AbsenOb();
    string ambilWaktuSekarang();
    string ambilTanggalSekarang();
    void absenMasuk();
    void tampilAbsenSendiri(int idCari);
    void absenKeluar();
    void simpankeFile();
    void loadDariFile();
};

// ===================== STRUCT LAPORAN =====================
struct laporan {
    int id;
    string tanggal;
    string jenis;
    string lama;
    string laporanHariIni;
    laporan* next;

    laporan(int _id, string _t, string _j, string _l, string _h);
};

// ===================== MENU OB =====================
class MenuOb {
private:
    laporan* head;
    AbsenOb absen;

public:
    MenuOb();
    void loadDariFile();
    void simpanKeFile();
    int getNextId();

    void tambahLaporan();
    void lihatLaporan();
    void hapusLaporan();
    void laporKerusakanFasilitas();
    void tampilkanMenu();
};

#endif
