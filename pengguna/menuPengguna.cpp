#include "menuPengguna.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "../auth/ManajemenUser.h"
#include "../KTP/ktp.h"

#include "../Buku.h"

using namespace std;

class ManajemenBuku {
private:
    Buku* bukuDaftar;
    int jumlahBuku;
    int kapasitas;

    void perbesarKapasitas() {
        kapasitas *= 2;
        Buku* bukuBaru = new Buku[kapasitas];
        for (int i = 0; i < jumlahBuku; i++) {
            bukuBaru[i] = bukuDaftar[i];
        }
        delete[] bukuDaftar;
        bukuDaftar = bukuBaru;
    }

    // Fungsi untuk memuat data dari file
    void muatDariFile() {
        ifstream file("buku.txt");
        if (!file.is_open()) return; // jika belum ada file, abaikan

        while (true) { 
            Buku buku;
            if (!(file >> buku.id)) break; // Baca ID, jika gagal (eof), keluar
            file.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan newline
            
            if (!getline(file, buku.judul)) break;
            if (!getline(file, buku.penulis)) break;
            
            if (!(file >> buku.tahun)) break;
            file.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan newline
            
            if (!getline(file, buku.isbn)) break;
            if (!getline(file, buku.kategori)) break;
            
            // Baca status
            if (!getline(file, buku.status)) {
                 buku.status = "Tersedia"; // Default jika baris status tidak ada (file lama)
            }
            
            // Validasi status
            if (buku.status.empty() || (buku.status != "Tersedia" && buku.status != "Dipinjam") ) {
                buku.status = "Tersedia";
            }

            // Baca harga, stok, totalTerjual
            if (!(file >> buku.harga)) buku.harga = 0;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!(file >> buku.stok)) buku.stok = 0;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!(file >> buku.totalTerjual)) buku.totalTerjual = 0;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            if (jumlahBuku >= kapasitas) {
                perbesarKapasitas();
            }
            bukuDaftar[jumlahBuku++] = buku;
        }
        file.close();
    }

    // Fungsi untuk menyimpan data ke file
    // Dibiarkan apa adanya sesuai permintaan Anda,
    // meskipun Pengguna idealnya tidak menyimpan ulang (write) ke buku.txt
    void simpanKeFile() {
        ofstream file("buku.txt", ios::trunc);
        for (int i = 0; i < jumlahBuku; i++) {
            file << bukuDaftar[i].id << "\n";
            file << bukuDaftar[i].judul << "\n";
            file << bukuDaftar[i].penulis << "\n";
            file << bukuDaftar[i].tahun << "\n";
            file << bukuDaftar[i].isbn << "\n";
            file << bukuDaftar[i].kategori << "\n";
            file << bukuDaftar[i].status << "\n";
            file << bukuDaftar[i].harga << "\n";
            file << bukuDaftar[i].stok << "\n";
            file << bukuDaftar[i].totalTerjual << "\n";
        }
        file.close();
    }

