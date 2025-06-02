#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cstdio>

const int TEXTURE_SIZE = 64; // Ukuran tekstur 64x64
GLuint jalanTexture;

// Definisi warna
GLfloat ungu[] = {0.42, 0.24, 0.60, 1.0};         // Ungu Lavender Gelap
GLfloat biruMetalik[] = {0.12, 0.33, 0.70, 1.0}; // Biru Baja
GLfloat putih[] = {0.96, 0.96, 0.96, 1.0};       // Putih Gading
GLfloat hitam[] = {0.05, 0.05, 0.05, 1.0};       // Hitam Arang
GLfloat abu[] = {0.6, 0.6, 0.6, 1.0};          // Abu-abu Medium
GLfloat silver[] = {0.78, 0.78, 0.78, 1.0};      // Silver Halus
GLfloat merah[] = {0.8, 0.15, 0.15, 1.0};        // Merah Marun
GLfloat biruLangit[] = {0.45, 0.74, 0.90, 1.0};  // Biru Langit Lembut
GLfloat rumputSiang[] = {0.4f, 0.8f, 0.4f, 1.0f}; // Lebih cerah
GLfloat rumputMalam[] = {0.1f, 0.3f, 0.1f, 1.0f};  // Lebih gelap
GLfloat coklatKayu[] = {0.45, 0.26, 0.13, 1.0};  // Coklat Kayu Tua
GLfloat hijauDaun[] = {0.25, 0.5, 0.2, 1.0};    // Hijau Daun Natural
GLfloat coklatTanah[] = {0.58, 0.44, 0.33, 1.0}; // Coklat Tanah Lembut
GLfloat abuBatu[] = {0.65, 0.65, 0.65, 1.0};      // Abu Batu Tua
GLfloat merahBata[] = {0.75, 0.3, 0.2, 1.0};      // Merah Bata Natural
GLfloat coklatAtap[] = {0.5, 0.25, 0.15, 1.0};  // Coklat Atap Gelap
GLfloat kuning[] = {0.95, 0.85, 0.3, 1.0};      // Kuning Emas Lembut

GLfloat darkSky[]       = {  0.02f, 0.02f, 0.1f,    1.0f };  // langit malam gelap
GLfloat moonColor[]     = {  0.9f,  0.9f,  0.8f,    1.0f };  // warna bulan
GLfloat starColor[]     = {  1.0f,  1.0f,  1.0f,    1.0f };  // warna bintang

GLfloat ambient[] = { 0.1f, 0.1f, 0.15f, 1.0f }; 
GLfloat diffuse[] = { 0.3f, 0.3f, 0.35f, 1.0f };

// Variabel untuk game
float carX = 0.0f;
float carZ = 0.0f;
float carSpeed = 0.0f;
float finishZ = 100.0f;
float maxSpeed = 0.2f;
float acceleration = 0.01f;
float steeringSpeed = 0.1f;
bool gameStarted = false;
bool gameOver = false;
float roadWidth = 5.0f;
float roadLength = 200.0f;
float wheelRotation = 0.0f;

// Variabel untuk kamera
float cameraHeight = 2.0f;
float cameraDistance = 5.0f;
float cameraAngle = 0.0f;
bool thirdPersonView = true;
bool topView = false;
bool sideTopView = false;

//Toogle tema
bool isNight = true; // Default malam, bisa diganti jadi false untuk default siang

// Struktur untuk rintangan
struct Obstacle {
    float x, z;
    int type;
};

// Struktur untuk objek dekoratif samping jalan
struct Decoration {
    float x, z;
    int type; // 0 = rumah tipe 1, 1 = rumah tipe 2, 2 = pohon besar, 3 = pohon kecil
    float rotasi; // Untuk orientasi acak rumah
};

std::vector<Obstacle> obstacles;
std::vector<Decoration> decorations;

// Tekstur aspal
GLuint asphaltTexture;

// Pembuatan tekstur aspal secara prosedural
#define TEXTURE_SIZE 256
GLubyte asphaltData[TEXTURE_SIZE][TEXTURE_SIZE][3];

