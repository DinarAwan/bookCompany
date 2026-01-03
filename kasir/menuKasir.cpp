#include "menuKasir.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "../Buku.h"

using namespace std;

// --- STRUKTUR UNTUK STACK (HISTORI) ---
struct Riwayat {
    string judulBuku;
    string aktivitas;
};

// ========== AVL TREE NODE ==========
struct AVLNode {
    Buku data;
    int height;
    AVLNode* left;
    AVLNode* right;
};

// ========== AVL TREE CLASS ==========
class AVLTreeBuku {
private:
    AVLNode* root;
    char sortMode; // 'I'=ID, 'H'=Harga, 'S'=Stok, 'T'=TotalTerjual
    
    // Helper: Get height of node
    int getHeight(AVLNode* node) {
        return (node == nullptr) ? 0 : node->height;
    }
    
    // Helper: Get balance factor
    int getBalance(AVLNode* node) {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }
    
    // Helper: Update height
    void updateHeight(AVLNode* node) {
        if (node != nullptr) {
            int leftHeight = getHeight(node->left);
            int rightHeight = getHeight(node->right);
            node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        }
    }
    
    // Helper: Compare two books based on sortMode
    bool isLessThan(const Buku& a, const Buku& b) {
        switch(sortMode) {
            case 'I': return a.id < b.id;
            case 'H': return a.harga < b.harga;
            case 'S': return a.stok < b.stok;
            case 'T': return a.totalTerjual < b.totalTerjual;
            default: return a.id < b.id;
        }
    }
    
