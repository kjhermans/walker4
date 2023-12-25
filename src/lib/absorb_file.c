#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/**
 * Absorbs a file into a memory buffer, malloced to include a
 * terminating zero (not given in the returned length), so you
 * can also use it as a string.
 */
int absorb_file
  (char* path, unsigned char** buf, unsigned* buflen)
{
  unsigned char tmp[ 1024 ];
  int fd = open(path, O_RDONLY, 0);

  if (fd < 0) {
    return -1;
  }
  *buf = 0;
  *buflen = 0;
  while (1) {
    int r = read(fd, tmp, sizeof(tmp));
    if (r <= 0) {
      return 0;
    } else {
      *buf = (unsigned char*)realloc(*buf, (*buflen) + r + 1);
      memcpy((*buf) + (*buflen), tmp, r);
      (*buflen) += r;
      (*buf)[ *buflen ] = 0;
    }
  }
}
