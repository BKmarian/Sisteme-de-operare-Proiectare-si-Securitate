#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>		/* printf, scanf, NULL */
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <string.h>

int
main (int argv, char *argc[])
{
  int fd;
  size_t count = 100;
  int read_bytes = 0;
  char *buf = (char *) malloc (sizeof (char) * count);
  fd = open (argc[1], O_RDONLY);
  if (fd == -1)
    {
      printf ("%s", strerror (errno));
      return -1;
    }
  while ((read_bytes = read (fd, buf, count)) != 0)
    {
      write (1, buf, read_bytes);
    }
  close (fd);
}
