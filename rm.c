#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

//fungsi untuk menampilkan nama direktori / file pada folder yang akan dihapus
char * listNama (char * path)
{
	static char buf[DIRSIZ + 1];
	char *p;
	
    // untuk menemukan karakter pertama setelah /
	for(p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
	
	// Return blank-padded name
	if (strlen(p) >= DIRSIZ) return p;
	memmove (buf, p, strlen(p));
	memset (buf + strlen(p), ' ', DIRSIZ - strlen(p));
	return buf;
}

void ls (char *path) {
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	if((fd = open(path, O_RDONLY)) < 0)
	{
		printf(2, "cannot open %s\n", path);
		return;
	}
	
	if(fstat(fd, &st) < 0)
	{
		printf(2, "cannot stat %s\n", path);
		close(fd);
		return;
	}
	
	switch(st.type)
	{
		case T_FILE:
			printf(1, "name = %s, type = file, size = %d sudah dihapus\n", listNama(path), st.size);
			unlink(path);
			break;
		
		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
				printf(1, "path too long\n");
				break;
			}

			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			
			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
				if (de.inum == 0) continue;
				memmove(p, de.name, strlen(de.name));
				p[strlen(de.name)] = 0;


				if (fstat(open(buf, O_RDONLY), &st) < 0)
				{
					printf(1, "cannot stat %s\n", buf);
					continue;
				}

				if (st.type == T_FILE) {
					printf(1, "name = %s, type = file, size = %d sudah dihapus\n", listNama(buf), st.size);
					unlink(buf);
				}

				else if (st.type == T_DIR) {
					printf(1, "name = %s, type = folder, size = %d sudah dihapus\n", listNama(buf), st.size);
					ls(buf);
				}
			}
			break;
		}
		unlink(path);
		close(fd);
}

void lsbintang(char *path) {
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	if((fd = open(path, 0)) < 0)
	{
		printf(2, "cannot open %s\n", path);
		return;
	}
	
	if(fstat(fd, &st) < 0)
	{
		printf(2, "cannot stat %s\n", path);
		close(fd);
		return;
	}
	
	switch(st.type)
	{
		case T_FILE:
			printf(1, "name = %s, type = file, size = %d sudah dihapus\n", listNama(path), st.size);
			unlink(path);
			break;
		
		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
				printf(1, "path too long\n");
				break;
			}

			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			
			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
				if (de.inum == 0) continue;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;

				if (stat(buf, &st) < 0)
				{
					printf(1, "cannot stat %s\n", buf);
					continue;
				}

				if (st.type == T_FILE) {
					printf(1, "name = %s, type = file, size = %d sudah dihapus\n", listNama(buf), st.size);
					unlink(buf);
				}

				else if (st.type == T_DIR) {
					if(strcmp(buf, "./.") || strcmp(buf,"./.."));
					printf(1,"name = %s, type = folder, size = %d Tidak bisa dihapus karena berupa direktori\n", buf, st.size);
				}
		}
		break;
	}
	close(fd);
}


int main(int argc, char *argv[]) {

    int fd;

    if (argc <= 1) {
        printf(1, "RM ERROR\n");
        exit();
    }

    else if (argc == 2) {
        if(strcmp(argv[1], "--help") == 0) {
            printf(1, "\n rm [OPTION] [FILE / DIREKTORI]\n");
            printf(1, " List Option : \n");
            printf(1, " no op   Menghapus File\n");
            printf(1, " -r      Menghapus Direktori beserta File di dalamnya\n");
            printf(1, "  *      Menghapus semua File pada direktori yg lg dikerjakan\n\n");
            
            exit();
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            if ((fd = open(argv[i+1], 0)) < 0) exit();
            ls(argv[i+1]);
            printf(1,"hapus folder %s\n", argv[i+1]);
        }

        else if (strcmp(argv[i], "*") == 0) {
            lsbintang(".");
            exit();
        }

		else {
            if ((fd = open(argv[1], 0)) < 0) exit();
            lsbintang(argv[1]);
        }
    }
}