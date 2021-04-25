#include <wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>

// fungsi algoritma enkripsi caesar cipher yg dipakai di 3C
char caesarcipher(char *msg, int shift)
{
    int i;
    char word;
    for (int i = 0; msg[i] != '\0'; ++i) // membaca tiap huruf di kalimatnya
    {
        word = msg[i]; // pindah huruf ke "word"
        if (word >= 'A' && word <= 'Z') // untuk huruf Uppercase
        {
            word = word + shift; // huruf saat ini dishift
            if (word > 'Z') // jika saat shift melebihi Z
            {
                word = word - 'Z' + 'A' - 1; // shift ke awal (misal Y dishift 5 jadi D)
            }
            msg[i] = word; // setelah word dishift, dikembalikan ke kalimat awal
        }
        else if (word >= 'a' && word <= 'z') // untuk huruf lowercase
        {
            word = word + shift; // huruf saat ini dishift
            if (word > 'z') // jika saat shift melebihi z
            {
                word = word - 'z' + 'a' - 1; // shift ke awal (misal z dishift 5 jadi e)
            }
            msg[i] = word; // setelah word dishift, dikembalikan ke kalimat awal
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        exit(EXIT_FAILURE);
    }

    // mulai membuat daemon
    pid_t pid, sid; // Variabel untuk menyimpan PID

    pid = fork(); // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
	* (nilai variabel pid < 0) */
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
	* (nilai variabel pid adalah PID dari child process) */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    // selesai membuat daemon

    // 3E. Membuat mode Killer menggunakan bash
    char killername[100];
    strcpy(killername, "/home/cliffton/Documents/Modul/Modul 2/killer.sh"); // killername = /home/cliffton/Documents/Modul/Modul 2/killer.sh
    FILE *killer = fopen(killername, "w"); // buat file dan buka killer.sh di /home/cliffton/Documents/Modul/Modul 2/ (sesuai killername)(w = create empty file)

    if (strcmp(argv[1], "-z") == 0)
    { // jika ada argumen -z maka akan kill semua proses
        fprintf(killer, "#!/bin/bash\nkillall -9 soal3\nrm %s", killername);
    }
    else if (strcmp(argv[1], "-x") == 0)
    { // jika ada argumen -x maka akan kill program utama tapi proses lain masih berjalan
        fprintf(killer, "#!/bin/bash\nkill %d\nrm %s", getpid(), killername);
    }
    else
    { // jika tidak ada argumen, tutup killer
        fclose(killer);
        exit(EXIT_FAILURE);
    }

    pid_t pidk = fork(); // difork. child process menjalankan chmod
    if (pidk == 0) // child process
    {
        char *arg[] = {"chmod", "x", killername, NULL}; // chmod x agar file menjadi executable
        execv("/bin/chmod", arg);
    }
    fclose(killer); // tutup killer

    char pesan[120] = "Download Success"; // pesan yg nanti bakal dienkripsi dan masuk ke status.txt

    // masuk program utama
    while (1)
    {
        char foldername[100] = "/home/cliffton/Documents/Modul/Modul 2/"; // direktori utama
        char strtime[100];
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(strtime, sizeof(foldername), "%Y-%m-%d_%H:%M:%S", tm); // string timestamp sesuai format di soal (dipakai di 3A)
        strcat(foldername, strtime); // foldername = foldername awal + timestamp (dipakai di 3A)

        // inisialisasi fork
        pid_t pid1 = fork();
        int status1;
        if (pid1 == 0) // child process
        {
            // fork lagi
            pid_t pid2 = fork();
            int status2;
            if (pid2 == 0) // child process
            {   
                // 3A. membuat direktori file sesuai dengan 'foldername' tadi
                char *arg1[] = {"mkdir", foldername, NULL};
                execv("/bin/mkdir", arg1);
            }
            
            else // main process
            { // 3B. Mendownload 10 gambar
                while ((wait(&status2)) > 0); // menunggu child process selesai
                for (int i = 0; i < 10; i++)
                {
                    // fork lagi
                    pid_t pid3 = fork();
                    if (pid3 == 0) // child process
                    {
                        time_t t = time(NULL); // detik epoch unix
                        struct tm *tp = localtime(&t);

                        char filename[100];
                        strcpy(filename, foldername); // copy foldername yg tadi ke filename
                        strcat(filename, "/"); // filename = filename + /

                        char timefilename[100];
                        strftime(timefilename, sizeof(timefilename), "%Y-%m-%d_%H:%M:%S", tp); // format timestamp untuk nama file
                        strcat(filename, timefilename); // filename = filename + format timestamp

                        char link[100];
                        sprintf(link, "https://picsum.photos/%d", (((int)t) % 1000) + 50); // ukuran = ((detik epoch unix modulo 1000) + 50) pixel persegi

                        char *arg2[] = {"wget", "-qO", filename, link, NULL}; // download link di atas
                        execv("/usr/bin/wget", arg2);
                    }
                    sleep(5); // jalankan setiap 5 detik
                }

                //3C. Membuat file status.txt
                char status[120];
                sprintf(status, "%s/status.txt", foldername); // status = foldername/status.txt
                caesarcipher(pesan, 5); // enkripsi pesan (Download Success) dengan caesar cipher shift 5
                FILE *download = fopen(status, "w"); // buat file status
                fprintf(download, "%s", pesan); // print pesan yg sudah dienkripsi tadi
                fclose(download); // tutup file
                //3C. Melakukan zip file
                // fork
                pid_t pid4 = fork();
                int status4;
                if (pid4 == 0) // child process
                {
                    char zipname[120];
                    strcpy(zipname, foldername); // copy 'foldername' ke zipname
                    strcat(zipname, ".zip"); // zipname = 'zipname'.zip
                    char *arg3[] = {"zip", "-j", "-r", "-m", zipname, foldername, NULL}; // buat zip dengan nama 'zipname' yg berisi 'foldername'
                    // -j = junk path. karena char zipname ada direktori, maka harus diabaikan menggunakan -j dan menyisakan nama filenya saja
                    // -r = recursive. semua file termasuk di subfolders masuk di zip
                    // -m = move. hapus file yg dizip setelah zip selesai
                    execv("/usr/bin/zip", arg3);
                }
                // Menghapus folder
                else // main process
                {
                    while ((wait(&status4)) > 0);
                    char *arg4[] = {"rm", "-d", foldername, NULL}; // hapus folder 'foldername'
                    execv("/bin/rm", arg4);
                }
            }
        }
        sleep(40); // jalankan setiap 40 detik (soal 3A)
    }
}