    // Rotation: Right
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    // Rotation: Left
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    // Insert node with auto-balancing
    AVLNode* insert(AVLNode* node, const Buku& buku) {
        // Standard BST insert
        if (node == nullptr) {
            AVLNode* newNode = new AVLNode;
            newNode->data = buku;
            newNode->height = 1;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }
        
        if (isLessThan(buku, node->data)) {
            node->left = insert(node->left, buku);
        } else {
            node->right = insert(node->right, buku);
        }
        
        // Update height
        updateHeight(node);
        
        // Get balance factor
        int balance = getBalance(node);
        
        // Left-Left Case
        if (balance > 1 && isLessThan(buku, node->left->data)) {
            return rotateRight(node);
        }
        
        // Right-Right Case
        if (balance < -1 && !isLessThan(buku, node->right->data)) {
            return rotateLeft(node);
        }
        
        // Left-Right Case
        if (balance > 1 && !isLessThan(buku, node->left->data)) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right-Left Case
        if (balance < -1 && isLessThan(buku, node->right->data)) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    // Search by ID (only works when sortMode == 'I')
    AVLNode* searchByID(AVLNode* node, int id) {
        if (node == nullptr || node->data.id == id) {
            return node;
        }
        
        if (id < node->data.id) {
            return searchByID(node->left, id);
        }
        
        return searchByID(node->right, id);
    }
    
    // In-order traversal (sorted)
    void inorderTraversal(AVLNode* node, bool ascending) {
        if (node == nullptr) return;
        
        if (ascending) {
            inorderTraversal(node->left, ascending);
            printBukuInfo(node->data);
            inorderTraversal(node->right, ascending);
        } else {
            inorderTraversal(node->right, ascending);
            printBukuInfo(node->data);
            inorderTraversal(node->left, ascending);
        }
    }
    
    // Range query for harga (only works when sortMode == 'H')
    void rangeQuery(AVLNode* node, int minHarga, int maxHarga, int& count) {
        if (node == nullptr) return;
        
        // Search left subtree
        if (node->data.harga > minHarga) {
            rangeQuery(node->left, minHarga, maxHarga, count);
        }
        
        // If current node is in range
        if (node->data.harga >= minHarga && node->data.harga <= maxHarga) {
            printBukuInfo(node->data);
            count++;
        }
        
        // Search right subtree
        if (node->data.harga < maxHarga) {
            rangeQuery(node->right, minHarga, maxHarga, count);
        }
    }
    
    // Helper: Count total nodes
    int countNodes(AVLNode* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
    
    // Collect all nodes to dynamic array (for Top N)
    void collectNodes(AVLNode* node, Buku* books, int& index) {
        if (node == nullptr) return;
        collectNodes(node->left, books, index);
        books[index++] = node->data;
        collectNodes(node->right, books, index);
    }
    
    // Clear tree
    void clearTree(AVLNode* node) {
        if (node == nullptr) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
    
    // Print book info helper
    void printBukuInfo(const Buku& buku) {
        cout << "ID: " << buku.id << " | " << buku.judul << endl;
        cout << "  Penulis: " << buku.penulis << " (" << buku.tahun << ")" << endl;
        cout << "  Kategori: " << buku.kategori << " | Status: " << buku.status << endl;
        cout << "  Harga: Rp " << buku.harga << " | Stok: " << buku.stok << " unit" << endl;
        cout << "  Total Terjual: " << buku.totalTerjual << " unit" << endl;
        cout << "  ----------------------------------------" << endl;
    }
    
public:
    AVLTreeBuku(char mode = 'I') {
        root = nullptr;
        sortMode = mode;
    }
    
    ~AVLTreeBuku() {
        clearTree(root);
    }
    
    void insertBuku(const Buku& buku) {
        root = insert(root, buku);
    }
    
    Buku* searchBukuByID(int id) {
        if (sortMode != 'I') {
            return nullptr; // Search only works for ID-sorted tree
        }
        AVLNode* result = searchByID(root, id);
        return (result != nullptr) ? &(result->data) : nullptr;
    }
    
    void displaySorted(bool ascending = true) {
        if (root == nullptr) {
            cout << "Tidak ada data buku." << endl;
            return;
        }
        inorderTraversal(root, ascending);
    }
    
    void displayRangeHarga(int minHarga, int maxHarga) {
        if (root == nullptr) {
            cout << "Tidak ada data buku." << endl;
            return;
        }
        
        cout << "========== Buku dengan Harga Rp " << minHarga << " - Rp " << maxHarga << " ==========" << endl;
        int count = 0;
        rangeQuery(root, minHarga, maxHarga, count);
        if (count == 0) {
            cout << "Tidak ada buku dalam range harga tersebut." << endl;
        } else {
            cout << "Total: " << count << " buku ditemukan." << endl;
        }
    }
    
    void displayTopN(int n) {
        if (root == nullptr) {
            cout << "Tidak ada data buku." << endl;
            return;
        }
        
        // Count total nodes
        int totalNodes = countNodes(root);
        if (totalNodes == 0) {
            cout << "Belum ada data penjualan." << endl;
            return;
        }
        
        // Allocate dynamic array
        Buku* books = new Buku[totalNodes];
        int index = 0;
        collectNodes(root, books, index);
        
        // Books are already sorted in descending order for totalTerjual
        int count = 0;
        cout << "========== Top " << n << " Buku Terlaris ==========" << endl;
        for (int i = totalNodes - 1; i >= 0 && count < n; i--) {
            cout << (count + 1) << ". ";
            printBukuInfo(books[i]);
            count++;
        }
        
        // Free dynamic array
        delete[] books;
        
        if (count == 0) {
            cout << "Belum ada data penjualan." << endl;
        }
    }
    
    void clear() {
        clearTree(root);
        root = nullptr;
    }
};

class ManajemenBukuKasir {
private:
    // ========== SINGLE LINKED LIST UNTUK BUKU ==========
    struct NodeBuku {
        Buku data;
        NodeBuku* next;
    };

    NodeBuku* head;
    
    // ========== AVL TREES untuk berbagai sorting ==========
    AVLTreeBuku* avlByID;
    AVLTreeBuku* avlByHarga;
    AVLTreeBuku* avlByStok;
    AVLTreeBuku* avlByTerjual;

    // ========== SINGLE LINKED LIST UNTUK STACK HISTORI ==========
    struct NodeRiwayat {
        Riwayat data;
        NodeRiwayat* next;
    };

    NodeRiwayat* topStack;

    void muatDariFile() {
        ifstream file("buku.txt");
        if (!file.is_open()) return; 

        while (true) { 
            Buku buku;
            if (!(file >> buku.id)) break; 
            file.ignore(1000, '\n'); 
            
            if (!getline(file, buku.judul)) break;
            if (!getline(file, buku.penulis)) break;
            
            if (!(file >> buku.tahun)) break;
            file.ignore(1000, '\n'); 
            
            if (!getline(file, buku.isbn)) break;
            if (!getline(file, buku.kategori)) break;
            
            if (!getline(file, buku.status)) {
                 buku.status = "Tersedia"; 
            }
            
            if (buku.status.empty() || (buku.status != "Tersedia" && buku.status != "Dipinjam") ) {
                buku.status = "Tersedia";
            }
            
            // Baca harga dengan validasi
            if (!(file >> buku.harga)) {
                buku.harga = 50000;
                file.clear(); // Clear the fail state
            }
            if (buku.harga < 0) buku.harga = 50000;
            file.ignore(1000, '\n');
            
            // Baca stok dengan validasi
            if (!(file >> buku.stok)) {
                buku.stok = 10;
                file.clear(); // Clear the fail state
            }
            if (buku.stok < 0) buku.stok = 10;
            file.ignore(1000, '\n');
            
            // Baca totalTerjual dengan validasi
            if (!(file >> buku.totalTerjual)) {
                buku.totalTerjual = 0;
                file.clear(); // Clear the fail state
            }
            if (buku.totalTerjual < 0) buku.totalTerjual = 0;
            file.ignore(1000, '\n');

            // Tambahkan ke linked list
            NodeBuku* nodeBaru = new NodeBuku;
            nodeBaru->data = buku;
            nodeBaru->next = nullptr;

            if (head == nullptr) {
                head = nodeBaru;
            } else {
                NodeBuku* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = nodeBaru;
            }
        }
        file.close();
        
        // Build all AVL trees after loading
        rebuildAllAVL();
    }

    void simpanKeFile() {
        ofstream file("buku.txt", ios::trunc);
        NodeBuku* temp = head;
        while (temp != nullptr) {
            file << temp->data.id << "\n";
            file << temp->data.judul << "\n";
            file << temp->data.penulis << "\n";
            file << temp->data.tahun << "\n";
            file << temp->data.isbn << "\n";
            file << temp->data.kategori << "\n";
            file << temp->data.status << "\n";
            file << temp->data.harga << "\n";
            file << temp->data.stok << "\n";
            file << temp->data.totalTerjual << "\n";
            temp = temp->next;
        }
        file.close();
    }

    // --- FUNGSI MENYIMPAN HISTORI KE TXT ---
    void simpanHistori() {
        ofstream file("histori.txt", ios::trunc);
        if (!file.is_open()) return;

        // Hitung jumlah node di stack
        int count = 0;
        NodeRiwayat* temp = topStack;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }

        file << count << "\n";

        // Simpan data dari top ke bottom
        temp = topStack;
        while (temp != nullptr) {
            file << temp->data.judulBuku << "\n";
            file << temp->data.aktivitas << "\n";
            temp = temp->next;
        }
        file.close();
    }

    // --- FUNGSI MEMUAT HISTORI DARI TXT ---
    void muatHistori() {
        ifstream file("histori.txt");
        if (!file.is_open()) return;

        int count;
        if (!(file >> count)) return; 
        file.ignore(1000, '\n');

        if (count <= 0) {
            file.close();
            return;
        }

        // Baca semua data ke array temporary
        Riwayat* tempArray = new Riwayat[count];
        for (int i = 0; i < count; i++) {
            if (!getline(file, tempArray[i].judulBuku)) {
                delete[] tempArray;
                file.close();
                return;
            }
            if (!getline(file, tempArray[i].aktivitas)) {
                delete[] tempArray;
                file.close();
                return;
            }
        }
        file.close();

        // Push dari belakang ke depan agar urutan tetap sama
        for (int i = count - 1; i >= 0; i--) {
            NodeRiwayat* nodeBaru = new NodeRiwayat;
            nodeBaru->data = tempArray[i];
            nodeBaru->next = topStack;
            topStack = nodeBaru;
        }

        delete[] tempArray;
    }
    
    // Rebuild all AVL trees from linked list
    void rebuildAllAVL() {
        // Clear existing trees
        avlByID->clear();
        avlByHarga->clear();
        avlByStok->clear();
        avlByTerjual->clear();
        
        // Rebuild from linked list
        NodeBuku* temp = head;
        while (temp != nullptr) {
            avlByID->insertBuku(temp->data);
            avlByHarga->insertBuku(temp->data);
            avlByStok->insertBuku(temp->data);
            avlByTerjual->insertBuku(temp->data);
            temp = temp->next;
        }
    }

public:
    ManajemenBukuKasir() {
        head = nullptr;
        topStack = nullptr;
        
        // Initialize AVL trees
        avlByID = new AVLTreeBuku('I');
        avlByHarga = new AVLTreeBuku('H');
        avlByStok = new AVLTreeBuku('S');
        avlByTerjual = new AVLTreeBuku('T');
        
        muatDariFile(); 
        muatHistori();
    }

    ~ManajemenBukuKasir() {
        simpanKeFile(); 
        simpanHistori();
        
        // Hapus semua node buku
        while (head != nullptr) {
            NodeBuku* temp = head;
            head = head->next;
            delete temp;
        }

        // Hapus semua node stack
        while (topStack != nullptr) {
            NodeRiwayat* temp = topStack;
            topStack = topStack->next;
            delete temp;
        }
        
        // Hapus AVL trees
        delete avlByID;
        delete avlByHarga;
        delete avlByStok;
        delete avlByTerjual;
    }

    // --- FUNGSI STACK: PUSH ---
    void pushRiwayat(string judul, string aksi) {
        NodeRiwayat* nodeBaru = new NodeRiwayat;
        nodeBaru->data.judulBuku = judul;
        nodeBaru->data.aktivitas = aksi;
        nodeBaru->next = topStack;
        topStack = nodeBaru;
    }

    // --- FUNGSI STACK: POP ---
    void popRiwayat() {
        if (topStack == nullptr) {
            cout << "\n[INFO] Histori kosong! Tidak ada record yang bisa di-pop." << endl;
            return;
        }

        string judulDihapus = topStack->data.judulBuku;
        string aksiDihapus = topStack->data.aktivitas;

        NodeRiwayat* temp = topStack;
        topStack = topStack->next;
        delete temp;

        cout << "\n[SUKSES] Menghapus record terakhir dari Stack:" << endl;
        cout << "Buku: " << judulDihapus << " | Status: " << aksiDihapus << endl;
    }

    // --- FUNGSI STACK: DISPLAY ---
    void lihatHistori() {
        cout << "============= Histori Aktivitas (Stack LIFO) =============" << endl;
        if (topStack == nullptr) {
            cout << "Stack Kosong. Belum ada aktivitas." << endl;
            return;
        }

        NodeRiwayat* temp = topStack;
        int counter = 1;
        while (temp != nullptr) {
            cout << "No. " << counter << " (TOP";
            if (counter > 1) cout << "-" << (counter - 1);
            cout << ") | ";
            cout << "Buku: " << temp->data.judulBuku << " | ";
            cout << "Status: " << temp->data.aktivitas << endl;
            temp = temp->next;
            counter++;
        }
        cout << "==========================================================" << endl;
    }

    void lihatbuku() {
        if (head == nullptr) {
            cout << "Tidak ada data buku tersedia" << endl;
            return;
        }

        cout << "\n============= DAFTAR BUKU =============" << endl;
        NodeBuku* temp = head;
        int counter = 1;
        while (temp != nullptr) {
            cout << "\n" << counter << ". ------------------------------------" << endl;
            cout << "   ID               : " << temp->data.id << endl;
            cout << "   Judul            : " << temp->data.judul << endl;
            cout << "   Penulis          : " << temp->data.penulis << endl;
            cout << "   Tahun Terbit     : " << temp->data.tahun << endl;
            cout << "   ISBN             : " << temp->data.isbn << endl;
            cout << "   Kategori         : " << temp->data.kategori << endl;
            cout << "   Status           : " << temp->data.status << endl;
            cout << "   Harga            : Rp " << temp->data.harga << endl;
            cout << "   Stok             : " << temp->data.stok << " unit" << endl;
            cout << "   Total Terjual    : " << temp->data.totalTerjual << " unit" << endl;
            cout << "   ------------------------------------" << endl;
            temp = temp->next;
            counter++;
        }
        cout << "\n======================================" << endl;
    }

    void pinjamBuku() {
        int idPinjam;
        cout << "============= Kasir: Proses Peminjaman =============" << endl;
        cout << "Masukkan ID Buku yang akan dipinjam: ";
        if (!(cin >> idPinjam)) {
            cout << "Input tidak valid.\n"; 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            return;
        }
        cin.ignore(1000, '\n');

        NodeBuku* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == idPinjam) {
                if (temp->data.status == "Tersedia" && temp->data.stok > 0) {
                    temp->data.status = "Dipinjam";
                    temp->data.stok--;
                    temp->data.totalTerjual++;
                    simpanKeFile(); 
                    rebuildAllAVL();
                    pushRiwayat(temp->data.judul, "DIPINJAM"); 
                    cout << "\n[SUKSES] Buku berhasil dipinjam." << endl;
                    cout << "Stok tersisa: " << temp->data.stok << " unit" << endl;
                } else {
                    cout << "\n[GAGAL] Buku tidak tersedia atau stok habis." << endl;
                }
                return; 
            }
            temp = temp->next;
        }
        cout << "\n[ERROR] Buku tidak ditemukan." << endl;
    }

