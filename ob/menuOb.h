#ifndef MENUOB_H
#define MENUOB_H

#include <string>
using namespace std;

// ===== CLASS LAPORAN =====
class laporan {
public:
    int id;
    string tanggal;
    string JenisPekerjaan;
    string LamaPekerjaan;
    string laporanHariIni;
    laporan* next;

    laporan(int id, string tanggal, string jenis, string lama, string laporanHarian)
        : id(id), tanggal(tanggal), JenisPekerjaan(jenis),
          LamaPekerjaan(lama), laporanHariIni(laporanHarian), next(nullptr) {}
};

// ===== CLASS MENU OB =====
class MenuOb {
private:
    laporan* head;

    void simpanKeFile();
    void loadDariFile();
    int getNextId();

public:
    MenuOb();
    void tambahLaporan();
    void lihatLaporan();
    void hapusLaporan();
    void tampilkanMenu();
};

#endif
