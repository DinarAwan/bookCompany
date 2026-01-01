#ifndef MANAJEMENUSER_H
#define MANAJEMENUSER_H

#include <string>
#include "../KTP/ktp.h"

using namespace std;

class ManajemenUser {
private:
    struct UserNode {
        KTP data;
        UserNode* next;
    };

    // BST Node untuk sorting & searching
    struct BSTNode {
        KTP data;
        BSTNode* left;
        BSTNode* right;
    };

    UserNode* head;
    BSTNode* bstRoot;

    void loadDariFile();
    void simpanKeFile();

    // BST Helper functions
    BSTNode* insertBST(BSTNode* root, const KTP& user);
    BSTNode* searchBST(BSTNode* root, long long int nik);
    void inorderBST(BSTNode* root);
    void clearBST(BSTNode* root);

public:
    ManajemenUser();
    ~ManajemenUser();

    bool tambahUser(const KTP& user);
    KTP* cariByNIK(long long int nik);
    bool verifikasiLogin(long long int nik, const string& password, int& role);
    void displayAllUsers();
    bool isNIKExists(long long int nik);

    // BST Public methods
    void buildBSTFromList();
    void displayUsersSorted();
    KTP* searchUserBST(long long int nik);
};

#endif
