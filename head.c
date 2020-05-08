#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char buf[512];

void head(int fd, char *name, int x) {
    int n, baris = 0;
    int i;
    
    while((n = read(fd, buf, sizeof(buf))) > 0 && baris < x) {
        for(i = 0; i <= n && baris < x; i++) {
            if(buf[i] != '\n') printf(1, "%c", buf[i]);
            else {
                printf(1, "\n");
                baris++;
            }
        }
    }

    if(n < 0) {
        printf(1, "ERROR\n");
        exit();
    }
}

void headC(int fd, char *name, int x) {
    int n;
    char str[10000];
  
    int j = strlen(str);
    while((n = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < x; i++, j++) {
            str[j] = buf[i];
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
            printf(1,"\n head -b [FILE]               Print 10 baris pertama\n");
            printf(1," head -n [N_ROW] [FILE]       Print N Row baris pertama\n");
            printf(1," head -v [FILE]               Print 10 Row baris pertama dan nama file pada\n");
            printf(1,"                              baris pertama\n");
            printf(1," head -q [FILE1] [FILE2]      Print 10 Row baris pertama file1 dan file2\n");
            printf(1,"                              sehingga total ada 20 baris\n");
            printf(1," head -c [N_BYTES] [FILE1]    Print N byte karakter dari File\n\n");
            
            exit();
        }
    }
        
    if (strcmp(argv[1], "-v") == 0) {
        for(int i = 2; i < argc; i++) {
            printf(1, "==> %s <==\n", argv[i]);
            if ((fd = open(argv[i], 0)) < 0) exit();
            head(fd, argv[i], 10);
            close(fd);
        }
    }

    else if (strcmp(argv[1], "-n") == 0) {
        for(int i = 2; i < argc; i++) {
            int x = atoi(argv[i]);
            if ((fd = open(argv[i+1], 0)) < 0) exit();
            head(fd, argv[i+1], x);
            close(fd);
        }
    }

    else if (strcmp(argv[1], "-q") == 0) {
        for(int i = 2; i < argc; i++) {
            if (((fd1 = open(argv[i], 0)) < 0) || (fd2 = open(argv[i+1], 0)) < 0) exit();
            head(fd1, argv[i], 10);
            // printf(1, "Untuk File %s\n\n", argv[i]);
            head(fd2, argv[i+1], 10);
            // printf(1, "Untuk File %s\n\n", argv[i+1]);
            close(fd1);
            close(fd2);
        }
    }

    else if (strcmp(argv[1], "-c") == 0) {
        for(int i = 2; i < argc; i++) {
            int x = atoi(argv[i]);
            if (((fd = open(argv[i+1], 0)) < 0)) exit();
            // printf(1, "Untuk File %s\n", argv[i+1]);
            headC(fd, argv[i+1], x);
            close(fd);
        }
    }

    else {
        for(int i = 1; i < argc; i++) {
            if ((fd = open(argv[i], 0)) < 0) exit();
            head(fd, argv[i], 10);
            close(fd);
        }
    }

    exit();
}
