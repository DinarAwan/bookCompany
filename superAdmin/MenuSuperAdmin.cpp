#include "MenuSuperAdmin.h"
#include <iostream>
using namespace std;

void MenuSuperAdmin::tampilkanMenu() {
    int pilihan;
    do {
        cout << "====== MENU SUPER ADMIN ======\n";
        cout << "1. Kelola User\n";
        cout << "2. Backup Data\n";
        cout << "3. Lihat Log Aktivitas\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                cout << "Mengelola user...\n";
                break;
            case 2:
                cout << "Melakukan backup data...\n";
                break;
            case 3:
                cout << "Menampilkan log aktivitas...\n";
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