public:
    ManajemenBuku() {
        kapasitas = 10;
        jumlahBuku = 0;
        bukuDaftar = new Buku[kapasitas];
        muatDariFile(); // load data saat awal
    }

    ~ManajemenBuku() {
        // simpanKeFile(); // Sebaiknya jangan simpan otomatis saat pengguna logout
        delete[] bukuDaftar;
        // Catatan: Saya menonaktifkan simpanKeFile() di destruktor
        // karena Pengguna tidak seharusnya mengubah buku.txt
        // Jika Anda tetap ingin menyimpannya, hapus '//' di atas.
    }

    
    
    void lihatbuku() {
        if (jumlahBuku == 0) {
            cout << "Tidak ada data buku tersedia" << endl;
            return;
            
        }

        cout << "\n===============================================" << endl;
        cout << "                  DAFTAR BUKU                  " << endl;
        cout << "===============================================" << endl;
        
        for (int i = 0; i < jumlahBuku; i++) {
            cout << "Buku #" << (i+1) << endl;
            cout << "   ID            : " << bukuDaftar[i].id << endl;
            cout << "   Judul         : " << bukuDaftar[i].judul << endl;
            cout << "   Penulis       : " << bukuDaftar[i].penulis << endl;
            cout << "   Tahun Terbit  : " << bukuDaftar[i].tahun << endl;
            cout << "   ISBN          : " << bukuDaftar[i].isbn << endl;
            cout << "   Kategori      : " << bukuDaftar[i].kategori << endl;
            cout << "   Harga         : Rp " << bukuDaftar[i].harga << endl;
            cout << "   Stok          : " << bukuDaftar[i].stok << " unit" << endl;
            cout << "   Status        : " << bukuDaftar[i].status << endl;
            cout << "-----------------------------------------------" << endl;
        }
    }

    // <-- FUNGSI BARU: Cari Buku -->
    void cariBuku() {
        string kataKunci;
        cout << "============= Cari Buku =============" << endl;
        cout << "Masukkan Judul atau Penulis untuk dicari: ";
        getline(cin, kataKunci); // Ambil input setelah cin >> pilihan

        if (kataKunci.empty()) {
            cout << "Kata kunci tidak boleh kosong." << endl;
            return;
        }

        bool ditemukan = false;
        cout << "\n--- Hasil Pencarian ---" << endl;
        for (int i = 0; i < jumlahBuku; i++) {
            // Pencarian sederhana (case-sensitive)
            // Cek di judul ATAU di penulis
            if (bukuDaftar[i].judul.find(kataKunci) != string::npos || 
                bukuDaftar[i].penulis.find(kataKunci) != string::npos) 
            {
                cout << "Buku #" << (i+1) << endl;
                cout << "   ID            : " << bukuDaftar[i].id << endl;
                cout << "   Judul         : " << bukuDaftar[i].judul << endl;
                cout << "   Penulis       : " << bukuDaftar[i].penulis << endl;
                cout << "   Tahun Terbit  : " << bukuDaftar[i].tahun << endl;
                cout << "   ISBN          : " << bukuDaftar[i].isbn << endl;
                cout << "   Kategori      : " << bukuDaftar[i].kategori << endl;
                cout << "   Harga         : Rp " << bukuDaftar[i].harga << endl;
                cout << "   Stok          : " << bukuDaftar[i].stok << " unit" << endl;
                cout << "   Status        : " << bukuDaftar[i].status << endl;
                cout << "-----------------------------------------------" << endl;
                ditemukan = true;
            }
        }

        if (!ditemukan) {
            cout << "Buku dengan kata kunci '" << kataKunci << "' tidak ditemukan." << endl;
        }
    }

    // <-- FUNGSI BARU: Reservasi Buku -->
    void reservasiBuku() {
        int idReservasi;
        cout << "============= Reservasi Buku =============" << endl;
        cout << "Masukkan ID Buku yang ingin direservasi: ";
        
        if (!(cin >> idReservasi)) {
            cout << "Input tidak valid. Masukkan angka.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan newline

        // 1. Cari buku di daftar
        int indexBuku = -1;
        for (int i = 0; i < jumlahBuku; i++) {
            if (bukuDaftar[i].id == idReservasi) {
                indexBuku = i;
                break;
            }
        }

        if (indexBuku == -1) {
            cout << "Buku dengan ID " << idReservasi << " tidak ditemukan." << endl;
            return;
        }

        // 2. Cek status buku
        if (bukuDaftar[indexBuku].status == "Tersedia") {
            cout << "Buku '" << bukuDaftar[indexBuku].judul << "' tersedia dan bisa langsung dipinjam." << endl;
            cout << "Tidak perlu reservasi." << endl;
            return;
        }

        // 3. Jika status "Dipinjam", proses reservasi
        if (bukuDaftar[indexBuku].status == "Dipinjam") {
            // 4. Cek apakah sudah pernah direservasi di file
            ifstream cekFile("reservasi.txt");
            int idCek;
            bool sudahReservasi = false;
            if (cekFile.is_open()) {
                while (cekFile >> idCek) {
                    if (idCek == idReservasi) {
                        sudahReservasi = true;
                        break;
                    }
                }
                cekFile.close();
            }

            if (sudahReservasi) {
                cout << "Buku '" << bukuDaftar[indexBuku].judul << "' sudah ada dalam daftar reservasi Anda." << endl;
            } else {
                // 5. Tulis reservasi ke file
                ofstream file("reservasi.txt", ios::app); // app = append (menambahkan)
                if (!file.is_open()) {
                    cout << "Gagal membuka file reservasi. Coba lagi." << endl;
                    return;
                }
                file << idReservasi << "\n"; // Simpan ID bukunya
                file.close();

                cout << "Buku '" << bukuDaftar[indexBuku].judul << "' berhasil direservasi." << endl;
                cout << "Anda akan dihubungi jika buku sudah tersedia." << endl;
            }
        }
    }
};


