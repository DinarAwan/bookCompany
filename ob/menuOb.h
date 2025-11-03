#ifndef MENUOB_H
#define MENUOB_H

#include <string>
using namespace std;

// ===================== CLASS ABSEN OB =====================
class AbsenOb {
public:
    AbsenOb();
    void absenMasuk();
    void tampilAbsenSendiri(int idCari);

private:
    struct Absen {
        int id;
        string nama;
        string tanggal;
        string waktuMasuk;
        string waktuKeluar;
        Absen* next;
    };
    Absen* head;

    string ambilWaktuSekarang();
    string ambilTanggalSekarang();
};

// ===================== CLASS MENU OB =====================
class MenuOb {
public:
    MenuOb();
    void tampilkanMenu();

private:
    struct laporan {
        int id;
        string tanggal;
        string JenisPekerjaan;
        string LamaPekerjaan;
        string laporanHariIni;
        laporan* next;
        laporan(int id_, string t, string j, string l, string lap)
            : id(id_), tanggal(t), JenisPekerjaan(j),
              LamaPekerjaan(l), laporanHariIni(lap), next(nullptr) {}
    };

    laporan* head;
    AbsenOb absen;

    void simpanKeFile();
    void loadDariFile();
    int getNextId();
    void tambahLaporan();
    void lihatLaporan();
    void hapusLaporan();
    void laporKerusakanFasilitas();
};

#endif