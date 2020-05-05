#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char buffer[500];

//untuk print n baris
void head (int fd, char * name, int x) {
    int n, baris = 0;
    
    while ((n = read(fd, buffer, sizeof(buffer))) > 0 && baris < x) {
        for (int i = 0; i <= n && baris < x; i++) {
            
            //kalau bukan enter, berarti masih 1 baris, jd di print
            if (buffer[i] != '\n') printf(1, "%c", buffer[i]);
            //kalau sudah enter, ganti baris
            else {
                printf(1, "\n");
                baris++;
            }
        }
    }

    //kalau file nya ga bisa dibaca, berarti error
    if(n < 0) {
        printf(1, "ERROR\n");
        exit();
    }
}

//untuk print per byte / per karakter
void headC (int fd, char * name, int x) {
    int n;
    char str[10000];
  
    int j = strlen(str);
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        
        //untuk print per karakternya
        for (int i = 0; i < x; i++, j++) {
            str[j] = buffer[i];
            printf(1, "%c", str[j]);
        }
        printf(1, "\n");
        break;
    }
    return;
}

int main(int argc, char *argv[]) {
    int fd, fd1, fd2;

    if(argc <= 1) {
        printf(1, "HEAD ERROR\n");
        exit();
    }
  
    else if(argc==2) {
        if(strcmp(argv[1],"--help") == 0) {
            printf(1,"[HELP] Perintah :\n\n");
            printf(1," head [FILE] 	      	    Print 10 baris pertama\n");
            printf(1," head -n [N_ROW] [FILE]   Print N Row baris pertama\n");
            printf(1," head -v [FILE]           Print 10 Row baris pertama dan nama file pada\n");
            printf(1,"                          baris pertama,\n");
            printf(1," head -q [FILE1] [FILE2]  Print 10 Row baris pertama file1 dan file2\n");
            printf(1,"                          sehingga total ada 20 baris\n");
            
            exit();
        }
    }

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            if ((fd = open(argv[i+1], 0)) < 0) {
                printf(1, "Tidak bisa buka %s\n", argv[i+1]);
                exit();
            }
            head(fd, argv[i+1], 10);
            close(fd);
        }
        
        else if (strcmp(argv[i], "-v") == 0) {
            printf(1, "==> %s <==\n", argv[i+1]);
            if ((fd = open(argv[i+1], 0)) < 0) {
                printf(1, "Tidak bisa buka %s\n", argv[i+1]);
                exit();
            }
            head(fd, argv[i+1], 10);
            close(fd);
        }

        else if (strcmp(argv[i], "-n") == 0) {
            int x = atoi(argv[i+1]);
            if ((fd = open(argv[i+2], 0)) < 0) {
                printf(1, "Tidak bisa buka %s\n", argv[i+2]);
                exit();
            }
            head(fd, argv[i+3], x);
            close(fd);
        }

        else if (strcmp(argv[i], "-q") == 0) {
            if (((fd1 = open(argv[i+1], 0)) < 0) || (fd2 = open(argv[i+2], 0)) < 0) {
                printf(1, "Tidak bisa buka %s\n", argv[i+1]);
                printf(1, "Tidak bisa buka %s\n", argv[i+2]);
                exit();
            }
            // printf(1, "Untuk File %s\n", argv[i+1]);
            head(fd1, argv[i+1], 10);
            // printf(1, "Untuk File %s\n", argv[i+2]);
            head(fd2, argv[i+2], 10);
            close(fd1);
            close(fd2);
        }

        else if (strcmp(argv[i], "-c") == 0) {
            int x = atoi(argv[i+1]);
            if (((fd = open(argv[i+2], 0)) < 0)) {
                printf(1, "Tidak bisa buka %s\n", argv[i+2]);
                exit();
            }
            // printf(1, "Untuk File %s\n", argv[i+1]);
            headC(fd, argv[i+2], x);
            close(fd);
        }
    }
    exit();
}
