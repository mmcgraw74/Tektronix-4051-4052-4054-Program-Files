#include <stdio.h>
#include <string.h>
int main(int argc, char **argv)
{
  FILE* fp;
  char str [150];
  int line;
  int line2;
  int newline=8000;
  char data [100];
  char data2 [100];
  char data3 [100];
  char data4 [100];
  int doBreak=0;
  int ret, ret2;
  int ch;
  fp = fopen(argv[1], "r");
  while (!doBreak && (1==fscanf(fp, "%[^\r\n]", data4 ))) {    
    ch = fgetc(fp);
    if (ch==-1) {doBreak=1;};
    if ((ch != '\r') && (ch != '\n')) {
      fseek(fp, SEEK_CUR, -1);
    } 
    ch = fgetc(fp);
    if (ch==-1) {doBreak=1;};
    if ((ch != '\r') && (ch != '\n')) {
      fseek(fp, -1, SEEK_CUR);
    } 
    ret = sscanf(data4, "%d DATA \"%[A-F0-9]\"",&line,data);
    ret2 = sscanf(data4, "%d data \"%[A-F0-9]\"",&line,data3);
    if ((ret != 2) && (ret2 !=2)) {
      ret = sscanf(data4, "%d DATA \"%[z]\"",&line,data);
      if (ret == 2) {
        fprintf(stdout, "%d DATA \"%s\"\r", newline++, data);
      } else {
        fprintf(stdout, "%s\r", data4);
      }
    } else {
      if(ret ==2) {
	strcpy(data2, data);
	if (strlen(data) > 30) {
	  data2[30]=0;
	  fprintf(stdout, "%d DATA \"%s\"\r", newline++, data2);
	  fprintf(stdout, "%d DATA \"%s\"\r", newline++, data+30);
	} else {
	  fprintf(stdout, "%d DATA \"%s\"\r", newline++, data2);
	}
      } else {
	strcpy(data2, data3);
	if (strlen(data3) > 30) {
	  data2[30]=0;
	  fprintf(stdout, "%d DATA \"%s\"\r", newline++, data2);
	  fprintf(stdout, "%d DATA \"%s\"\r", newline++, data3+30);
	} else {
	  fprintf(stdout, "%d DATA \"%s\"\r", newline++, data2);
	}
      }
    }
  }
  fclose(fp);
  return 0;
}
