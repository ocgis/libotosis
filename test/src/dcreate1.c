#include <osbind.h>
#include <stdio.h>

int
main(int argc, char * argv[])
{
  int    err;

  if(argc == 2)
  {
    err = Dcreate(argv[1]);
    fprintf(stderr, "err     = %d\n", err);
  }
  else
  {
    fprintf(stderr, "%s takes one argument: file name\n", argv[0]);
  }

  return 0;
}
