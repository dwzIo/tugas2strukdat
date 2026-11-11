#include <iostream>
using namespace std;

struct NodeQ {
    string nama_pel;
    NodeQ* next;
};

struct Queue {
    NodeQ* front;
    NodeQ* rear;
};

bool isEmptyQ(Queue& q) {
    return q.front == nullptr;
}

void enqueue(Queue& q, string nama_pel) {
    NodeQ* baru = new NodeQ{nama_pel, nullptr};
    if (isEmptyQ(q)) {
        q.front = q.rear = baru;
    }
    else {
        q.rear->next = baru;
        q.rear = baru;
    }
}

void enqueueFront(Queue& q, string nama_pel) {
    NodeQ* baru = new NodeQ{nama_pel, q.front};
    q.front = baru;
    if (q.rear == nullptr) q.rear = baru;
}

string dequeue(Queue& q) {
    if (isEmptyQ(q)) return "";
    NodeQ* hapus = q.front;
    string nama = hapus->nama_pel;
    q.front = q.front->next;
    if (q.front == nullptr) q.rear = nullptr;
    delete hapus;
    return nama;
}

void hapusbelakang(Queue& q) {
    if (isEmptyQ(q)) return;
    if (q.front == q.rear) {
        delete q.front;
        q.front = q.rear = nullptr;
        return;
    }
    NodeQ* temp = q.front;
    while (temp->next != q.rear) temp = temp->next;
    delete q.rear;
    q.rear = temp;
    q.rear->next = nullptr;
}

void clearQueue(Queue& q) {
    while (!isEmptyQ(q)) dequeue(q);
}

struct Menu {
    int harga;
    string nama_menu;
    string kategori;
    bool tersedia;
    Menu* left;
    Menu* right;
    Queue antrian;
};

bool insertMenu(Menu*& root, int harga, string nama, string kategori) {
    if (root == nullptr) {
        root = new Menu{harga, nama, kategori, true, nullptr, nullptr, {nullptr, nullptr}};
        return true;
    }
    else if (nama < root->nama_menu) {
        return insertMenu(root->left, harga, nama, kategori);
    }
    else if (nama > root->nama_menu) {
        return insertMenu(root->right, harga, nama, kategori);
    }
    return false;
}

Menu* cariMenu(Menu* root, string nama) {
    if (root == nullptr) return nullptr;
    if (nama == root->nama_menu) return root;
    if (nama < root->nama_menu) return cariMenu(root->left, nama);
    return cariMenu(root->right, nama);
}

void inorder(Menu* root) {
    if (root == nullptr) return;

    inorder(root->left);
    cout << "Nama Menu  : " << root->nama_menu << '\n';
    cout << "Kategori   : " << root->kategori << '\n';
    cout << "Harga      : " << root->harga << '\n';
    cout << "Status     : " << (root->tersedia ? "Tersedia" : "Habis") << "\n\n";
    inorder(root->right);
}

bool hapusmenu(Menu*& root, string nama) {
    if (root == nullptr) return false;

    if (nama < root->nama_menu) return hapusmenu(root->left, nama);
    if (nama > root->nama_menu) return hapusmenu(root->right, nama);

    if (root->left == nullptr) {
        Menu* hapus = root;
        root = root->right;
        clearQueue(hapus->antrian);
        delete hapus;
    }
    else if (root->right == nullptr) {
        Menu* hapus = root;
        root = root->left;
        clearQueue(hapus->antrian);
        delete hapus;
    }
    else {
        Menu* successor = root->right;
        while (successor->left != nullptr) successor = successor->left;

        clearQueue(root->antrian);
        root->nama_menu = successor->nama_menu;
        root->kategori = successor->kategori;
        root->harga = successor->harga;
        root->tersedia = successor->tersedia;
        root->antrian = successor->antrian;     
        successor->antrian = {nullptr, nullptr};   

        hapusmenu(root->right, root->nama_menu);
    }
    return true;
}

void hapussemuamenu(Menu*& root) {
    if (root == nullptr) return;
    hapussemuamenu(root->left);
    hapussemuamenu(root->right);
    clearQueue(root->antrian);
    delete root;
    root = nullptr;
}

const int pes_pel_PESAN = 1;
const int pes_pel_UPDATE = 2;

struct pes_pel {
    int jenis;           
    string nama_menu;
    string nama_pel;     
    bool menuggu;   
    pes_pel* next;
};

struct LinkedStack {
    pes_pel* top;
};

void buatriwayat(LinkedStack& s) {
    s.top = nullptr;
}

bool isEmpty(LinkedStack& s) {
    return s.top == nullptr;
}

void push(LinkedStack& s, int jenis, string nama_menu, string nama_pel, bool menuggu) {
    pes_pel* baru = new pes_pel{jenis, nama_menu, nama_pel, menuggu, s.top};
    s.top = baru;
}

bool pop(LinkedStack& s, pes_pel& hasil) {
    if (isEmpty(s)) return false;
    pes_pel* hapus = s.top;
    hasil = *hapus;
    s.top = s.top->next;
    delete hapus;
    return true;
}

void clearStack(LinkedStack& s) {
    while (s.top != nullptr) {
        pes_pel* hapus = s.top;
        s.top = s.top->next;
        delete hapus;
    }
}

