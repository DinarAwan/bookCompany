# Dokumentasi Implementasi Struktur Data
## Project: BookCompany - Sistem Manajemen Perpustakaan

> Dokumen ini berisi detail implementasi berbagai struktur data yang digunakan dalam project BookCompany.

---

## 📋 Daftar Struktur Data yang Diimplementasikan

1. [AVL Tree](#1-avl-tree)
2. [Binary Search Tree (BST)](#2-binary-search-tree-bst)
3. [Stack](#3-stack)
4. [Queue](#4-queue)
5. [Priority Queue](#5-priority-queue)
6. [Linked List](#6-linked-list)
   - Single Linked List
   - Circular Linked List
7. [Dynamic Array](#7-dynamic-array)
8. [Huffman Tree](#8-huffman-tree)

---

## 1. AVL Tree

### Implementasi
**File:** [kasir/menuKasir.cpp](file:/TugasPro/kasir/menuKasir.cpp)  
**Baris:** 27-287

### Detail
- **Class:** `AVLTreeBuku`
- **Tujuan:** Menyimpan dan mengelola data buku dengan sorting otomatis dan akses cepat O(log n)
- **Fitur:**
  - Multiple sort modes: by ID, Harga, Stok, Total Terjual
  - Auto-balancing dengan rotation (left & right)
  - Range query untuk pencarian berdasarkan rentang harga
  - Top N query untuk buku terlaris

### Fungsi Utama
- `insert()` - Line 90-137: Insert dengan auto-balancing
- `rotateRight()` - Line 62-74: Rotasi kanan untuk balancing
- `rotateLeft()` - Line 76-88: Rotasi kiri untuk balancing
- `searchByID()` - Line 139-150: Pencarian O(log n)
- `rangeQuery()` - Line 167-186: Query berdasarkan range harga
- `countNodes()` - Line ~188: Helper untuk menghitung total nodes
- `collectNodes()` - Line ~195: Collect nodes ke dynamic array
- `displayTopN()` - Line 260-294: Menampilkan top N buku (menggunakan dynamic array)

---

## 2. Binary Search Tree (BST)

### Implementasi
**File:** [auth/ManajemenUser.cpp](file:/TugasPro/auth/ManajemenUser.cpp)  
**Baris:** 228-343

**Header:** [auth/ManajemenUser.h](file:/TugasPro/auth/ManajemenUser.h)  
**Baris:** 16-48

### Detail
- **Struct:** `BSTNode`
- **Tujuan:** Sorting dan searching user berdasarkan NIK
- **Fitur:**
  - Insert sorted by NIK
  - Inorder traversal untuk display terurut
  - Search user O(log n) average case

### Fungsi Utama
- `insertBST()` - Line 230-248: Insert node dengan sorting otomatis
- `searchBST()` - Line 250-261: Pencarian user by NIK
- `inorderBST()` - Line 263-292: Traversal inorder (sorted display)
- `buildBSTFromList()` - Line 303-317: Build BST dari linked list
- `clearBST()` - Line 294-301: Cleanup memory

### Penggunaan
- Menu SuperAdmin untuk sorting user by NIK
- Search user dengan kompleksitas O(log n)

---

## 3. Stack

### Implementasi
**File:** [kasir/menuKasir.cpp](file:/TugasPro/kasir/menuKasir.cpp)  
**Baris:** 305-575

### Detail
- **Struct:** `NodeRiwayat` (Line 306-309)
- **Pointer:** `topStack` (Line 311)
- **Tujuan:** Menyimpan histori aktivitas buku (LIFO - Last In First Out)
- **Implementasi:** Menggunakan Single Linked List

### Fungsi Utama
- `pushRiwayat()` - Line 528-535: Push data ke stack
- `popRiwayat()` - Line 537-553: Pop data dari stack
- `lihatHistori()` - Line 555-575: Display isi stack
- `simpanHistori()` - Line 403-426: Simpan stack ke file
- `muatHistori()` - Line 428-467: Load stack dari file

### Use Case
- Tracking aktivitas: DIPINJAM, DIKEMBALIKAN, DITAMBAHKAN, DIHAPUS, DIBELI
- Undo-like functionality untuk melihat histori terakhir

---

## 4. Queue

### Implementasi 1: Queue untuk Tugas OB (Admin)
**File:** [admin/MenuAdmin.cpp](file:/TugasPro/admin/MenuAdmin.cpp)  
**Baris:** 13-175

### Detail
- **Struct:** `NodeTugas` (Line 14-18)
- **Pointers:** `queueFront`, `queueRear` (Line 21-22)
- **Tujuan:** Antrian tugas untuk OB (FIFO - First In First Out)
- **Implementasi:** Menggunakan Linked List

### Fungsi Utama
- `enqueueLinkedList()` - Line 37-48: Tambah tugas ke rear queue
- `bersihkanQueue()` - Line 27-34: Clear queue memory
- Load & Save dari/ke file `tugas_ob.txt`

---

### Implementasi 2: Queue untuk Tugas OB (OB Menu)
**File:** [ob/menuOb.cpp](file:/TugasPro/ob/menuOb.cpp)  
**Baris:** 10-88, 446-497

### Detail
- **Tujuan:** OB dapat melihat dan menyelesaikan tugas (dequeue)
- **Fitur:**
  - Enqueue (Admin)
  - Dequeue (OB menyelesaikan tugas)
  - Display queue
  - Persistence ke file

### Fungsi Utama
- `enqueueLinkedList()` - Line 37-48
- `lihatAntrian()` - Line 446-467: Display queue FIFO
- `selesaikanTugas()` - Line 469-497: Dequeue tugas pertama

---

## 5. Priority Queue

### Implementasi
**File:** [huffman/HuffmanTree.cpp](file:/TugasPro/huffman/HuffmanTree.cpp) 
**Baris:** 88-119

**Header:** [huffman/HuffmanNode.h](file:/TugasPro/huffman/HuffmanNode.h)  
**Baris:** 19-25

### Detail
- **Struct:** `PriorityQueueNode` (HuffmanNode.h Line 20-25)
- **Tujuan:** Priority queue untuk building Huffman Tree
- **Sorting:** Berdasarkan frekuensi karakter (ascending)
- **Implementasi:** Manual menggunakan sorted linked list

### Fungsi Utama
- `insertPQ()` - Line 88-107: Insert dengan sorting by frequency
- `extractMin()` - Line 109-119: Extract node dengan frequency terkecil

### Use Case
- Huffman encoding algorithm
- Build Huffman tree dari frequency table

---

## 6. Linked List

### 6.1 Single Linked List

#### Implementasi 1: Manajemen Buku (Kasir)
**File:** [kasir/menuKasir.cpp](file:/TugasPro/kasir/menuKasir.cpp)  
**Baris:** 291-401

**Detail:**
- **Struct:** `NodeBuku` (Line 292-295)
- **Tujuan:** Primary storage untuk data buku
- **Fitur:** CRUD operations, load/save dari file

#### Implementasi 2: Manajemen Buku (Karyawan)
**File:** [karyawan/menuKaryawan.cpp](file:/TugasPro/karyawan/menuKaryawan.cpp)  
**Baris:** 220-477

**Detail:**
- **Struct:** `NodeBuku` (Line 222-225)
- **Fungsi:** Tambah, lihat, hapus buku

#### Implementasi 3: Manajemen User
**File:** [auth/ManajemenUser.cpp](file:/TugasPro/auth/ManajemenUser.cpp)  
**Baris:** 7-226

**Detail:**
- **Tujuan:** Menyimpan data user (KTP + role + password)
- **Fungsi:** CRUD, login verification, display all users

#### Implementasi 4: Frequency Table (Huffman)
**File:** [huffman/HuffmanTree.h](file:/TugasPro/huffman/HuffmanTree.h)  
**Baris:** 11-17

**Detail:**
- **Struct:** `FreqNode`
- **Tujuan:** Menyimpan frekuensi karakter untuk Huffman encoding

#### Implementasi 5: Huffman Codes Table
**File:** [huffman/HuffmanTree.h](file:/TugasPro/huffman/HuffmanTree.h)  
**Baris:** 20-26

**Detail:**
- **Struct:** `CodeNode`
- **Tujuan:** Menyimpan mapping karakter ke Huffman code

---

### 6.2 Circular Linked List

#### Implementasi 1: Absen Karyawan
**File:** [karyawan/menuKaryawan.cpp](file:/TugasPro/karyawan/menuKaryawan.cpp)  
**Baris:** 13-217

**Detail:**
- **Class:** `AbsenKaryawan`
- **Struct:** `AbsenK` (Line 15-22)
- **Circular Logic:** Line 72, 78 (`baru->next = head`)
- **Tujuan:** Menyimpan data absensi karyawan

**Fungsi:**
- `absenMasuk()` - Line 48-83
- `absenKeluar()` - Line 124-157
- `tampilAbsenSendiri()` - Line 98-122

#### Implementasi 2: KTP Module
**File:** [KTP/linkedlist.h](file:/TugasPro/KTP/linkedlist.h)  
**Baris:** 9-98

**Detail:**
- **Class:** `CircularLinkedListKTP`
- **Tujuan:** Demo circular linked list untuk data KTP

#### Implementasi 3: Absen OB
**File:** [ob/menuOb.cpp](file:/TugasPro/ob/menuOb.cpp)  
**Baris:** 90-265

**Detail:**
- **Circular Logic:** Line 131 (`head->next = head`)
- **Tujuan:** Tracking absensi OB

---

## 7. Dynamic Array

### Implementasi
**File:** [kasir/menuKasir.cpp](file:/TugasPro/kasir/menuKasir.cpp)  
**Baris:** ~260-294 (dalam fungsi `displayTopN()`)

### Detail
- **Tipe:** Manual dynamic array allocation menggunakan `new[]` dan `delete[]`
- **Tujuan:** Temporary storage untuk collecting dan sorting data buku dari AVL tree
- **Implementasi:** Pure manual memory management, **TIDAK menggunakan STL vector**

### Cara Kerja
```cpp
// 1. Count total nodes
int totalNodes = countNodes(root);

// 2. Allocate dynamic array
Buku* books = new Buku[totalNodes];

// 3. Collect data from tree
int index = 0;
collectNodes(root, books, index);

// 4. Process data (display top N)
for (int i = totalNodes - 1; i >= 0 && count < n; i--) {
    printBukuInfo(books[i]);
}

// 5. Free memory
delete[] books;
```

### Use Case
- **Top N Buku Terlaris**: Mengumpulkan semua buku dari AVL tree untuk sorting
- **Temporary Processing**: Data dikumpulkan, diproses, lalu di-deallocate

### Memory Management
- **Allocation**: `new Buku[size]` - Heap allocation
- **Deallocation**: `delete[] books` - Prevents memory leak
- **Safety**: Size calculated before allocation (no overflow)

### Keunggulan Implementasi Manual
✅ No external library dependencies (pure C++)  
✅ Full control over memory management  
✅ Educational value untuk understanding dynamic memory  
✅ Lightweight dan efficient  

---

## 8. Huffman Tree

### Implementasi
**File:** [huffman/HuffmanTree.cpp](file:/TugasPro/huffman/HuffmanTree.cpp)  
**Baris:** 1-457

**Header:** [huffman/HuffmanTree.h](file:/TugasPro/huffman/HuffmanTree.h)  
**Node:** [huffman/HuffmanNode.h](file:/TugasPro/huffman/HuffmanNode.h)

### Detail
- **Class:** `HuffmanTree`
- **Node Struct:** `HuffmanNode` (HuffmanNode.h Line 4-12)
- **Tujuan:** File compression menggunakan Huffman encoding
- **Algoritma:** Greedy algorithm dengan priority queue

### Komponen
1. **Frequency Table** - Linked list untuk frekuensi karakter
2. **Priority Queue** - Untuk building tree
3. **Binary Tree** - Huffman tree dengan leaf = karakter
4. **Codes Table** - Mapping karakter ke binary code

### Fungsi Utama
- `buildFrequencyTable()` - Line 75-86: Hitung frekuensi karakter
- `buildHuffmanTree()` - Line 122-175: Build tree dari frequency
- `generateCodes()` - Line 178-191: Generate Huffman codes (recursive)
- `compress()` - Line 206-219: Kompresi text ke binary
- `decompress()` - Line 222-253: Dekompresi binary ke text
- `visualizeTree()` - Line 335-351: Visualisasi tree structure
- `saveCompressed()` - Line 372-399: Save ke file
- `loadCompressed()` - Line 402-456: Load dari file

### Use Case
- SuperAdmin menu: Kompres/Dekompres file
- Text compression dengan ratio 40-60%

---

## 📊 Ringkasan Penggunaan

| Struktur Data | Lokasi Utama | Fungsi | Kompleksitas |
|---------------|--------------|--------|--------------|
| **AVL Tree** | Kasir Menu | Sort & search buku | Insert: O(log n), Search: O(log n) |
| **BST** | User Management | Sort user by NIK | Insert: O(log n), Search: O(log n) |
| **Stack** | Kasir Menu | Histori aktivitas | Push/Pop: O(1) |
| **Queue** | Admin/OB Menu | Antrian tugas | Enqueue/Dequeue: O(1) |
| **Priority Queue** | Huffman Algorithm | Build Huffman tree | Insert: O(n), ExtractMin: O(1) |
| **Single LL** | Kasir, Karyawan, Auth | Primary storage | Insert: O(1), Search: O(n) |
| **Circular LL** | Absensi | Absen karyawan/OB | Insert: O(1), Search: O(n) |
| **Dynamic Array** | AVL Tree (Top N) | Temporary storage | Allocation: O(n), Access: O(1) |
| **Huffman Tree** | SuperAdmin | File compression | Build: O(n log n), Encode/Decode: O(n) |

---

## 🎯 Kesimpulan

Project ini mengimplementasikan **9 struktur data berbeda** untuk berbagai keperluan:
- **Tree structures** (AVL, BST, Huffman) untuk operasi efisien
- **Linear structures** (Stack, Queue, Linked List) untuk manajemen data
- **Specialized structures** (Priority Queue, Circular LL, Dynamic Array) untuk use case spesifik

Setiap struktur data dipilih berdasarkan kebutuhan spesifik fitur dan kompleksitas operasi yang diinginkan.

> **✨ Catatan Penting:** Project ini **100% pure manual implementation** tanpa menggunakan STL `vector` atau container library lainnya. Semua struktur data diimplementasikan dari scratch menggunakan pointer, dynamic memory (`new`/`delete`), dan linked structures.

---

*Dokumen ini dibuat: 3 Januari 2026*  
*Terakhir diperbarui: 3 Januari 2026 (Added Dynamic Array, Removed STL vector dependency)*  
*Project: BookCompany - Struktur Data Semester 3*
