#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
// static ushort *crt = (ushort*)P2V(0xb8000);
int main(int argc, char *argv[]){

	if(argc != 2){
		printf(1, "less [fileName]");
	}

	int file = open(argv[1], O_RDONLY);

	if(file < 0){
		printf(2, "error when opening file %s", argv[1]);
		exit();
	}
	char c;
	char input;

	int r, i;

	int charCount = 0;
	for(i = 0; i < 23; i++){
		r =	read(file, &c, sizeof(c));
		while(r > 0){
			if(r < 0)exit();
			charCount++;
			printf(1, "%c", c);
			if(c == '\n' || charCount == 80){
				charCount = 0;
				break;
			}
			r =	read(file, &c, sizeof(c));
		}
		if(r < 0)exit();
	}
	while (r > 0) {
		r =	read(file, &c, sizeof(c));
		charCount++;
		if(c != '\n'){
			printf(1, "%c", c);
		}
		if(c == '\n' || charCount == 80){
			read(1, &input, 1);
			charCount = 0;
		}
		if(input == 'q'){
			break;
		}
	}	
	printf(1, "\n");

    exit();

}