void tambahmenu(Menu*& root) {
    string nama;
    string kategori;
    int harga;

    cout << "Nama menu : ";
    getline(cin, nama);
    cout << "Kategori  : ";
    getline(cin, kategori);
    cout << "Harga     : ";
    cin >> harga;
    cin.ignore();

    if (insertMenu(root, harga, nama, kategori)) cout << "Menu berhasil ditambahkan\n";

    else cout << "Menu sudah ada\n";
}

void lihatmenu(Menu* root) {
    if (root == nullptr) {
        cout << "Daftar menu kosong\n";
        return;
    }
    inorder(root);
}

void pesanmenu(Menu* root, LinkedStack& s) {
    string pelanggan, nama;
    cout << "Nama pelanggan : ";
    getline(cin, pelanggan);
    cout << "Nama menu      : ";
    getline(cin, nama);

    Menu* m = cariMenu(root, nama);
    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (m->tersedia) {
        cout << "Pesanan " << pelanggan << " berhasil dibuat\n";
        push(s, pes_pel_PESAN, nama, pelanggan, false);
    }
    else {
        cout << "Menu sedang habis, " << pelanggan << " masuk ke dalam waiting list\n";
        enqueue(m->antrian, pelanggan);
        push(s, pes_pel_PESAN, nama, pelanggan, true);
    }
}

void menuyangtersedia(Menu* root, LinkedStack& s) {
    string nama;
    cout << "Nama menu : ";
    getline(cin, nama);

    Menu* m = cariMenu(root, nama);
    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }
    if (m->tersedia) {
        cout << "Menu sudah tersedia\n";
        return;
    }

    m->tersedia = true;
    cout << "Menu tersedia kembali\n";

    string dilayani = "";
    if (!isEmptyQ(m->antrian)) {
        dilayani = dequeue(m->antrian);
        cout << "Pesanan langsung diberikan kepada " << dilayani << " dari waiting list\n";
    }
    push(s, pes_pel_UPDATE, nama, dilayani, false);
}

void tandaimenuhabis(Menu* root) {
    string nama;
    cout << "Nama menu : ";
    getline(cin, nama);

    Menu* m = cariMenu(root, nama);
    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }
    m->tersedia = false;
    cout << "Menu " << nama << " sekarang habis\n";
}

void hapusmenuinput(Menu*& root) {
    string nama;
    cout << "Nama menu yang dihapus : ";
    getline(cin, nama);

    if (hapusmenu(root, nama))
        cout << "Menu berhasil dihapus\n";
    else
        cout << "Menu tidak ditemukan\n";
}

void undo(Menu* root, LinkedStack& s) {
    pes_pel a;
    if (!pop(s, a)) {
        cout << "Tidak ada pelanggan untuk di-undo\n";
        return;
    }

    Menu* m = cariMenu(root, a.nama_menu);

    if (a.jenis == pes_pel_PESAN) {
        if (m != nullptr && a.menuggu) hapusbelakang(m->antrian);
        cout << "Undo: Pesanan menu " << a.nama_menu << " dibatalkan\n";
    }
    else {
        if (m != nullptr) {
            m->tersedia = false;
            if (a.nama_pel != "") enqueueFront(m->antrian, a.nama_pel);
        }
        cout << "Undo: Menu " << a.nama_menu << " kembali habis\n";
    }
}

void tampilantrian(Menu* root) {
    string nama;
    cout << "Nama menu : ";
    getline(cin, nama);

    Menu* m = cariMenu(root, nama);
    if (m == nullptr || isEmptyQ(m->antrian)) {
        cout << "Antrian kosong\n";
        return;
    }

    NodeQ* temp = m->antrian.front;
    while (temp != nullptr) {
        cout << temp->nama_pel << " menunggu menu " << nama << '\n';
        temp = temp->next;
    }
}

int main() {
    Menu* root = nullptr;
    LinkedStack riwayat;
    buatriwayat(riwayat);

    int pilih;
    do {
        cout << "\n===== Menu Restoran =====\n";
        cout << "1. Tambah menu\n";
        cout << "2. Lihat daftar menu\n";
        cout << "3. Catat pesanan pelanggan\n";
        cout << "4. Tandai menu\n";
        cout << "5. Hapus menu\n";
        cout << "6. Undo pelanggan terakhir\n";
        cout << "7. Tampilkan antrian pelanggan\n";
        cout << "0. Keluar\n";
        cout << "Pilih : ";
        cin >> pilih;
        system("cls");
        cin.ignore();

        switch (pilih) {
            case 1: tambahmenu(root);
            system("pause"); system("cls");
            break;

            case 2: lihatmenu(root);
            system("pause"); system("cls");
            break;

            case 3: pesanmenu(root, riwayat);
            system("pause"); system("cls");
            break;

            case 4: {
                string opsi;
                cout << "Tandai tersedia/habis? "; cin >> opsi;
                cin.ignore();
                (opsi == "tersedia" ? menuyangtersedia(root, riwayat) : tandaimenuhabis(root));
                system("pause"); system("cls");
            } 
            break;

            case 5: hapusmenuinput(root);
            system("pause"); system("cls");
            break;

            case 6: undo(root, riwayat);
            system("pause"); system("cls");
            break;

            case 7: tampilantrian(root);
            system("pause"); system("cls");
            break;

            case 0: cout << "Terima kasih!\n";
        }
    } while (pilih != 0);

    clearStack(riwayat);
    hapussemuamenu(root);
    return 0;
}