#include "MenuSuperAdmin.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// ==================== MENU MANAJEMEN LAPORAN & KOMPRESI ====================
void SuperAdmin::menuManajemenLaporan() {
    int pilihan;
    do {
        cout << "\n======= MANAJEMEN LAPORAN & KOMPRESI HUFFMAN =======\n";
        cout << "=== BACA LAPORAN ===\n";
        cout << "1. Laporan Kasir\n";
        cout << "2. Laporan Karyawan\n";
        cout << "3. Semua Laporan\n";
        cout << "\n=== HUFFMAN COMPRESSION ===\n";
        cout << "4. Kompresi Laporan\n";
        cout << "5. Dekompresi Laporan\n";
        cout << "6. Analisis Frekuensi Karakter\n";
        cout << "7. Visualisasi Huffman Tree\n";
        cout << "8. Statistik Kompresi\n";
        cout << "0. Kembali\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();
        
        switch(pilihan) {
            case 1: bacaLaporanKasir(); break;
            case 2: bacaLaporanKaryawan(); break;
            case 3: bacaSemuaLaporan(); break;
            case 4: kompresiLaporan(); break;
            case 5: dekompresiLaporan(); break;
            case 6: analisisFrequensi(); break;
            case 7: visualisasiHuffmanTree(); break;
            case 8: statistikKompresi(); break;
            case 0: cout << "Kembali ke menu utama...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
}

// ==================== BACA LAPORAN KASIR ====================
void SuperAdmin::bacaLaporanKasir() {
    cout << "\n========== LAPORAN KASIR ==========\n";
    
    ifstream file("laporan_kasir.txt");
    if (!file.is_open()) {
        cout << "Belum ada laporan kasir.\n";
        return;
    }
    
    string line;
    int nomor = 1;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string tanggal, nik, nama, sTransaksi, sTerjual, sDipinjam, sPendapatan, catatan;
        
        getline(ss, tanggal, '|');
        getline(ss, nik, '|');
        getline(ss, nama, '|');
        getline(ss, sTransaksi, '|');
        getline(ss, sTerjual, '|');
        getline(ss, sDipinjam, '|');
        getline(ss, sPendapatan, '|');
        getline(ss, catatan);
        
        cout << "\n--- Laporan #" << nomor++ << " ---\n";
        cout << "Tanggal       : " << tanggal << "\n";
        cout << "NIK           : " << nik << "\n";
        cout << "Nama          : " << nama << "\n";
        cout << "Total Transaks: " << sTransaksi << "\n";
        cout << "Buku Terjual  : " << sTerjual << "\n";
        cout << "Buku Dipinjam : " << sDipinjam << "\n";
        cout << "Pendapatan    : Rp " << sPendapatan << "\n";
        cout << "Catatan       : " << catatan << "\n";
        cout << "--------------------------------\n";
    }
    file.close();
    
    if (nomor == 1) {
        cout << "Belum ada laporan.\n";
    }
}

// ==================== BACA LAPORAN KARYAWAN ====================
void SuperAdmin::bacaLaporanKaryawan() {
    cout << "\n========== LAPORAN KARYAWAN ==========\n";
    
    ifstream file("laporan_karyawan.txt");
    if (!file.is_open()) {
        cout << "Belum ada laporan karyawan.\n";
        return;
    }
    
    string line;
    int nomor = 1;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string tanggal, nik, nama, sDitambah, sDikurangi, aktivitas, masalah;
        
        getline(ss, tanggal, '|');
        getline(ss, nik, '|');
        getline(ss, nama, '|');
        getline(ss, sDitambah, '|');
        getline(ss, sDikurangi, '|');
        getline(ss, aktivitas, '|');
        getline(ss, masalah);
        
        cout << "\n--- Laporan #" << nomor++ << " ---\n";
        cout << "Tanggal       : " << tanggal << "\n";
        cout << "NIK           : " << nik << "\n";
        cout << "Nama          : " << nama << "\n";
        cout << "Buku Ditambah : " << sDitambah << "\n";
        cout << "Stok Dikurangi: " << sDikurangi << "\n";
        cout << "Aktivitas     : " << aktivitas << "\n";
        cout << "Masalah       : " << masalah << "\n";
        cout << "--------------------------------\n";
    }
    file.close();
    
    if (nomor == 1) {
        cout << "Belum ada laporan.\n";
    }
}

// ==================== BACA SEMUA LAPORAN ====================
void SuperAdmin::bacaSemuaLaporan() {
    cout << "\n========== SEMUA LAPORAN ==========\n";
    cout << "\n[KASIR]:\n";
    bacaLaporanKasir();
    cout << "\n[KARYAWAN]:\n";
    bacaLaporanKaryawan();
}

