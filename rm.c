#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char *parent_dir;

//sama kyk strcat
void strcat(char *destination, char *source){
    
    int j = strlen (destination);
    int x = strlen (source);
    for (int i = 0; i < x; i++, j++) destination[j] = source[i];
    destination[j]='\0';
}

//kalu hapus file / direktori dalam folder, harus rekursif / baca semua file / direktori di dalamnya
//Fungsi ini untuk membaca seluruh file / direktori di dalam folder yg mw dihapus
void recursiveDir (char * path, int flag) {

    char buffer[264], *p;
    int fd;
    struct dirent de;
    struct stat st;

    //masuk ke direktori yang mau dihapus
    chdir(path);
    fd = open(".", 0);
    fstat(fd, &st);
    
    strcpy(buffer, ".");
    p = buffer + strlen(buffer);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        
        if (stat(buffer, &st) < 0){
            printf(1, "Tidak bisa stat %s\n", buffer);
            continue;
        }
        
        char *dalemDir = buffer + 1;

        if (strcmp(dalemDir, "/.") == 0 || strcmp(dalemDir, "/..") == 0) continue;

        if (st.type == 1) {
            ++dalemDir;

            char *temp = malloc(256*sizeof(char));
            strcpy(temp, parent_dir);       //temp = parentDir
            strcat(parent_dir, dalemDir);   //parent dir = dalemDir
            strcat(parent_dir, "/");        //parent dir  gabung /
            // printf(1, "%s dalemDir\n", dalemDir);

            recursiveDir(dalemDir, 1);
            unlink(dalemDir);
            
            printf(1,"hapus folder %s\n",dalemDir);
            strcpy(parent_dir, temp);
        }
        else {
            unlink(dalemDir+1);
	        printf(1,"hapus file %s\n", dalemDir+1);
        }
    }
    close(fd);
    chdir("..");

    if (flag == 1) unlink(path);
    else return;
}

int main(int argc, char *argv[]){
    parent_dir = malloc(64*sizeof(char));

    if (argc <= 1) {
        printf(1, "RM ERROR\n");
        exit();
    }

    else if (argc == 2) {
        if(strcmp(argv[1],"--help") == 0) {
            printf(1, "rm [OPTION] [FILE / DIREKTORI]\n");
            printf(1, "List Option : \n");
            printf(1, " -b      Menghapus File\n");
            printf(1, " -r      Menghapus Direktori beserta File di dalamnya\n");
            printf(1, "  *      Menghapus semua File pada direktori tertentu\n");
            
            exit();
        }
    }

    else if (argc == 3) {
      for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-r") == 0) recursiveDir(argv[i+1], 1);

            else if (strcmp(argv[i], "-b") == 0) unlink(argv[i+1]);

            else if (strcmp(argv[i], "*") == 0) recursiveDir(argv[i+1], 0);
        }
    }
}