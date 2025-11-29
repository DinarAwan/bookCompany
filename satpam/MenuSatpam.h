#ifndef MENUSATPAM_H
#define MENUSATPAM_H

#include <string>

/**
 * @brief Deklarasi Kelas untuk Menu Satpam
 * Kelas ini menangani semua fungsionalitas yang tersedia untuk Satpam.
 */
class MenuSatpam {

public:
    /**
     * @brief Menampilkan menu utama dan memproses input pengguna.
     */
    void tampilkanMenu();

private:
    // --- Kelompok 1: Tugas & Kehadiran ---
    void lihatTugas();
    void absenKehadiran();
    void selesaikanTugas();
    void lihatLaporanSelesai();
    void lihatLogAbsen();

    // --- Kelompok 2: Laporan & Keamanan ---
    void laporKejadian();
    void lihatLaporanKejadian();
    void logPatroli();
    void laporanHarian();
    void modeDarurat();
    void cekStatusSistem();
    void lihatNotifikasi();

    // --- Kelompok 3: Log Barang & Kendaraan ---
    void logKendaraan();
    void lihatLogKendaraan();
    void catatBarang();
    void lihatBarang();

    // --- Kelompok 4: CRUD Pengunjung ---
    void catatPengunjung();
    void tampilkanPengunjung();
    void ubahPengunjung();
    void hapusPengunjung();

    // --- Kelompok 5: Pengaturan ---
    void pengaturanProfil();
};

#endif // MENUSATPAM_H