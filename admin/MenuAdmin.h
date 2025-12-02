#ifndef MENUADMIN_H
#define MENUADMIN_H

class MenuAdmin {
public:
    void tampilkanMenu();


private:
    void kelolaTugasSatpam();
    void tambahTugasSatpam();
    void lihatTugasSatpam();
    void editTugasSatpam();
    void hapusTugasSatpam();
    int getNextTaskId();
    void kelolaTugasOb();
    void tambahTugasOb();
    void lihatTugasOb();
};

#endif