void MenuPengguna::tampilkanMenu() {
    ManajemenBuku x;
    int pilihan;
    do {
        // DIUBAH: Judul Menu
        cout << "========== MENU PENGGUNA ==========\n";
        cout << "Selamat Datang!\n";
        cout << "1. Lihat Semua Buku\n";
        cout << "2. Cari Buku (Katalog)\n";
        cout << "3. Reservasi Buku\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        // DIUBAH: Menambahkan validasi input
        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Pilih menu: "; // Tanya lagi
        }
        
        // Membersihkan buffer setelah input angka
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                cout << "Menampilkan semua data buku...\n\n";
                x.lihatbuku();
                break;
            case 2: // <-- DIUBAH
                cout << "Mencari buku...\n\n";
                x.cariBuku();
                break;
            case 3: // <-- DIUBAH
                cout << "Membuat reservasi...\n\n";
                x.reservasiBuku();
                break;
            case 0:
                cout << "Logout berhasil!\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}

// Static method untuk registrasi pengguna baru
void MenuPengguna::registrasi() {
    ManajemenUser manajemenUser;
    KTP userBaru;
    
    cout << "\n========== REGISTRASI PENGGUNA BARU ==========\n";
    
    // Input NIK
    cout << "Masukkan NIK: ";
    cin >> userBaru.nik;
    
    // Cek apakah NIK sudah ada
    if (manajemenUser.isNIKExists(userBaru.nik)) {
        cout << "Error: NIK sudah terdaftar! Silakan login.\n";
        cin.ignore(1000, '\n');
        return;
    }
    
    cin.ignore(1000, '\n');
    
    // Input Nama
    cout << "Masukkan Nama Lengkap: ";
    getline(cin, userBaru.nama);
    
    // Input Tanggal Lahir
    cout << "Masukkan Tanggal Lahir (hari bulan tahun): ";
    cin >> userBaru.tanggalLahir.hari 
        >> userBaru.tanggalLahir.bulan 
        >> userBaru.tanggalLahir.tahun;
    
    // Input Jenis Kelamin
    cout << "Masukkan Jenis Kelamin (1: LAKI-LAKI, 2: PEREMPUAN): ";
    int jk;
    cin >> jk;
    userBaru.jenisKelamin = (jk == 1) ? LAKI_LAKI : PEREMPUAN;
    cin.ignore(1000, '\n');
    
    // Input Alamat
    cout << "Masukkan Alamat: ";
    getline(cin, userBaru.alamat);
    
    // Input Agama
    cout << "Masukkan Agama (1:ISLAM, 2:KRISTEN, 3:KATOLIK, 4:HINDU, 5:BUDDHA, 6:KHONGHUCU): ";
    int ag;
    cin >> ag;
    userBaru.agama = (Agama)ag;
    
    // Input Status Perkawinan
    cout << "Masukkan Status Perkawinan (1: BELUM KAWIN, 2: KAWIN): ";
    int st;
    cin >> st;
    userBaru.statusPerkawinan = (st == 1) ? BELUM_KAWIN : KAWIN;
    cin.ignore(1000, '\n');
    
    // Input Pekerjaan
    cout << "Masukkan Pekerjaan: ";
    getline(cin, userBaru.pekerjaan);
    
    // Input Kewarganegaraan
    cout << "Kewarganegaraan (1: WNI, 2: WNA): ";
    int kw;
    cin >> kw;
    userBaru.kewarganegaraan = (kw == 1) ? WNI : WNA;
    cin.ignore(1000, '\n');
    
    // Role otomatis = 3 (Pengguna)
    userBaru.role = 3;
    
    // Input Password
    cout << "Masukkan Password: ";
    getline(cin, userBaru.password);
    
    // Konfirmasi Password
    string konfirmasiPassword;
    cout << "Konfirmasi Password: ";
    getline(cin, konfirmasiPassword);
    
    if (userBaru.password != konfirmasiPassword) {
        cout << "\nError: Password tidak cocok!\n";
        return;
    }
    
    // Tambahkan user
    if (manajemenUser.tambahUser(userBaru)) {
        cout << "\n========================================\n";
        cout << "Registrasi Berhasil!\n";
        cout << "========================================\n";
        cout << "NIK: " << userBaru.nik << "\n";
        cout << "Nama: " << userBaru.nama << "\n";
        cout << "Role: PENGGUNA\n";
        cout << "\nAnda dapat login sekarang dengan NIK dan password Anda.\n";
        cout << "========================================\n";
    } else {
        cout << "\nGagal melakukan registrasi!\n";
    }
}