    void kembalikanBuku() {
        int idKembali;
        cout << "============= Kasir: Proses Pengembalian =============" << endl;
        cout << "Masukkan ID Buku yang dikembalikan: ";
        if (!(cin >> idKembali)) {
            cout << "Input tidak valid.\n"; 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            return;
        }
        cin.ignore(1000, '\n');

        NodeBuku* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == idKembali) {
                if (temp->data.status == "Dipinjam") {
                    temp->data.status = "Tersedia";
                    temp->data.stok++;
                    simpanKeFile();
                    rebuildAllAVL();
                    pushRiwayat(temp->data.judul, "DIKEMBALIKAN");
                    cout << "\n[SUKSES] Buku berhasil dikembalikan." << endl;
                    cout << "Stok sekarang: " << temp->data.stok << " unit" << endl;
                } else {
                    cout << "\n[INFO] Buku sudah berstatus tersedia." << endl;
                }
                return; 
            }
            temp = temp->next;
        }
        cout << "\n[ERROR] Buku tidak ditemukan." << endl;
    }
    
    // ========== FITUR BARU: BELI BUKU ==========
    void beliBuku() {
        int idBeli, jumlah;
        cout << "============= Transaksi Pembelian Buku =============" << endl;
        cout << "Masukkan ID Buku: ";
        if (!(cin >> idBeli)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        cin.ignore(1000, '\n');
        
        // Cari buku menggunakan AVL (O(log n))
        Buku* foundBuku = avlByID->searchBukuByID(idBeli);
        
        if (foundBuku == nullptr) {
            cout << "\n[ERROR] Buku tidak ditemukan." << endl;
            return;
        }
        
        cout << "\nBuku: " << foundBuku->judul << endl;
        cout << "Harga: Rp " << foundBuku->harga << endl;
        cout << "Stok tersedia: " << foundBuku->stok << " unit" << endl;
        
        cout << "\nMasukkan jumlah beli: ";
        if (!(cin >> jumlah) || jumlah <= 0) {
            cout << "Jumlah tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        cin.ignore(1000, '\n');
        
        // Update di linked list juga (find the matching node)
        NodeBuku* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == idBeli) {
                if (temp->data.stok < jumlah) {
                    cout << "\n[GAGAL] Stok tidak mencukupi! Stok tersedia: " << temp->data.stok << endl;
                    return;
                }
                
                // Process transaction
                temp->data.stok -= jumlah;
                temp->data.totalTerjual += jumlah;
                int totalHarga = temp->data.harga * jumlah;
                
                // Print receipt
                cout << "\n========== STRUK PEMBELIAN ==========" << endl;
                cout << "Buku: " << temp->data.judul << endl;
                cout << "Harga Satuan: Rp " << temp->data.harga << endl;
                cout << "Jumlah: " << jumlah << endl;
                cout << "Total: Rp " << totalHarga << endl;
                cout << "=====================================" << endl;
                cout << "Transaksi berhasil! Stok tersisa: " << temp->data.stok << " unit" << endl;
                
                // Save and rebuild
                simpanKeFile();
                rebuildAllAVL();
                pushRiwayat(temp->data.judul, "DIBELI (" + to_string(jumlah) + " unit)");
                
                return;
            }
            temp = temp->next;
        }
    }
    
    void tambahBuku() {
        Buku bukuBaru;
        // Inisialisasi eksplisit untuk ID dan tahun saja
        bukuBaru.id = 0;
        bukuBaru.tahun = 0;
        
        cout << "============= Tambah Buku Baru =============" << endl;
        
        cout << "Masukkan ID Buku: ";
        cin >> bukuBaru.id;
        while (cin.fail() || bukuBaru.id <= 0) {
            cout << "ID harus angka positif. Coba lagi: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> bukuBaru.id;
        }
        cin.ignore(1000, '\n');
        
        cout << "Masukkan Judul Buku: ";
        getline(cin, bukuBaru.judul);
        
        cout << "Masukkan Penulis Buku: ";
        getline(cin, bukuBaru.penulis);
        
        cout << "Masukkan Tahun Terbit: ";
        cin >> bukuBaru.tahun;
        while (cin.fail() || bukuBaru.tahun <= 0) {
            cout << "Tahun harus angka positif. Coba lagi: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> bukuBaru.tahun;
        }
        cin.ignore(1000, '\n');
        
        cout << "Masukkan ISBN: ";
        getline(cin, bukuBaru.isbn);
        
        cout << "Masukkan Kategori: ";
        getline(cin, bukuBaru.kategori);
        
        cout << "Masukkan Harga (Rp): ";
        cin >> bukuBaru.harga;
        while (cin.fail() || bukuBaru.harga <= 0) {
            cout << "Harga harus angka positif. Coba lagi: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> bukuBaru.harga;
        }
        cin.ignore(1000, '\n');
        
        cout << "Masukkan Stok Awal: ";
        cin >> bukuBaru.stok;
        while (cin.fail() || bukuBaru.stok <= 0) {
            cout << "Stok harus angka positif. Coba lagi: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> bukuBaru.stok;
        }
        cin.ignore(1000, '\n');
        
        bukuBaru.status = "Tersedia";
        bukuBaru.totalTerjual = 0;
        
        // Add to linked list
        NodeBuku* nodeBaru = new NodeBuku;
        nodeBaru->data = bukuBaru;
        nodeBaru->next = nullptr;
        
        if (head == nullptr) {
            head = nodeBaru;
        } else {
            NodeBuku* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = nodeBaru;
        }
        
        simpanKeFile();
        rebuildAllAVL();
        
        cout << "\n[SUKSES] Buku berhasil ditambahkan!" << endl;
        pushRiwayat(bukuBaru.judul, "DITAMBAHKAN");
    }
    
    // ========== FITUR BARU: UPDATE STOK ==========
    void updateStok() {
        int idBuku, tambahStok;
        cout << "============= Update Stok Buku =============" << endl;
        cout << "Masukkan ID Buku: ";
        if (!(cin >> idBuku)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        cin.ignore(1000, '\n');
        
        NodeBuku* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == idBuku) {
                cout << "Buku: " << temp->data.judul << endl;
                cout << "Stok saat ini: " << temp->data.stok << " unit" << endl;
                
                cout << "Masukkan jumlah tambahan stok: ";
                if (!(cin >> tambahStok) || tambahStok < 0) {
                    cout << "Jumlah tidak valid.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    return;
                }
                cin.ignore(1000, '\n');
                
                temp->data.stok += tambahStok;
                cout << "\n[SUKSES] Stok berhasil diupdate!" << endl;
                cout << "Stok baru: " << temp->data.stok << " unit" << endl;
                
                simpanKeFile();
                rebuildAllAVL();
                pushRiwayat(temp->data.judul, "RESTOCK (+" + to_string(tambahStok) + " unit)");
                
                return;
            }
            temp = temp->next;
        }
        
        cout << "\n[ERROR] Buku tidak ditemukan." << endl;
    }
    
    // ========== FITUR BARU: HAPUS BUKU ==========
    void hapusBuku() {
        int idHapus;
        cout << "============= Hapus Buku =============" << endl;
        cout << "Masukkan ID Buku yang akan dihapus: ";
        while (!(cin >> idHapus)) {
            cout << "ID harus angka. Coba lagi: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');
        
        if (head == nullptr) {
            cout << "Buku dengan ID " << idHapus << " tidak ditemukan" << endl;
            return;
        }
        
        // If first node
        if (head->data.id == idHapus) {
            string judulDihapus = head->data.judul;
            NodeBuku* temp = head;
            head = head->next;
            delete temp;
            simpanKeFile();
            rebuildAllAVL();
            pushRiwayat(judulDihapus, "DIHAPUS");
            cout << "Buku dengan ID " << idHapus << " berhasil dihapus." << endl;
            return;
        }
        
        // Find node to delete
        NodeBuku* temp = head;
        while (temp->next != nullptr && temp->next->data.id != idHapus) {
            temp = temp->next;
        }
        
        if (temp->next == nullptr) {
            cout << "Buku dengan ID " << idHapus << " tidak ditemukan" << endl;
            return;
        }
        
        string judulDihapus = temp->next->data.judul;
        NodeBuku* nodeHapus = temp->next;
        temp->next = nodeHapus->next;
        delete nodeHapus;
        simpanKeFile();
        rebuildAllAVL();
        pushRiwayat(judulDihapus, "DIHAPUS");
        cout << "Buku dengan ID " << idHapus << " berhasil dihapus." << endl;
    }
    
    // ========== MENU AVL FEATURES ==========
    void menuAVL() {
        int pilihan;
        do {
            cout << "\n========== MANAJEMEN BUKU (AVL TREE) ==========" << endl;
            cout << "1. Search Buku by ID (O(log n))" << endl;
            cout << "2. Urutkan by Harga (Murah -> Mahal)" << endl;
            cout << "3. Urutkan by Harga (Mahal -> Murah)" << endl;
            cout << "4. Urutkan by Stok (Sedikit -> Banyak)" << endl;
            cout << "5. Urutkan by Stok (Banyak -> Sedikit)" << endl;
            cout << "6. Cari Buku by Range Harga" << endl;
            cout << "7. Top N Buku Terlaris" << endl;
            cout << "0. Kembali" << endl;
            cout << "Pilih menu: ";
            
            while (!(cin >> pilihan)) {
                cout << "Input tidak valid. Masukkan angka: ";
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Pilih menu: ";
            }
            cin.ignore(1000, '\n');
            
            switch(pilihan) {
                case 1: { // Search by ID
                    int id;
                    cout << "Masukkan ID Buku: ";
                    if (cin >> id) {
                        cin.ignore(1000, '\n');
                        Buku* found = avlByID->searchBukuByID(id);
                        if (found != nullptr) {
                            cout << "\n========== Buku Ditemukan! ==========" << endl;
                            cout << "ID: " << found->id << endl;
                            cout << "Judul: " << found->judul << endl;
                            cout << "Penulis: " << found->penulis << " (" << found->tahun << ")" << endl;
                            cout << "Kategori: " << found->kategori << endl;
                            cout << "Status: " << found->status << endl;
                            cout << "Harga: Rp " << found->harga << endl;
                            cout << "Stok: " << found->stok << " unit" << endl;
                            cout << "Total Terjual: " << found->totalTerjual << " unit" << endl;
                        } else {
                            cout << "Buku tidak ditemukan." << endl;
                        }
                    } else {
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                    break;
                }
                case 2: // Sort by harga ascending
                    cout << "\n========== Buku Diurutkan by Harga (Murah -> Mahal) ==========" << endl;
                    avlByHarga->displaySorted(true);
                    break;
                case 3: // Sort by harga descending
                    cout << "\n========== Buku Diurutkan by Harga (Mahal -> Murah) ==========" << endl;
                    avlByHarga->displaySorted(false);
                    break;
                case 4: // Sort by stok ascending
                    cout << "\n========== Buku Diurutkan by Stok (Sedikit -> Banyak) ==========" << endl;
                    avlByStok->displaySorted(true);
                    break;
                case 5: // Sort by stok descending
                    cout << "\n========== Buku Diurutkan by Stok (Banyak -> Sedikit) ==========" << endl;
                    avlByStok->displaySorted(false);
                    break;
                case 6: { // Range query harga
                    int minHarga, maxHarga;
                    cout << "Masukkan harga minimum (Rp): ";
                    if (!(cin >> minHarga)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        break;
                    }
                    cout << "Masukkan harga maksimum (Rp): ";
                    if (!(cin >> maxHarga)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        break;
                    }
                    cin.ignore(1000, '\n');
                    avlByHarga->displayRangeHarga(minHarga, maxHarga);
                    break;
                }
                case 7: { // Top N terlaris
                    int n;
                    cout << "Masukkan N (Top N Buku Terlaris): ";
                    if (!(cin >> n) || n <= 0) {
                        cout << "Input tidak valid." << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                        break;
                    }
                    cin.ignore(1000, '\n');
                    avlByTerjual->displayTopN(n);
                    break;
                }
                case 0:
                    cout << "Kembali ke menu utama..." << endl;
                    break;
                default:
                    cout << "Pilihan tidak valid!" << endl;
            }
            
            if (pilihan != 0) {
                cout << "\nTekan Enter untuk melanjutkan...";
                cin.get();
            }
            
        } while (pilihan != 0);
    }
};

void MenuKasir::tampilkanMenu() {
    int pilihan;
    ManajemenBukuKasir manajemenKasir; 

    do {
        cout << "========== MENU KASIR ==========\n";
        cout << "1. Lihat Daftar Buku\n"; 
        cout << "2. Pinjam Buku\n"; 
        cout << "3. Kembalikan Buku\n";
        cout << "4. Beli Buku (Transaksi Penjualan)\n";
        cout << "5. Tambah Buku Baru\n";
        cout << "6. Update Stok Buku\n";
        cout << "7. Hapus Buku\n";
        cout << "8. Manajemen Buku (AVL Tree)\n";
        cout << "9. Lihat Histori (STACK)\n"; 
        cout << "10. Hapus Histori Terakhir (POP)\n"; 
        cout << "11. Buat Laporan Harian\n";  // NEW: Report feature
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: "; 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            cout << "Pilih menu: "; 
        }
        cin.ignore(1000, '\n');

        switch (pilihan) {
            case 1:
                manajemenKasir.lihatbuku();
                break;
            case 2:
                manajemenKasir.pinjamBuku();
                break;
            case 3:
                manajemenKasir.kembalikanBuku();
                break;
            case 4:
                manajemenKasir.beliBuku();
                break;
            case 5:
                manajemenKasir.tambahBuku();
                break;
            case 6:
                manajemenKasir.updateStok();
                break;
            case 7:
                manajemenKasir.hapusBuku();
                break;
            case 8:
                manajemenKasir.menuAVL();
                break;
            case 9: 
                manajemenKasir.lihatHistori();
                break;
            case 10:
                manajemenKasir.popRiwayat();
                break;
            case 11:
                buatLaporanHarian("", "");  // User will input manually
                break;
            case 0:
                cout << "Logout berhasil! (Data tersimpan)\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}