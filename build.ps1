# ========================================
# Build Script untuk BookCompany Project
# ========================================
# Script ini akan:
# 1. Compile semua file .cpp yang diperlukan
# 2. Jika berhasil, langsung menjalankan program
# 3. Menampilkan pesan error jika compile gagal

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Compiling BookCompany Project..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Command compile lengkap dengan semua file .cpp
g++ -o output/main.exe main.cpp auth/Login.cpp auth/ManajemenUser.cpp admin/MenuAdmin.cpp superAdmin/MenuSuperAdmin.cpp superAdmin/HuffmanFeatures.cpp huffman/HuffmanTree.cpp pengguna/menuPengguna.cpp satpam/MenuSatpam.cpp karyawan/menuKaryawan.cpp karyawan/laporanKaryawan.cpp kasir/menuKasir.cpp kasir/laporanKasir.cpp ob/menuOb.cpp

# Check jika compile berhasil
if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Compile BERHASIL!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  Menjalankan program..." -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    
    # Jalankan program
    .\output\main.exe
    
} else {
    Write-Host ""
    Write-Host "Compile GAGAL!" -ForegroundColor Red
    Write-Host "Periksa error di atas untuk detail." -ForegroundColor Yellow
    Write-Host ""
}
