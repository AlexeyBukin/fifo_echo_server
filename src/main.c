#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MY_FIFO_FILE ".my_fifo"

int main()
{
  int       result, fd, maxDescriptor;
  char      input_str[1024];
  struct    timeval tv;

  int status = mkfifo(MY_FIFO_FILE, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
  if (status != 0)
  {
    printf("%s\n", "Cannot create FIFO");
    return (0);
  }

  // https://stackoverflow.com/questions/23433904/write-read-to-from-fifo-files-linux
  fd = open(MY_FIFO_FILE, O_RDWR); // RDWR to avoid EOF return to select

  fd_set readset;

  maxDescriptor = fileno(stdin) > fd ? fileno(stdin) : fd;

  while(1)
  {
    tv.tv_sec = 10;
    tv.tv_usec = 10;

    FD_ZERO(&readset);
    FD_SET(fileno(stdin), &readset);
    FD_SET(fd, &readset);

    result = select(maxDescriptor+1, &readset, NULL, NULL, &tv);

    if(result == -1)
      perror("select()");
    else if(result)
    {
      if( FD_ISSET(fileno(stdin), &readset) )
      {
        int read_byte_num = read(fileno(stdin), input_str, 1023);
        if (read_byte_num < 0)
        {
          printf("%s\n", "Error while reading");
          return (0);
        }
        input_str[read_byte_num] = '\0';
        printf("Console: %s\n", input_str);
      }
      if(input_str[0] == 'Q')
        if(input_str[1] == '\0')
          break;

      if( FD_ISSET(fd, &readset) )
      {
        int read_byte_num = read(fd, input_str, 1023);
        if (read_byte_num < 0)
        {
          printf("%s\n", "Error while reading");
          return (0);
        }
        input_str[read_byte_num] = '\0';
        printf("Client: %s\n", input_str);
      }
      if(input_str[0] == 'Q')
        if(input_str[1] == '\0')
          break;
    }
    else
    {
      printf("%s%d\n%s\n", "result is", result, "No additional commands, sleep...");
    }
//    sleep(2);
  }

  remove(MY_FIFO_FILE);
  return 0;
}
