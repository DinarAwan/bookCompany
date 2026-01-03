#include "menuKasir.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;

// ==================== IMPLEMENTASI LAPORAN HARIAN KASIR ====================
void MenuKasir::buatLaporanHarian(string nik, string nama) {
    cout << "\n===== BUAT LAPORAN HARIAN KASIR =====\n";
    
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
    
    int totalTransaksi, bukuTerjual, bukuDipinjam;
    double totalPendapatan;
    string catatan;
    
    cout << "\n--- Data Laporan ---\n";
    cout << "Tanggal: " << tanggal << "\n";
    cout << "NIK: " << nik << "\n";
    cout << "Nama: " << nama << "\n\n";
    
    cout << "Total Transaksi Hari Ini: ";
    cin >> totalTransaksi;
    cout << "Total Buku Terjual: ";
    cin >> bukuTerjual;
    cout << "Total Buku Dipinjam: ";
    cin >> bukuDipinjam;
    cout << "Total Pendapatan (Rp): ";
    cin >> totalPendapatan;
    cin.ignore();
    cout << "Catatan Tambahan: ";
    getline(cin, catatan);
    
    // Save ke file
    ofstream file("laporan_kasir.txt", ios::app);
    if (file.is_open()) {
        file << tanggal << "|" 
             << nik << "|" 
             << nama << "|" 
             << totalTransaksi << "|" 
             << bukuTerjual << "|" 
             << bukuDipinjam << "|" 
             << totalPendapatan << "|" 
             << catatan << endl;
        file.close();
        cout << "\n✓ Laporan berhasil disimpan!\n";
    } else {
        cout << "\n✗ Gagal menyimpan laporan!\n";
    }
}
