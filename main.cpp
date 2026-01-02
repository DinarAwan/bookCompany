#include <iostream>
#include "auth/Login.h"
#include "admin/MenuAdmin.h"
#include "superAdmin/MenuSuperAdmin.h"
#include "pengguna/menuPengguna.h"
#include "satpam/MenuSatpam.h"
#include "karyawan/menuKaryawan.h"
#include "kasir/menuKasir.h"
#include "ob/menuOb.h"

using namespace std;

int main() {
    int pilihanAwal;
    
    do {
        cout << "\n==========================================\n";
        cout << "|                                        |\n";
        cout << "|   SELAMAT DATANG DI DEMEN BACA         |\n";
        cout << "|                                        |\n";
        cout << "==========================================\n";
        cout << "1. Login\n";
        cout << "2. Registrasi (Pengguna Baru)\n";
        cout << "0. Exit\n";
        cout << "Pilih menu: ";
        cin >> pilihanAwal;
        
        if (pilihanAwal == 1) {
            // Proses Login
            Login login;
            login.prosesLogin();
            
            int role = login.getRole();
            cin.ignore(1000, '\n');
            
            if (role == 1) {
                MenuAdmin adminMenu;
                adminMenu.tampilkanMenu();
            } 
            else if (role == 2) {
                MenuSuperAdmin superMenu;
                superMenu.tampilkanMenu();
            } else if (role == 3) {
                MenuPengguna penggunaMenu;
                penggunaMenu.tampilkanMenu();
            } else if (role == 4) {
                MenuSatpam satpamMenu;
                satpamMenu.tampilkanMenu();
            } else if (role == 5) {
                MenuKaryawan karyawanMenu;
                karyawanMenu.tampilkanMenu();
            } else if (role == 6) {
                MenuKasir kasirMenu;
                kasirMenu.tampilkanMenu();
            } else if (role == 7) {
                MenuOb obMenu;
                obMenu.tampilkanMenu();
            }
            else {
                cout << "Login gagal!\n";
            }
        } 
        else if (pilihanAwal == 2) {
            // Proses Registrasi
            MenuPengguna::registrasi();
            cin.ignore(1000, '\n');
        }
        else if (pilihanAwal == 0) {
            cout << "Terima kasih telah menggunakan DEMEN BACA!\n";
        }
        else {
            cout << "Pilihan tidak valid!\n";
        }
        
    } while (pilihanAwal != 0);

    return 0;
}