void generateAsphaltTexture() {
    srand(time(NULL)); // Inisialisasi seed untuk generator angka acak

    // Membuat tekstur aspal kasar secara acak
    for (int i = 0; i < TEXTURE_SIZE; i++) {
        for (int j = 0; j < TEXTURE_SIZE; j++) {
            // Warna abu-abu gelap sebagai dasar aspal
            int baseGray = 50 + rand() % 30; // Rentang 50–79 (warna dasar gelap)

            // Meningkatkan kontras dan variasi
            if (rand() % 4 == 0) { // Perubahan lebih sering
                baseGray += 40 + rand() % 40; // Butiran lebih terang, rentang lebih lebar
            }
            if (rand() % 6 == 0) { // Retakan lebih sering
                baseGray -= 30 + rand() % 20; // Retakan lebih gelap, rentang lebih lebar
            }

            // Menambahkan noise yang lebih mencolok
             baseGray += (rand() % 11) - 5; // Menambahkan nilai antara -5 dan 5

            // Memastikan nilai tetap dalam rentang yang valid
            baseGray = baseGray > 255 ? 255 : baseGray;
            baseGray = baseGray < 0 ? 0 : baseGray;

            asphaltData[i][j][0] = baseGray;
            asphaltData[i][j][1] = baseGray;
            asphaltData[i][j][2] = baseGray;
        }
    }

    // Membuat dan menetapkan tekstur
    glGenTextures(1, &asphaltTexture);
    glBindTexture(GL_TEXTURE_2D, asphaltTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, asphaltData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Fungsi untuk membuat roda
void buatRoda(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Mengorientasikan roda dengan benar - torus secara default berada di bidang XZ
    // Untuk mobil yang bergerak sepanjang sumbu Z, roda harus berputar pada sumbu X
    glRotatef(90.0, 0.0, 1.0, 0.0);  // Rotasi pertama untuk orientasi yang benar

    // Kemudian terapkan rotasi roda pada sumbu yang sesuai (sumbu Y roda, yang sekarang menjadi Z di world space)
    glRotatef(wheelRotation, 0.0, 0.0, 1.0);

    // Ban (silinder hitam)
    glColor3fv(hitam);
    glutSolidTorus(0.08, 0.18, 20, 20);

    // Pelek (silinder perak)
    glColor3fv(abu);
    glutSolidTorus(0.04, 0.13, 10, 10);

    glPopMatrix();
}

// Fungsi untuk membuat mobil
void buatMobil() {
    // Bodi utama mobil (kotak)
    glPushMatrix();
    glColor3fv(ungu);
    glScalef(1.0, 0.5, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Bagian kabin mobil (kotak) - Struktur ungu
    glPushMatrix();
    glColor3fv(ungu);
    glTranslatef(0.0, 0.4, -0.2);
    glScalef(0.8, 0.4, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Kaca depan (trapesium)
    glPushMatrix();
    glColor3fv(putih);
    glTranslatef(0.0, 0.35, 0.3);
    glRotatef(-45.0, 1.0, 0.0, 0.0);
    glScalef(0.75, 0.25, 0.45);
    glutSolidCube(1.0);
    glPopMatrix();

    // Kaca samping kiri - Tambahan
    glPushMatrix();
    glColor3fv(putih);
    glTranslatef(0.4, 0.4, -0.2);
    glScalef(0.05, 0.35, 0.9);
    glutSolidCube(1.0);
    glPopMatrix();

    // Kaca samping kanan - Tambahan
    glPushMatrix();
    glColor3fv(putih);
    glTranslatef(-0.4, 0.4, -0.2);
    glScalef(0.05, 0.35, 0.9);
    glutSolidCube(1.0);
    glPopMatrix();

    // Moncong/hidung mobil (depan)
    glPushMatrix();
    glColor3fv(ungu);
    glTranslatef(0.0, 0.0, 0.8);
    glScalef(1.0, 0.4, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();

    // Lampu depan (bola)
    glPushMatrix();
    glColor3fv(putih);
    glTranslatef(0.3, 0.1, 1.1);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3fv(putih);
    glTranslatef(-0.3, 0.1, 1.1);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    // Bagian belakang mobil
    glPushMatrix();
    glColor3fv(ungu);
    glTranslatef(0.0, 0.0, -0.9);
    glScalef(1.0, 0.4, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();

    // Lampu belakang (bola)
    glPushMatrix();
    glColor3fv(merah);
    glTranslatef(0.3, 0.1, -1.1);
    glutSolidSphere(0.08, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3fv(merah);
    glTranslatef(-0.3, 0.1, -1.1);
    glutSolidSphere(0.08, 10, 10);
    glPopMatrix();

    // Roda
    buatRoda(0.5, -0.15, 0.6); // Roda depan kanan
    buatRoda(-0.5, -0.15, 0.6); // Roda depan kiri
    buatRoda(0.5, -0.15, -0.6); // Roda belakang kanan
    buatRoda(-0.5, -0.15, -0.6); // Roda belakang kiri

    // --- Bayangan Mobil ---
    if (!isNight) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND); // Aktifkan blending untuk transparansi
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.1f, 0.1f, 0.1f, 0.4f); // Dark gray, semi-transparent
        glPushMatrix();
        glTranslatef(0.0, -0.45, 0.0); // Posisi bayangan sedikit di bawah mobil
        glScalef(1.2f, 0.05f, 2.2f); // Pipih dan lebar, menyesuaikan bentuk mobil
        glutSolidSphere(0.5, 20, 20); // Gunakan bola pipih untuk bayangan oval
        glPopMatrix();
        glDisable(GL_BLEND); // Nonaktifkan blending
        glEnable(GL_LIGHTING);
    }
}

// Membuat pohon sebagai rintangan
void buatPohon(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);
    glScalef(scale, scale, scale);

    // Batang pohon
    glColor3fv(coklatKayu);
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.0);
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun pohon
    glColor3fv(hijauDaun);
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);  
    glutSolidCone(1.5, 3.0, 10, 10);
    glPopMatrix();

    // --- Bayangan Pohon ---
    if (!isNight) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.1f, 0.1f, 0.1f, 0.4f); // Dark gray, semi-transparent
        glPushMatrix();
        glTranslatef(0.0, -0.1, 0.0); // Posisi bayangan di dasar pohon
        glScalef(1.8f, 0.05f, 1.8f); // Ukuran bayangan disesuaikan dengan alas daun
        glutSolidSphere(0.5, 20, 20); // Bola pipih untuk bayangan melingkar
        glPopMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

// Membuat batu sebagai rintangan
void buatBatu(float x, float z) {
    glPushMatrix();
    glTranslatef(x, -0.1, z);

    glColor3fv(abuBatu);
    glScalef(1.0, 0.6, 1.0);
    glutSolidSphere(1.0, 10, 10);

    // --- Bayangan Batu ---
    if (!isNight) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.1f, 0.1f, 0.1f, 0.4f); // Dark gray, semi-transparent
        glPushMatrix();
        glTranslatef(0.0, -0.2, 0.0); // Posisi bayangan di dasar batu
        glScalef(1.0f, 0.05f, 1.0f); // Ukuran bayangan disesuaikan dengan batu
        glutSolidSphere(0.5, 20, 20); // Bola pipih
        glPopMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

// Membuat rumah tipe 1 (rumah sederhana kotak)
void buatRumahTipe1(float x, float z, float rotasi) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);
    glRotatef(rotasi, 0.0, 1.0, 0.0);

    // Badan rumah
    glColor3fv(merahBata);
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(3.0, 2.0, 2.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // Atap rumah
    glColor3fv(coklatAtap);
    glPushMatrix();
    glTranslatef(0.0, 2.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(3.2, 2.5, 4, 10);
    glPopMatrix();

    // Pintu
    glColor3fv(coklatKayu);
    glPushMatrix();
    glTranslatef(0.0, 0.7, 1.3);
    glScalef(0.8, 1.4, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Jendela kiri
    glColor3fv(biruLangit);
    glPushMatrix();
    glTranslatef(-1.0, 1.3, 1.3);
    glScalef(0.8, 0.8, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Jendela kanan
    glPushMatrix();
    glTranslatef(1.0, 1.3, 1.3);
    glScalef(0.8, 0.8, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Bayangan Rumah Tipe 1 ---
    if (!isNight) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.1f, 0.1f, 0.1f, 0.4f); // Dark gray, semi-transparent
        glPushMatrix();
        glTranslatef(0.0, -0.1, 0.0); // Posisi bayangan di dasar rumah
        glScalef(3.2f, 0.05f, 2.7f); // Ukuran bayangan disesuaikan dengan alas rumah
        glutSolidCube(1.0); // Bentuk kubus pipih
        glPopMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

// Membuat rumah tipe 2 (rumah bertingkat)
void buatRumahTipe2(float x, float z, float rotasi) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);
    glRotatef(rotasi, 0.0, 1.0, 0.0);

    // Lantai 1
    glColor3fv(silver);
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(4.0, 2.0, 3.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Lantai 2
    glColor3fv(abu);
    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0);
    glScalef(3.5, 2.0, 2.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // Atap
    glColor3fv(kuning);
    glPushMatrix();
    glTranslatef(0.0, 4.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(3.0, 2.5, 4, 10);
    glPopMatrix();

    // Pintu lantai 1
    glColor3fv(hitam);
    glPushMatrix();
    glTranslatef(0.0, 0.7, 1.55);
    glScalef(1.0, 1.4, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Jendela lantai 1 kiri
    glColor3fv(biruLangit);
    glPushMatrix();
    glTranslatef(-1.5, 1.3, 1.55);
    glScalef(0.8, 0.8, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Jendela lantai 1 kanan
    glPushMatrix();
    glTranslatef(1.5, 1.3, 1.55);
    glScalef(0.8, 0.8, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Jendela lantai 2 tengah
    glPushMatrix();
    glTranslatef(0.0, 3.0, 1.3);
    glScalef(1.5, 0.8, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // --- Bayangan Rumah Tipe 2 ---
    if (!isNight) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.1f, 0.1f, 0.1f, 0.4f); // Dark gray, semi-transparent
        glPushMatrix();
        glTranslatef(0.0, -0.1, 0.0); // Posisi bayangan di dasar rumah
        glScalef(4.2f, 0.05f, 3.2f); // Ukuran bayangan disesuaikan dengan alas rumah
        glutSolidCube(1.0); // Bentuk kubus pipih
        glPopMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
    
    glPopMatrix();
}

// Membuat pohon kecil (semak/bush)
void buatPohonKecil(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0, z);
    
    // Daun pohon kecil (bentuk bola)
    glColor3fv(hijauDaun);
    glPushMatrix();
    glTranslatef(0.0, -0.1, 0.0);
    glutSolidSphere(0.5, 10, 10);
    glPopMatrix();
    
    // --- Bayangan Pohon Kecil ---
  if (!isNight) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.08f, 0.08f, 0.08f, 0.2f); 
    glPushMatrix();
    glTranslatef(0.0, -0.1f, 0.0); 

    glScalef(0.7f, 0.03f, 0.7f); // Lebar/panjang 0.7f, ketebalan 0.03f
    glutSolidSphere(0.5, 20, 20); // Gambar bola pipih
    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
} 
    glPopMatrix();
}

void buatFinishLine(float posZ) {
    float width = roadWidth;   // Lebar garis finish
    float depth = 2.0f;        // Panjang area finish
    float x = 0.0f;
    int rows = 4;
    int cols = 10;

    float cellWidth = width / cols;
    float cellDepth = depth / rows;

    glPushMatrix();
    glTranslatef(x, 0.01f, posZ);  // Sedikit di atas tanah untuk mencegah z-fighting
    glDisable(GL_LIGHTING);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if ((row + col) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f); // putih
            else
                glColor3f(0.0f, 0.0f, 0.0f); // hitam

            float x1 = -width / 2 + col * cellWidth;
            float z1 = row * cellDepth;
            float x2 = x1 + cellWidth;
            float z2 = z1 + cellDepth;

            glBegin(GL_QUADS);
            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x2, 0.0f, z1);
            glVertex3f(x2, 0.0f, z2);
            glVertex3f(x1, 0.0f, z2);
            glEnd();
        }
    }

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Membuat landscape dengan perumahan dan pohon
void buatLandscape() {
    // Tanah dasar
    glPushMatrix();
    glColor3fv(coklatTanah);
    glBegin(GL_QUADS);
    glVertex3f(-roadWidth * 15, -20.0, -10.0);
    glVertex3f(roadWidth * 15, -20.0, -10.0);
    glVertex3f(roadWidth * 15, -20.0, roadLength);
    glVertex3f(-roadWidth * 15, -20.0, roadLength);
    glEnd();
    glPopMatrix();
    
    // Rumput di sekeliling jalan
    glPushMatrix();
    glColor3fv(isNight ? rumputMalam : rumputSiang);

    // Kiri jalan
    glBegin(GL_QUADS);
    glVertex3f(-roadWidth * 15, -0.49, -10.0);
    glVertex3f(-roadWidth/2, -0.49, -10.0);
    glVertex3f(-roadWidth/2, -0.49, roadLength);
    glVertex3f(-roadWidth * 15, -0.49, roadLength);
    glEnd();
    
    // Kanan jalan
    glBegin(GL_QUADS);
    glVertex3f(roadWidth/2, -0.49, -10.0);
    glVertex3f(roadWidth * 15, -0.49, -10.0);
    glVertex3f(roadWidth * 15, -0.49, roadLength);
    glVertex3f(roadWidth/2, -0.49, roadLength);
    glEnd();
    
    glPopMatrix();
    
    // Jalan dengan tekstur
    glPushMatrix();
    glDisable(GL_LIGHTING);  
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, jalanTexture);

    glColor3f(1.0f, 1.0f, 1.0f);  
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-roadWidth/2, -0.48, -10.0);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(roadWidth/2, -0.48, -10.0);
    glTexCoord2f(5.0f, roadLength / 10.0f); glVertex3f(roadWidth/2, -0.48, roadLength);
    glTexCoord2f(0.0f, roadLength / 10.0f); glVertex3f(-roadWidth/2, -0.48, roadLength);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);    // Nyalakan lagi
    glPopMatrix();

    // Garis tengah jalan
    glDisable(GL_LIGHTING);
    glColor3fv(putih);
    glLineWidth(10.0);  
    glBegin(GL_LINES);
    for (int i = -10; i < roadLength; i += 5) {
        if (i % 10 == 0) {
            glVertex3f(0.0, -0.47, i);
            glVertex3f(0.0, -0.47, i + 3);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);  

    glPopMatrix();
    
    // Gambar dekorasi (rumah dan pohon)
    for (const auto& deco : decorations) {
        // Hanya gambar yang berada pada jarak tertentu dari mobil
        if (deco.z > carZ - 50 && deco.z < carZ + 150) {
            switch (deco.type) {
                case 0: // Rumah tipe 1
                    buatRumahTipe1(deco.x, deco.z, deco.rotasi);
                    break;
                case 1: // Rumah tipe 2
                    buatRumahTipe2(deco.x, deco.z, deco.rotasi);
                    break;
                case 2: // Pohon besar
                    buatPohon(deco.x, deco.z, 1.5);
                    break;
                case 3: // Pohon kecil
                    buatPohonKecil(deco.x, deco.z);
                    break;
            }
        }
    }
}

