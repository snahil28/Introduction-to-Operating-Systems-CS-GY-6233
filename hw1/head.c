#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void head(int fd, char *name, int lines) {
      int i, n, l; // i=index pos., n = size of read, l = no of lines read

      l = 0;

      while ((n = read(fd, buf, sizeof(buf))) > 0 && l<lines) {
            for (i = 0; i < n && l<lines; i++) {
                  if (buf[i] != '\n') {// print char until '\n' occurs
                        printf(1, "%c", buf[i]);
                  }
                  else {// print '\n' if appears
                     printf(1,"\n");
		     l++;	
                  }
                  if (l == lines) {//continue until required lines are printed
                        exit();
                  }
            }
      }
     if (n<0)//throw error if unable to read from file
    {
       printf(1, "head: ead error\n");
       exit();
    }
}

int main(int argc, char *argv[]) {
      int fd, lines, i;
      char *fname;

      fd = 0; //default case: no file name given, read from standard input
      fname = "";
      lines = 10; //by default 10 lines will be read

      if (argc <= 1) {
            head(fd, fname, lines);
            exit();
      }

      else {
            for (i = 1; i < argc; i++) {
                  if (atoi(argv[i]) == 0 && *argv[i] != '0' && *argv[i] != '-') {// size is not given, it is not a number
                        if ((fd = open(argv[i], 0)) < 0) {//can't open the file
                        //fd==0 (EOF), fd==-1 (error)
                        printf(1, "head: cannot open %s\n", argv[i]);
                        exit();
                        }
                  }
                  else {//size is given, it's a number
                        argv[i]++;
                        lines = atoi(argv[i]++);
                  }
            }
            head(fd, fname, lines);
            close(fd);
            exit();
      }
}

