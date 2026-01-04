#ifndef BUKU_H
#define BUKU_H

#include <string>
using namespace std;

// Struktur data Buku - DEFINISI TUNGGAL untuk seluruh project
struct Buku {
    int id;
    string judul;
    string penulis;
    int tahun;
    string isbn;
    string kategori;
    string status;
    int harga;           // Harga buku dalam Rupiah
    int stok;            // Jumlah stok tersedia
    int totalTerjual;    // Total unit terjual
};

#endif