// Membuat langit
void buatLangitMalam() {
    static bool    starsInitialized = false;
    static float   starPos[100][2];    // 100 bintang, masing-masing (x,y)
    if (!starsInitialized) {
        starsInitialized = true;
        srand(42);  // seed tetap agar setiap run reproducible
        for (int i = 0; i < 100; ++i) {
            starPos[i][0] = (rand() % 100) / 100.0f;          // x dalam [0,1)
            starPos[i][1] = 0.5f + (rand() % 50) / 100.0f;    // y dalam [0.5,1)
        }
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);

      // 1) Quad langit malam
      glBegin(GL_QUADS);
        glColor3fv(darkSky);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(1.0f, 0.5f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);
      glEnd();

      // 2) Bulan sabit
    GLfloat moonBright[] = {1.0f, 1.0f, 0.9f};  // Putih kekuningan
    GLfloat skyColor[]   = {0.05f, 0.05f, 0.1f}; // Langit malam (gelap)

    // Bulan utama (terang)
    glColor3fv(moonBright);
    glPushMatrix();
    glTranslatef(0.8f, 0.85f, 0.0f);
    glutSolidSphere(0.1, 50, 50);
    glPopMatrix();

    // Bayangan bulan (seolah bulan sabit)
    glColor3fv(skyColor);
    glPushMatrix();
    glTranslatef(0.82f, 0.85f, 0.01f); 
    glutSolidSphere(0.1, 50, 50);
    glPopMatrix();
    
      // 3) Bintang
      glPointSize(2.0f);
      glBegin(GL_POINTS);
        glColor3fv(starColor);
        for (int i = 0; i < 100; ++i) {
            glVertex2f(starPos[i][0], starPos[i][1]);
        }
      glEnd();

      // Kembalikan state
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menggambar awan
void gambarAwan(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f); // Putih bersih

    // 3 lingkaran
    glutSolidSphere(0.04, 20, 20);
    glTranslatef(0.03f, 0.01f, 0.0f);
    glutSolidSphere(0.05, 20, 20);
    glTranslatef(0.03f, -0.01f, 0.0f);
    glutSolidSphere(0.04, 20, 20);
 
    glPopMatrix();
}

