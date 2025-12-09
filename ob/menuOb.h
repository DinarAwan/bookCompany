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
    void simpankeFile(); // Sesuai deklarasi Anda (k kecil)
    void loadDariFile();
};

// ===================== LAPORAN PEKERJAAN =====================
struct laporan {
    int id;
    string tanggal;
    string jenis;
    string lama;
    string laporanHariIni;
    laporan* next;

    // Constructor ini yang diimplementasikan di .cpp
    laporan(int _id, string _t, string _j, string _l, string _h);
};

// ===================== MENU OB =====================
class MenuOb {
private:
    laporan* head;
    AbsenOb absen; // Komposisi class AbsenOb

public:
    MenuOb();
    void loadDariFile();
    void simpanKeFile();
    int getNextId();

    void tambahLaporan();
    void lihatLaporan();
    void hapusLaporan();
    void laporKerusakanFasilitas();

    // Fitur antrian tugas dari admin
    void muatAntrian();      // load queue dari file
    void simpanAntrian();    // simpan queue ke file
    void lihatTugasSaya();
    void selesaikanTugas();

    void tampilkanMenu();
      // Fitur Queue Tugas
    void lihatTugasSaya();
    void selesaikanTugas();
    
    void tampilkanMenu();
};

  
#endif

