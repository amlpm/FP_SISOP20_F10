#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]){

	if(argc < 2){
		printf(1, "touch [fileName]");
	}

	int err;
	for(int i=1; i<argc; i++) {
        if( (err = open(argv[i], O_CREATE | O_RDWR) ) < 0) {
            printf(1,"error when creating %s\n", argv[i]);
			continue;
        }
        close(err);
    }
    exit();

}
