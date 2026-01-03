#ifndef MENUKARYAWAN_H
#define MENUKARYAWAN_H

#include <string>
using namespace std;

class MenuKaryawan {
public:
    void tampilkanMenu();
    void buatLaporanHarian(string nik, string nama);  // New: laporan
};

#endif
