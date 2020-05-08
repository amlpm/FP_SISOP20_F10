#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char *sourcePath, *destinationPath, *parentDir;;

//fungsi untuk copy isi source ke destination
void cpFile (char *destination, char *source) {
    int baca, tulis, buat, untukWrite;
    char buffer[128];

    if ((baca = open(source, O_RDONLY)) < 0 ) {
        printf(1, "Tidak bisa baca %s\n", source);
        return;
    }

    if ((buat = open(destination, O_CREATE)) < 0 ) {
        printf(1, "Tidak bisa buat %s\n", destination);
        return;
    }

    else {
        printf(1, "Bisa baca dan buat %s %s\n", source, destination);
        close(buat);
    }
    
    if ((tulis = open(destination, O_WRONLY)) < 0 ){
        printf(1, "Tidak bisa tulis ke %s\n", destination);
        return;
    }

    while ((untukWrite = read(baca, buffer, sizeof(buffer))) > 0) {
        write(tulis, buffer, untukWrite);
    }

    close(baca);
    close(tulis);
}

//penggabungan destination string ke source string
void strcat(char *destination, char *source) {
    int j = strlen(destination);
    for(int i = 0; i < strlen(source); ++i, j++){
        destination[j] = source[i];
    }
    destination[j] = '\0';
    return;
}

//untuk pindahin smua isi direktori ke direktori lain
void reverseDir(char * path) {

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    char *pathBaru          = malloc(256 * sizeof(char));
    char *sourceBaru        = malloc(256 * sizeof(char));
    char *destinationBaru   = malloc(256 * sizeof(char));
    char *temp              = malloc(256 * sizeof(char));

    if (chdir(path) < 0) printf(1, "Tidak bisa buka %s\n", path);

    if ((fd = open(".", 0)) < 0) printf(1, "Tidak bisa buka %s\n", path);

    if (fstat(fd, &st) < 0){
        printf(2, "Tidak bisa stat %s\n", path);
        close(fd);
        return;
    }

    if (2 + DIRSIZ + 1 > sizeof buf) printf(1, "Path terlalu panjang\n");

    strcpy(buf, ".");
    p = buf+strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0) continue;

        memmove(p, de.name, DIRSIZ);

        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0){
            printf(1, "Tidak bisa stat %s\n", buf);
            continue;
        }
        
        char *namaBaru = buf + 1; //karna buf = './namafile'
        if (!strcmp(namaBaru, "/.") || !strcmp(namaBaru, "/..")) continue;
        
        if (st.type == 1) {

            *pathBaru = '\0';
            ++namaBaru;
            strcat(pathBaru, destinationPath);
            strcat(pathBaru, "/");
            strcat(pathBaru, path);
            strcat(pathBaru, "/");
            strcat(pathBaru, namaBaru);

            mkdir(pathBaru);

            *pathBaru = '\0';
            strcat(pathBaru, path);
            strcat(pathBaru, namaBaru);

            strcpy(temp, parentDir);
            strcat(parentDir, namaBaru);
            strcat(parentDir, "/");
            reverseDir(namaBaru);
            unlink(namaBaru);
            strcpy(parentDir, temp);
        }
        
        else {
            *destinationBaru = '\0';
            *sourceBaru = '\0';

            strcat(sourceBaru, namaBaru);

            strcat(destinationBaru, destinationPath);
            strcat(destinationBaru, parentDir);
            strcat(destinationBaru, sourceBaru);
            cpFile(destinationBaru, namaBaru+1);
            unlink(namaBaru + 1);
        }
    }
    close(fd);
    chdir("..");
    unlink(path);
}

int file(char *path) {
    struct stat st;
    int fd;

    if((fd = open(path, 0)) < 0) return 1;

    if(fstat(fd, &st) < 0) return 1;
    close(fd);

    if(st.type == 2)return 1;

    return 0;
}

char* direktori(char * path) {
    int i = strlen(path) - 2;
    
    for(; i > 1 && path[i] != '/'; --i);
    if(path[0] == '/') return path + i + 1;
    return path;
}

int main(int argc, char **argv) {
    parentDir       = malloc(128*sizeof(char));
    sourcePath      = malloc(128*sizeof(char));
    destinationPath = malloc(128*sizeof(char));

    if (argc == 1){
        printf(1, "MV ERROR\n");
        exit();
    }

    else if (argc == 2){
        if(strcmp(argv[1], "--help") == 0) {
            printf(1, "\n mv [OPTION] [FILE / DIREKTORI ASAL] [FILE / DIREKTORI TUJUAN\n");
            printf(1, " List Option : \n");
            printf(1, " -b      Memindahkan isi direktori / file asal ke direktori / file tujuan\n");
            printf(1, " *       Memindahkan seluruh isi direktori asal ke direktori lainnya\n\n");
            
            exit();
        }
    }
    
    strcpy(sourcePath, argv[1]);
    strcpy(destinationPath, argv[2]);

    if (!strcmp(sourcePath, "*")) {
        strcpy(parentDir, "/");
        reverseDir(".");
        exit();
    }

    if (!file(sourcePath)) {
        
        sourcePath      = direktori(sourcePath);
        char *pathBaru  = malloc(256*sizeof(char));

        if (sourcePath[strlen(sourcePath) - 1 ] != '/') strcat(sourcePath, "/");

        if (file(destinationPath)) {

            strcpy(parentDir, direktori(destinationPath));
            if (parentDir[strlen(parentDir) - 1 ] != '/') strcat(parentDir, "/");
            
            strcpy(pathBaru, destinationPath);
            if (pathBaru[strlen(pathBaru) - 1] != '/') strcat(pathBaru, "/");

            for (int i = strlen(destinationPath) - 1; i > -1 && destinationPath[i] != '/'; destinationPath[i--] = '\0');
            mkdir(pathBaru);

            reverseDir(sourcePath);
            exit();
        }

        else {
            strcpy(parentDir, sourcePath);
            if (destinationPath[strlen(destinationPath) - 1] != '/') strcat(destinationPath, "/");
            
            strcat(pathBaru, destinationPath);
            strcat(pathBaru, sourcePath);
            mkdir(pathBaru);
            reverseDir(parentDir);
        }
    }
    
    else {
        cpFile(destinationPath, sourcePath);
        unlink(sourcePath);
    }
    exit();
}