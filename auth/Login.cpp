#include "Login.h"
#include <iostream>
using namespace std;

Login::Login() {
    role = 0;
}

void Login::prosesLogin() {
    cout << endl;
    cout << "==========================================\n";
    cout << "|                                        |\n";
    cout << "|   SELAMAT DATANG DI DEMEN BACA         |\n";
    cout << "|                                        |\n";
    cout << "==========================================\n";

    cout << " Silahkan login terlebih dahulu:\n";
    cout << " Masukkan username: ";
    cin >> username;
    cout << " Masukkan password: ";
    cin >> password;

    if (username == "admin" && password == "1234") {
        role = 1;
        cout << "\nLogin berhasil! Anda masuk sebagai ADMIN.\n";
    } 
    else if (username == "super" && password == "1234") {
        role = 2;
        cout << "\nLogin berhasil! Anda masuk sebagai SUPER ADMIN.\n";
    } else if (username == "pengguna" && password == "1234") {
        role = 3;
        cout << "\nLogin berhasil! Anda masuk sebagai PENGGUNA.\n";
    } else if (username == "satpam" && password == "1234") {
        role = 4;
        cout << "\nLogin berhasil! Anda masuk sebagai SATPAM.\n";
    } else if (username == "karyawan" && password == "1234") {
        role = 5;
        cout << "\nLogin berhasil! Anda masuk sebagai KARYAWAN.\n";
    } else if (username == "kasir" && password == "1234") {
        role = 6;
        cout << "\nLogin berhasil! Anda masuk sebagai KASIR.\n";
    } else if (username == "ob" && password == "1234") {
        role = 7;
        cout << "\nLogin berhasil! Anda masuk sebagai OB.\n";
    }
    else {
        cout << "\nUsername atau password salah!\n";
        role = 0;
    }
    cout << endl;
}

int Login::getRole() const {
    return role;
}
