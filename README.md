# SudokuKita

Sudokukita adalah Proyek Laporan akhir untuk Mata Kuliah Dasar Pemrogramman

## Cara Compile dan Run

Program dicompile dengan menggunakan `gcc`.

Berikut adalah cara untuk compile dan run program ini:

> [!WARNING]
> Linux sedang dalam tahap percobaan, kemungkinan program tidak bisa dijalankan.

1. Buka terminal dan arahkan direktori Anda ada di dalam direktori program ini.
2. Ketikkan perintah sebegai berikut :
   1. Powershell (Windows):

        ```C
        gcc -O2 -Wall -Wextra '.\sudoku_4x4.c' -o `.\sudoku.exe`
        ```

   2. Terminal (Linux) _Untested_:

        ```C
        gcc -O2 -Wall -Wextra './sudoku_4x4.c' -o './sudoku_4x4'
        ```

3. Ketikkan perintah `./sudoku_4x4` atau `./sudoku.exe` untuk menjalankan program ini.

Argument untuk program ini adalah jumlah blank yang ingin diisi. Jika tidak diisi, maka defaultnya adalah 6.

```bash
./sudoku_4x4(.exe) [blank]    ## Mengirimkan jumlah blank
./sudoku_4x4(.exe) 6          ## Mengirimkan 6 blank 
```

Program ini akan menampilkan menu interaktif untuk memainkan permainan Sudoku. Anda dapat memilih nomor yang ingin di inputkan dan posisi untuk mengisiinya. Anda juga dapat memilih opsi untuk menampilkan jawaban atau keluar dari program.

## Cara Bermain

Untuk bermain, Anda dapat memilih nomor yang ingin di inputkan dan posisi untuk mengisiinya. Anda juga dapat memilih opsi untuk menampilkan jawaban atau keluar dari program. Selebihnya, anda dapat menjalankan program yang sudah di compile dan program akan memberikan instruksi untuk bermain.