// Fungsi langit siang + awan
void buatLangitSiang() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // 1) Langit biru terang (gradasi)
    glBegin(GL_QUADS);
        glColor3f(0.6f, 0.85f, 1.0f);  // Biru muda terang (bawah)
        glVertex2f(0.0f, 0.5f);
        glVertex2f(1.0f, 0.5f);
        glColor3f(0.3f, 0.7f, 1.0f);  // Biru lebih dalam (atas)
        glVertex2f(1.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);
    glEnd();

    // 2) Matahari
    glColor3f(1.0f, 1.0f, 0.8f); // Kuning terang (lebih putih)
    glPushMatrix();
    glTranslatef(0.85f, 0.85f, 0.0f);
    glutSolidSphere(0.1, 50, 50);
    glPopMatrix();

    // 3) Awan-awan
    gambarAwan(0.2f, 0.9f);
    gambarAwan(0.4f, 0.85f);
    gambarAwan(0.65f, 0.88f);

    // Restore OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Memeriksa tabrakan antara mobil dan rintangan
bool checkCollision() {
    float carSize = 1.5f;
    
    for (const auto& obstacle : obstacles) {
        float dx = carX - obstacle.x;
        float dz = carZ - obstacle.z;
        float distance = sqrt(dx*dx + dz*dz);
        
        float collisionThreshold = (obstacle.type == 0) ? 1.5f : 1.8f; 
        
        if (distance < collisionThreshold) {
            return true;
        }
    }
    
    // Memeriksa jika mobil keluar jalur
    if (fabs(carX) > roadWidth/2) {
        return true;
    }
    
    return false;
}

