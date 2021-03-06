//
// Created by SomeDude on 19.06.2019.
//
#include "fatfs/source/ff.h"
#include "fatfstest.h"

void main()
{
  FIL f;
  int w;
  FATFS fs;
  FCHK(f_mount(&fs, "", 1));
  if (f_open(&f, "/abc/a.txt", FA_CREATE_ALWAYS|FA_READ|FA_WRITE) == FR_OK) {
    FCHK(f_write(&f, "abc\ndef\n", 9, &w));
    printf("open & write %d\n", w);
    FCHK(f_sync(&f));
    FCHK(f_close(&f));
    FCHK(f_unlink("/abc/a.txt"));
  } else {
    printf("open error\n");
  }
}