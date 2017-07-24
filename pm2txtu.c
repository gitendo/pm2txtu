#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

	char src[65535];
	char dst[65535];
	char *psrc, *pdst, *ptmp;
	char secret[6] = { 0x13, 0xA2, 0xB4, 0x6C, 0x79, 0x4C };
	short int count, code;
	int	i,j,fs;
	FILE *fp;

	printf("\nPrincess Maker 2 (DOS ver.) script unwrapper\n");
	printf("written by: tmk, https://github.com/gitendo\n");
	
	memset(src, 0, sizeof(src));
	memset(dst, 0, sizeof(dst));

	fp = fopen(argv[1], "rb");
	fseek(fp, 0L, SEEK_END);
	fs = ftell(fp);
	rewind(fp);
	fread(src, fs, 1, fp);
	fclose(fp);

	psrc = &src[0];
	pdst = &dst[0];

	code = *(short int *) psrc;
	psrc += 2;
	if(code != 0x0401) {
		printf("\nProbably not script file, exiting...\n");
		return 1;
	}

	for(i = 2, j = 0; i < fs; i++, j++) {
		src[i] = src[i] - secret[j];
		if(j == 5)
			j -= sizeof(secret);
	}

	while(1) {
		code = *(short int *) psrc;
		psrc += 2;

		if(code == 0) {
			break;
		} 
		else if((code & 0x8000) > 0) {
			count = code & 0x7FFF;
			code = *(short int *) psrc;
			psrc += 2;
			ptmp = psrc;
			psrc = pdst - code;
			for(i = 0; i < count * 2; i++) {
				*pdst = *psrc;
				pdst++;
				psrc++;
			}
			psrc = ptmp;
		} else {
			count = code;
			for(i = 0; i < count * 2; i++) {
				*pdst = *psrc;
				pdst++;
				psrc++;
			}
		}
	}

	fs = pdst - &dst[0];
	fp = fopen(argv[1], "wb");
	fwrite(dst, fs, 1, fp);
	fclose(fp);
}
