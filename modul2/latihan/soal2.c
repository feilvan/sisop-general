// Buatlah sebuah program yang dapat mengcopy folder beserta semua isi dari folder di /home/{USER}/Downloads
// ke dalam sebuah folder dengan format nama tanggal-bulan-tahun_jam:menit:detik (contoh: 26-03-2021_16:22:09).
// Gunakan fork, exec, dan wait.

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main() {
	  time_t rawtime;
  	struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
	  timeinfo = localtime(&rawtime);
	  strftime(buffer, 80, "%d-%m-%Y_%T", timeinfo);

    chdir("/home/xa"); // ganti "xa" jadi nama user
    pid_t child_id;
    int status;
    child_id = fork();
    if (child_id < 0) {
      exit(EXIT_FAILURE);
    }

    if (child_id == 0) {
      char *argv[] = {"mkdir", buffer, NULL};
      execv("/bin/mkdir", argv);
    } else {
      while ((wait(&status)) > 0);
      char *argv[] = {"cp", "-p", "-r", "Downloads", buffer, NULL};
      execv("/bin/cp", argv);
    }
}
