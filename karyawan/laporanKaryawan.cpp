#include "menuKaryawan.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;

// ==================== IMPLEMENTASI LAPORAN HARIAN KARYAWAN ====================
void MenuKaryawan::buatLaporanHarian(string nik, string nama) {
    cout << "\n===== BUAT LAPORAN HARIAN KARYAWAN ===== \n";
    
    // Auto-generate tanggal
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string tanggal = to_string(ltm->tm_mday) + "-" + 
                     to_string(1 + ltm->tm_mon) + "-" + 
                     to_string(1900 + ltm->tm_year);
    
    // Input manual NIK dan Nama
    cout << "\n--- Input Data Pembuat Laporan ---\n";
    cout << "Masukkan NIK Anda: ";
    getline(cin, nik);
    cout << "Masukkan Nama Anda: ";
    getline(cin, nama);
    
    int bukuDitambah, stokDikurangi;
    string aktivitas, masalah;
    
    cout << "\n--- Data Laporan ---\n";
    cout << "Tanggal: " << tanggal << "\n";
    cout << "NIK: " << nik << "\n";
    cout << "Nama: " << nama << "\n\n";
    
    cout << "Jumlah Buku Ditambah Hari Ini: ";
    cin >> bukuDitambah;
    cout << "Stok Dikurangi: ";
    cin >> stokDikurangi;
    cin.ignore();
    cout << "Aktivitas Hari Ini: ";
    getline(cin, aktivitas);
    cout << "Masalah yang Ditemukan (jika ada): ";
    getline(cin, masalah);
    
    // Save ke file
    ofstream file("laporan_karyawan.txt", ios::app);
    if (file.is_open()) {
        file << tanggal << "|" 
             << nik << "|" 
             << nama << "|" 
             << bukuDitambah << "|" 
             << stokDikurangi << "|" 
             << aktivitas << "|" 
             << masalah << endl;
        file.close();
        cout << "\n✓ Laporan berhasil disimpan!\n";
    } else {
        cout << "\n✗ Gagal menyimpan laporan!\n";
    }
}