// Membuat rintangan
void generateObstacles() {
    srand(time(NULL));
    
    for (int z = 20; z < roadLength; z += 15) {
        if (rand() % 100 < 65) { // 65% kemungkinan ada rintangan
            Obstacle obs;
            obs.z = z;
            obs.x = (rand() % int(roadWidth) - roadWidth/2) * 0.8f; // posisi acak dalam jalan
            obs.type = 0; // jenis rintangan = 0 (batu)
            obstacles.push_back(obs);
        }
    }
}

// Membuat dekorasi perumahan dan pohon secara acak
void generateDecorations() {
    decorations.clear(); // Hapus dekorasi sebelumnya
    srand(time(NULL) + glutGet(GLUT_ELAPSED_TIME));
    
    // Jarak dari pinggir jalan ke perumahan di kiri dan kanan
    float leftSide = -roadWidth * 2.5f;
    float rightSide = roadWidth * 2.5f;
    
    // Buat perumahan dan pohon di sepanjang jalan
    for (int z = -10; z < roadLength; z += 5) {
        // Sisi kiri jalan
        if (rand() % 100 < 95) { // 95% kemungkinan ada dekorasi
            Decoration deco;
            deco.z = z;
            deco.x = leftSide + (rand() % 20) / 10.0f; // Posisi acak di sekitar sisi jalan
            deco.type = rand() % 4; // 0-1: rumah, 2-3: pohon
            deco.rotasi = (rand() % 4) * 90.0f; // Rotasi acak (0, 90, 180, 270 derajat)
            decorations.push_back(deco);
        }
        
        // Sisi kanan jalan
        if (rand() % 100 < 85) { // 85% kemungkinan ada dekorasi
            Decoration deco;
            deco.z = z + (rand() % 5); // Sedikit offset untuk menghindari keseragaman
            deco.x = rightSide - (rand() % 20) / 10.0f; // Posisi acak di sekitar sisi jalan
            deco.type = rand() % 4; // 0-1: rumah, 2-3: pohon
            deco.rotasi = (rand() % 4) * 90.0f; // Rotasi acak (0, 90, 180, 270 derajat)
            decorations.push_back(deco);
        }
        
        // Tambahkan beberapa pohon kecil secara acak
        if (rand() % 100 < 50) {
            Decoration deco;
            deco.z = z + (rand() % 10);
            // Tempatkan di antara jalan dan perumahan
            if (rand() % 2 == 0) {
                deco.x = -roadWidth/2 - (rand() % 10) / 5.0f; // Sisi kiri jalan
            } else {
                deco.x = roadWidth/2 + (rand() % 10) / 5.0f; // Sisi kanan jalan
            }
            deco.type = 3; // Pohon kecil
            deco.rotasi = 0.0f;
            decorations.push_back(deco);
        }
    }
}

