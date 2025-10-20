#include <iostream>
#include <limits> // <-- TAMBAHKAN INI
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

    Login login;
    login.prosesLogin();

    int role = login.getRole();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
        cout << "Program dihentikan.\n";
    }

    return 0;
}