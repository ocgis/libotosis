#include <osbind.h>
#include <stdio.h>

int
main(void)
{
  _DTA dta;
  int  err;

  Fsetdta(&dta);

  err = Fsfirst("*",
                FA_HIDDEN |
                FA_SYSTEM |
                FA_LABEL  |
                FA_DIR    |
                FA_CHANGED);

  while(err == 0)
  {
    printf("dta_attribute = 0x%02x\n", dta.dta_attribute);
    printf("dta_time      = 0x%04x\n", dta.dta_time);
    printf("dta_date      = 0x%04x\n", dta.dta_date);
    printf("dta_size      = %ld\n", dta.dta_size);
    printf("dta_name      = %sx\n", dta.dta_name);
    printf("=============================\n");
    err = Fsnext();
  }

  printf("err = %d\n", err);

  return 0;
}
