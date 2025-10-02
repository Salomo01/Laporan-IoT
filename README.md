# Anggota Kelompok
| Nama          | NRP          |
| ------------- | ------------ |
| Salomo | 5027221063 |
| Nama | nrp |
| Nama | nrp |

# 📡 Laporan Proyek: IoT - Pengiriman Data Ultrasonik ke ThingSpeak

## 1. Judul
**Implementasi Sistem IoT menggunakan ESP32 & Sensor Ultrasonik untuk Mengirimkan Data Jarak ke ThingSpeak**

---

## 2. Abstrak
Proyek ini mendemonstrasikan implementasi IoT sederhana dengan menggunakan modul WiFi (ESP32), sensor ultrasonik (HC-SR04) dan platform cloud **ThingSpeak**. Data jarak dibaca oleh sensor ultrasonik, dikonversi ke satuan cm dan inci, kemudian dikirim ke ThingSpeak setiap interval yang ditentukan. Selain itu, terdapat kontrol LED sederhana berdasarkan ambang jarak.

---

## 3. Tujuan
1. Menghubungkan ESP32 ke jaringan WiFi dan platform ThingSpeak.  
2. Membaca data dari sensor ultrasonik dan mengonversinya menjadi jarak (cm/inch).  
3. Mengirimkan data jarak ke channel ThingSpeak secara periodik.  
4. Menampilkan dan memverifikasi data melalui Serial Monitor dan dashboard ThingSpeak.

---

## 4. Alat dan Bahan
- ESP32 (atau board kompatibel yang mendukung WiFi)  
- Sensor ultrasonik HC-SR04 (Triger, Echo)  
- LED (opsional) dan resistor sesuai kebutuhan  
- Kabel jumper   
- Komputer dengan Arduino IDE terpasang  
- Akun ThinkSpeak (Write API Key)  

---

## 5. Metodologi / Langkah Pengerjaan
1. **Persiapan ThingSpeak**
   - Buat akun pada https://thingspeak.com dan buat channel baru.
   - Catat `Write API Key` untuk mengizinkan device mengirim data.

2. **Persiapan Hardware**
   - Hubungkan sensor HC-SR04:
     - TRIG → GPIO 13 (contoh)
     - ECHO → GPIO 12 (contoh)
   - Hubungkan LED ke GPIO 2 (jika digunakan), dengan resistor pembatas arus.

3. **Setup Arduino IDE**
   - Pastikan board ESP32 terpasang di Arduino IDE.
   - Install library yang diperlukan (bila perlu): `ESP32 core` sudah menyertakan `WiFi.h`.
   - Siapkan file `iot_thingspeak.ino` dengan mengganti SSID, password, dan Write API Key.

4. **Unggah Kode**
   - Unggah `iot_thingspeak.ino` ke board ESP32.
   - Buka **Serial Monitor** (baud rate 115200) untuk melihat log koneksi dan hasil pembacaan jarak.

5. **Verifikasi**
   - Pantau Serial Monitor untuk memastikan koneksi WiFi dan pesan "Data terkirim ke ThingSpeak".
   - Buka dashboard channel ThingSpeak dan cek apakah field1/field2 menerima data.

---

## 6. Penjelasan Kode (Singkat)
- **Nama file rekomendasi**: `iot_thingspeak.ino`  
- **Fungsi utama**:
  - Menghubungkan ESP32 ke WiFi menggunakan kredensial `ssid` dan `password`.
  - Mengukur durasi echo dari sensor ultrasonik dan menghitung jarak dalam cm dan inch.
  - Mengontrol LED berdasarkan kondisi jarak (`distanceCm < 10.0` → LED ON).
  - Mengirimkan data ke ThingSpeak via HTTP GET ke `api.thingspeak.com/update?api_key=...&field1=...&field2=...`.
  - Menjaga interval pengiriman **≥ 15 detik** (contoh menggunakan `delay(20000)` untuk 20 detik — sesuai kebijakan update ThingSpeak minimal 15 detik).

> Catatan: Ganti `ssid`, `password`, dan `writeAPIKey` di bagian atas kode sebelum meng-upload.

---

## 7. Hasil & Pembahasan

### 7.1 Hasil Pengukuran di Serial Monitor
<img width="1547" height="873" alt="Image" src="https://github.com/user-attachments/assets/c758fbe7-9897-47e2-b5b5-dab362e1474a" />  
*Gambar: Tampilan Serial Monitor yang menampilkan pembacaan jarak (cm & inch), serta konfirmasi pengiriman data ke ThingSpeak.*

**Pembahasan:**
- Serial Monitor menunjukkan pembacaan jarak secara berkala dan pesan "Data terkirim ke ThingSpeak" saat HTTP GET berhasil.
- Bila terjadi kegagalan koneksi, akan muncul pesan "Gagal connect ke ThingSpeak" — periksa koneksi WiFi atau API Key.

---

### 7.2 Tampilan Kode di Arduino IDE
<img width="1918" height="1078" alt="Image" src="https://github.com/user-attachments/assets/1696b76e-c21c-4982-9e06-d2258fcbac92" /> 
*Gambar: Kode yang sedang dibuka di Arduino IDE (penempatan library & inisialisasi).*

**Pembahasan:**
- Kode mencakup inisialisasi WiFi, konfigurasi pin, pembacaan sensor, dan fungsi `kirim_thingspeak()` untuk HTTP request.
- Pastikan board & port sudah benar sebelum mengunggah.

---

### 7.3 Proses Koneksi ke ThingSpeak
<img width="1126" height="702" alt="Image" src="https://github.com/user-attachments/assets/228181e1-a047-4805-9843-c2a6909fa0a1" />
*Gambar: Indikator proses koneksi/pengiriman saat program berjalan.*

**Pembahasan:**
- Gambar memperlihatkan langkah runtime: koneksi WiFi → membuat request → mengirim data.
- Jika server merespons 200 OK biasanya data tercatat pada channel.

---

### 7.4 Dashboard ThingSpeak (Verifikasi Data)
<img width="1900" height="971" alt="Image" src="https://github.com/user-attachments/assets/1be47a4a-51d4-4a61-af0f-c987ced65cb7" />
*Gambar: Dashboard ThingSpeak, grafik yang menampilkan data jarak yang diterima pada field-field channel.*

**Pembahasan:**
- Field1: nilai jarak (cm) — ditampilkan dalam bentuk grafik/plot.  
- Field2: nilai jarak (inch) — ditampilkan pula.
- Visualisasi real-time membantu memantau pola jarak selama percobaan.

---

### 7.5 API Key (Dokumentasi Internal)
<img width="1897" height="970" alt="Image" src="https://github.com/user-attachments/assets/d7915547-039b-417e-b83f-4001550000c6" />
*Gambar: Tangkapan layar API Key (JANGAN bagikan publik jika project bersifat privat).*

**Pembahasan:**
- Pastikan `Write API Key` disimpan aman. Jika repository publik, **jangan** commit API Key ke repo publik — gunakan variabel lingkungan atau file konfigurasi yang di-ignore (`.gitignore`).

---

## 8. Kesimpulan
- ESP32 berhasil terhubung ke WiFi dan mengirim data jarak yang dibaca dari sensor ultrasonik ke platform ThingSpeak.  
- Data tampil pada Serial Monitor dan divisualisasikan pada dashboard ThingSpeak.  
- Implementasi sederhana ini dapat diperluas untuk notifikasi (mis. lewat email/SMS) atau integrasi dengan layanan lain.

---