// ==================== KOMPRESI LAPORAN ====================
void SuperAdmin::kompresiLaporan() {
    cout << "\n===== KOMPRESI LAPORAN HUFFMAN =====\n";
    cout << "Pilih laporan yang akan dikompres:\n";
    cout << "1. Kasir\n";
    cout << "2. Karyawan\n";
    cout << "Pilihan: ";
    
    int pilihan;
    cin >> pilihan;
    cin.ignore();
    
    string filename, outputFile;
    switch(pilihan) {
        case 1: 
            filename = "laporan_kasir.txt";
            outputFile = "compressed/laporan_kasir.huff";
            break;
        case 2: 
            filename = "laporan_karyawan.txt";
            outputFile = "compressed/laporan_karyawan.huff";
            break;
        default:
            cout << "Pilihan tidak valid!\n";
            return;
    }
    
    // Baca file
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "File tidak ditemukan!\n";
        return;
    }
    
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string content = buffer.str();
    inputFile.close();
    
    if (content.empty()) {
        cout << "File kosong!\n";
        return;
    }
    
    int originalSize = content.length();
    
    // Build Huffman tree dan compress
    cout << "\nMembangun Huffman Tree...\n";
    huffmanTree->buildFrequencyTable(content);
    huffmanTree->buildHuffmanTree();
    
    cout << "Melakukan kompresi...\n";
    string compressed = huffmanTree->compress(content);
    
    // Save compressed file
    huffmanTree->saveCompressed(outputFile, compressed);
    
    int compressedSize = (compressed.length() + 7) / 8; // Convert bits to bytes
    
    cout << "\n✓ Kompresi berhasil!\n";
    huffmanTree->displayCompressionStats(originalSize, compressedSize);
}

// ==================== DEKOMPRESI LAPORAN ====================
void SuperAdmin::dekompresiLaporan() {
    cout << "\n===== DEKOMPRESI LAPORAN =====\n";
    cout << "Masukkan nama file .huff (tanpa path): ";
    string filename;
    getline(cin, filename);
    
    string fullPath = "compressed/" + filename;
    
    // Load compressed file
    cout << "\nMemuat file terkompresi...\n";
    string compressedData = huffmanTree->loadCompressed(fullPath);
    
    if (compressedData.empty()) {
        cout << "Gagal memuat file!\n";
        return;
    }
    
    // Decompress
    cout << "Melakukan dekompresi...\n";
    string decompressed = huffmanTree->decompress(compressedData);
    
    // Display
    cout << "\n===== ISI FILE TERDEKOMPRESI =====\n";
    cout << decompressed << "\n";
    cout << "==================================\n";
}

// ==================== ANALISIS FREKUENSI ====================
void SuperAdmin::analisisFrequensi() {
    cout << "\n===== ANALISIS FREKUENSI KARAKTER =====\n";
    cout << "Pilih file untuk analisis:\n";
    cout << "1. Kasir\n";
    cout << "2. Karyawan\n";
    cout << "Pilihan: ";
    
    int pilihan;
    cin >> pilihan;
    cin.ignore();
    
    string filename;
    switch(pilihan) {
        case 1: filename = "laporan_kasir.txt"; break;
        case 2: filename = "laporan_karyawan.txt"; break;
        default:
            cout << "Pilihan tidak valid!\n";
            return;
    }
    
    // Baca file
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File tidak ditemukan!\n";
        return;
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();
    
    if (content.empty()) {
        cout << "File kosong!\n";
        return;
    }
    
    // Build table dan tree
    huffmanTree->buildFrequencyTable(content);
    huffmanTree->displayFrequencyTable();
    
    huffmanTree->buildHuffmanTree();
    cout << "\n";
    huffmanTree->displayCodes();
}

// ==================== VISUALISASI HUFFMAN TREE ====================
void SuperAdmin::visualisasiHuffmanTree() {
    if (huffmanTree->getRoot() == NULL) {
        cout << "\nBelum ada Huffman tree. Lakukan kompresi atau analisis terlebih dahulu.\n";
        return;
    }
    
    huffmanTree->visualizeTree();
}

// ==================== STATISTIK KOMPRESI ====================
void SuperAdmin::statistikKompresi() {
    int origSize = huffmanTree->getOriginalSize();
    int compSize = huffmanTree->getCompressedSize();
    
    if (origSize == 0) {
        cout << "\nBelum ada data kompresi. Lakukan kompresi terlebih dahulu.\n";
        return;
    }
    
    huffmanTree->displayCompressionStats(origSize, compSize);
}
