# Script untuk compile program dengan benar
# Jalankan script ini untuk memastikan semua file ter-compile dengan update terbaru

# Compile program
g++ -o output/main.exe main.cpp auth/Login.cpp auth/ManajemenUser.cpp admin/MenuAdmin.cpp superAdmin/MenuSuperAdmin.cpp pengguna/menuPengguna.cpp satpam/MenuSatpam.cpp karyawan/menuKaryawan.cpp kasir/menuKasir.cpp ob/menuOb.cpp

# Jalankan program jika compile berhasil
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compile berhasil! Menjalankan program..." -ForegroundColor Green
    .\output\main.exe
} else {
    Write-Host "Compile gagal!" -ForegroundColor Red
}
