#include <osbind.h>
#include <stdio.h>

int
main(int argc, char * argv[])
{
  int       err;
  _DISKINFO di;

  err = Dfree(&di, 0);

  fprintf(stderr, "b_free   = %ld\n", di.b_free);
  fprintf(stderr, "b_total  = %ld\n", di.b_total);
  fprintf(stderr, "b_secsiz = %ld\n", di.b_secsiz);
  fprintf(stderr, "b_clsiz  = %ld\n", di.b_clsiz);
  fprintf(stderr, "err      = %d\n", err);

  return 0;
}
