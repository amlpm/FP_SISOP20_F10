#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void copyFile(int fd, int fd2){
	int r, w;
	char buf[512];
	while((r = read(fd, buf, sizeof(buf))) > 0){
		w = write(fd2, buf, r);
		if(w != r || w < 0)
			break;
	}
}

int main(int argc, char *argv[]){

	if(argc < 2){
		printf(1, "touch [fileName]");
	}

	int fd, fd2;

	for(int i=1; i<argc; i++) {

        if( (fd = open(argv[i], O_CREATE | O_RDONLY) ) < 0) {
            printf(2, "error when creating %s\n", argv[i]);
			continue;
        }

		if( (fd2 = open(argv[i]+1, O_CREATE | O_WRONLY) ) < 0) {
            printf(2, "error when creating %s\n", argv[i]);
			continue;
        }

		copyFile(fd, fd2);

        close(fd);
		close(fd2);

		unlink(argv[i]);

        if( (fd = open(argv[i], O_CREATE | O_WRONLY) ) < 0) {
            printf(2, "error when creating %s\n", argv[i]);
			continue;
        }

		if( (fd2 = open(argv[i]+1, O_CREATE | O_RDONLY) ) < 0) {
            printf(2, "error when creating %s\n", argv[i]);
			continue;
        }

		copyFile(fd2, fd);

        close(fd);
		close(fd2);

		unlink(argv[i]+1);

    }
    exit();

}
