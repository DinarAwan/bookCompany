#include "Login.h"
#include <iostream>

using namespace std;

Login::Login() {
    role = 0;
    nik = 0;
    manajemenUser = new ManajemenUser();
}

Login::~Login() {
    delete manajemenUser;
}

void Login::prosesLogin() {
    cout << endl;
    cout << "==========================================\n";
    cout << "|                                        |\n";
    cout << "|   SELAMAT DATANG DI DEMEN BACA         |\n";
    cout << "|                                        |\n";
    cout << "==========================================\n";

    cout << " Silahkan login terlebih dahulu:\n";
    cout << " Masukkan NIK: ";
    cin >> nik;
    cout << " Masukkan password: ";
    cin >> password;

    int userRole;
    if (manajemenUser->verifikasiLogin(nik, password, userRole)) {
        role = userRole;
        
        cout << "\nLogin berhasil! Anda masuk sebagai ";
        switch(role) {
            case 1: cout << "ADMIN"; break;
            case 2: cout << "SUPER ADMIN"; break;
            case 3: cout << "PENGGUNA"; break;
            case 4: cout << "SATPAM"; break;
            case 5: cout << "KARYAWAN"; break;
            case 6: cout << "KASIR"; break;
            case 7: cout << "OB"; break;
            default: cout << "UNKNOWN"; break;
        }
        cout << ".\n";
    }
    else {
        cout << "\nNIK atau password salah!\n";
        role = 0;
    }
    cout << endl;
}

int Login::getRole() const {
    return role;
}
