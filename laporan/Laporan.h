#ifndef LAPORAN_H
#define LAPORAN_H

#include <string>
using namespace std;

// Base struct untuk laporan umum
struct LaporanItem {
    string tanggal;         // Format: DD-MM-YYYY
    string nik;             // NIK pembuat laporan
    string nama;            // Nama pembuat laporan
    string role;            // Role: Kasir/Karyawan/Admin/OB/Satpam
    string konten;          // Konten laporan (multi-line)
    LaporanItem* next;      // Pointer ke laporan berikutnya
    
    LaporanItem() : next(NULL) {}
};

// Struct khusus untuk laporan Kasir
struct LaporanKasir {
    string tanggal;
    string nik;
    string nama;
    int totalTransaksi;
    int bukuTerjual;
    int bukuDipinjam;
    double totalPendapatan;
    string catatanTambahan;
    LaporanKasir* next;
    
    LaporanKasir() : next(NULL), totalTransaksi(0), bukuTerjual(0), 
                     bukuDipinjam(0), totalPendapatan(0.0) {}
};

// Struct khusus untuk laporan Karyawan
struct LaporanKaryawan {
    string tanggal;
    string nik;
    string nama;
    int bukuDitambah;
    int stokDikurangi;
    string aktivitas;       // Deskripsi aktivitas hari ini
    string masalah;         // Masalah yang ditemukan (jika ada)
    LaporanKaryawan* next;
    
    LaporanKaryawan() : next(NULL), bukuDitambah(0), stokDikurangi(0) {}
};

// Struct khusus untuk laporan Admin
struct LaporanAdmin {
    string tanggal;
    string nik;
    string nama;
    string aktivitasOperasional;
    int totalBukuDiproses;
    string keputusanDiambil;
    string masalah;
    LaporanAdmin* next;
    
    LaporanAdmin() : next(NULL), totalBukuDiproses(0) {}
};

// Struct khusus untuk laporan OB
struct LaporanOB {
    string tanggal;
    string nik;
    string nama;
    string areaDibersihkan;
    string kondisiFasilitas;
    string perbaikanDilakukan;
    string catatanTambahan;
    LaporanOB* next;
    
    LaporanOB() : next(NULL) {}
};

// Struct khusus untuk laporan Satpam
struct LaporanSatpam {
    string tanggal;
    string nik;
    string nama;
    int jumlahPengunjung;
    string waktuPatroli;
    string kejadianKhusus;
    string kondisiKeamanan;
    LaporanSatpam* next;
    
    LaporanSatpam() : next(NULL), jumlahPengunjung(0) {}
};

#endif