// Fungsi untuk menampilkan teks di layar
void renderText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// Fungsi untuk menampilkan teks dengan font yang lebih kecil
void renderSmallText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text);
        text++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Atur posisi kamera berdasarkan posisi mobil
    if (topView) {
        // Kamera tampak atas agak miring
        gluLookAt(
            carX, 20.0, carZ - 30.0,  // Posisi kamera
            carX, 0.0, carZ,          // Titik yang dilihat
            0.0, 1.0, 0.0             // Vektor up
        );
    } else if (thirdPersonView) {
        // Tampilan third-person di belakang mobil
        gluLookAt(
            carX, 0.8, carZ + 0.5,           // Posisi kamera
            carX, 0.8, carZ + 10.0,          // Titik yang dilihat
            0.0, 1.0, 0.0                    // Vektor up
        );
    } else if(sideTopView){
        gluLookAt(
            carX + 7.0, 7.0, carZ - 10.0,   // Posisi kamera
            carX, 0.0, carZ,              // Titik yang dilihat
            0.0, 1.0, 0.0                 // Vektor up
        );
    } else {
        // Tampilan first-person dari dalam mobil       
        gluLookAt(
            carX, 1.0 + cameraHeight, carZ - cameraDistance,    // Posisi kamera
            carX, 0.5, carZ + 10.0,                             // Titik yang dilihat
            0.0, 1.0, 0.0                                       // Vektor up
        );
    }
    
    // Atur pencahayaan
    GLfloat posisiCahaya[] = {0.0, 50.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, posisiCahaya);

    // Membuat langit sesuai mode
    if (isNight) {
        buatLangitMalam();
    } else {
        buatLangitSiang();
    }

    // Membuat landscape
    buatLandscape();
    
    // Menggambar rintangan
    for (const auto& obstacle : obstacles) {
        if (obstacle.z > carZ - 10 && obstacle.z < carZ + 100) {
            if (obstacle.type == 0) {
                buatBatu(obstacle.x, obstacle.z);
            } else {
                buatPohon(obstacle.x, obstacle.z, 1.0f);
            }
        }
    }
    buatFinishLine(finishZ);
    // Menggambar mobil
    glPushMatrix();
    glTranslatef(carX, 0.0, carZ);
    buatMobil();
    glPopMatrix();
    
    // Tampilkan teks game status
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    
    // Tampilkan instruksi jika game belum dimulai
    if (!gameStarted && !gameOver) {
        renderText(300, 300, "Tekan UP untuk mulai");
    }
    
    // Tampilkan game over jika game sudah berakhir
    if (gameOver) {
        glColor3f(1.0, 0.0, 0.0);
        renderText(350, 300, "GAME OVER");
        
        glColor3f(1.0, 1.0, 1.0);
        renderSmallText(350, 280, "Tekan R untuk restart");
    } else {    
        if (carZ >= finishZ) {
            glColor3f(1.0, 1.0, 0.0);
            renderText(350, 320, "FINISH!");
        }
    }

    // Tampilkan informasi pada layar
    if (gameStarted && !gameOver) {
        // Kecepatan
        char speedText[50];
        snprintf(speedText, sizeof(speedText), "Kecepatan: %.1f", carSpeed * 500);
        renderSmallText(10, 580, speedText);
        // Mode Siang/Malam
        const char* modeStatus = "Ubah Mode (M)";
        renderSmallText(10.0, 540.0, modeStatus);
        // Posisi Kamera
        const char * cameraStatus = "Ubah Kamera (C/B/T/F)";
        renderSmallText(10.0, 560.0, cameraStatus);
    }

    glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void update(int value) {
    if (gameStarted && !gameOver) {
        // Update posisi mobil
        carZ += carSpeed;
        
        // Memperbarui rotasi roda berdasarkan kecepatan
        // Rotasi negatif membuat roda berputar ke depan saat mobil bergerak di arah Z positif
        wheelRotation -= carSpeed * 50.0f;  // Dikalikan faktor lebih besar agar rotasi lebih terlihat
        if (wheelRotation < 0.0f) wheelRotation += 360.0f;  // Menjaga sudut tetap antara 0–360
        
        // Memeriksa tabrakan
        if (checkCollision()) {
            gameOver = true;
        }
        
        // Restart timer untuk update berikutnya
        glutTimerFunc(16, update, 0); // ~60 FPS
    }

    if (carZ >= finishZ) {
        gameOver = false;
        carSpeed = 0.0f;
    }
    
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: // ESC
            exit(0);
            break;
        case 'r': // Restart game
        case 'R':
            if (gameOver) {
                carX = 0.0f;
                carZ = 0.0f;
                carSpeed = 0.0f;
                gameStarted = false;
                gameOver = false;
                obstacles.clear();
                generateObstacles();
                generateDecorations();
                glutTimerFunc(16, update, 0);
            }
            break;
        case 'f':
        case 'F':
            if (gameStarted && !gameOver) {
                topView = false;
                thirdPersonView = false;
                sideTopView = false;
            }
            break;
        case 'c':
        case 'C':
            thirdPersonView = !thirdPersonView;
            topView = false; 
            sideTopView = false;
            break;
        case 'w':
            if (gameStarted && !gameOver) {
                if (carSpeed < maxSpeed) {
                    carSpeed += acceleration;
                }
            }
            break;
        case 's':
            if (gameStarted && !gameOver) {
                if (carSpeed > 0) {
                    carSpeed -= acceleration * 2;
                    if (carSpeed < 0) carSpeed = 0;
                }
            }
            break;
        case 'a':
            if (gameStarted && !gameOver) {
                carX += steeringSpeed;
            }
            break;
        case 'd':
            if (gameStarted && !gameOver) {
                carX -= steeringSpeed;
            }
            break;
        case 't':
        case 'T':
            topView = !topView;
            thirdPersonView = false;
            sideTopView = false;
            break;
        case 'b':
        case 'B':
            sideTopView = !sideTopView;
            topView = false;
            thirdPersonView = false;
            break;
        case 'm': // Toggle malam/siang
        case 'M':
            isNight = !isNight;
            if (isNight) {
                glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Warna langit malam
            } else {
                glClearColor(0.6f, 0.85f, 1.0f, 1.0f); // Warna langit siang
            }
            break;
    }

    glutPostRedisplay(); // Redraw layar setelah input keyboard
}

void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            if (!gameStarted && !gameOver) {
                gameStarted = true;
                generateObstacles();
                generateDecorations();
                glutTimerFunc(16, update, 0);
            }
            if (gameStarted && !gameOver) {
                if (carSpeed < maxSpeed) {
                    carSpeed += acceleration;
                }
            }
            break;
        case GLUT_KEY_DOWN:
            if (gameStarted && !gameOver) {
                if (carSpeed > 0) {
                    carSpeed -= acceleration * 2;
                    if (carSpeed < 0) carSpeed = 0;
                }
            }
            break;
        case GLUT_KEY_RIGHT:
            if (gameStarted && !gameOver) {
                carX -= steeringSpeed;
            }
            break;
        case GLUT_KEY_LEFT:
            if (gameStarted && !gameOver) {
                carX += steeringSpeed;
            }
            break;
    }
    glutPostRedisplay();
}

// Tekstur jalan aspal
void buatTeksturAspal() {
    GLubyte data[TEXTURE_SIZE][TEXTURE_SIZE][3];

    for (int i = 0; i < TEXTURE_SIZE; ++i) {
        for (int j = 0; j < TEXTURE_SIZE; ++j) {
            int random = rand() % 100;
            int intensity;
            if (random < 20) {
                intensity = 100 + rand() % 30;  
            } else {
                intensity = 30 + rand() % 15;   
            }

            // R = G = B untuk abu-abu netral
            data[i][j][0] = intensity;
            data[i][j][1] = intensity;
            data[i][j][2] = intensity;
        }
    }

    glGenTextures(1, &jalanTexture);
    glBindTexture(GL_TEXTURE_2D, jalanTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Car Racing Game - Obstacle Drift");

    if (isNight)
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);  // Awal malam
    else
        glClearColor(0.6f, 0.85f, 1.0f, 1.0f);    // Awal siang
        
    // Mengaktifkan fitur 3D
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glEnable(GL_TEXTURE_2D);
    buatTeksturAspal();

    glutMainLoop();
    return 0;
}