#include <mintbind.h>
#include <osbind.h>
#include <stdio.h>

typedef struct
{
  unsigned short mode;		/* File type and access permissions */
  long index;			/* Inode */
  unsigned short dev;		/* Bios device */
  unsigned short reserved1;	/* Reserved */
  unsigned short nlink;		/* Number of links */
  unsigned short uid;		/* uid */
  unsigned short gid;		/* gid */
  long size;			/* File size in bytes */
  long blksize;			/* Block size */
  long nblocks;			/* Blocks used by file*/
  short mtime;			/* Time of last modification */
  short mdate;			/* Date of last modification */
  short atime;			/* Time of last access */
  short adate;			/* Date of last access */
  short ctime;			/* Time of file creation */
  short cdate;			/* Date of file creation */
  short attr;			/* Standard file attributes */
  short reserved2;		/* Reserved */
  long reserved3;		/* Reserved */
  long reserved4;		/* Reserved */
} _XATTR;


static
char *
time_str(short timeIn)
{
  static char time[30];

  sprintf(time,
          "%2d:%02d:%02d",
          (timeIn >> 11) & 0x1f,
          (timeIn >> 5)  & 0x3f,
          (timeIn << 1) & 0x3f);

  return time;
}


static
char *
date_str(short dateIn)
{
  static char date[30];

  sprintf(date,
          "%d-%02d-%02d",
          ((dateIn >> 9) & 0x7f) + 1980,
          (dateIn >> 5)  & 0xf,
          dateIn & 0x1f);

  return date;
}


int
main(int argc, char * argv[])
{
  _XATTR xattr;
  int    err;

  if(argc == 2)
  {
    err = Fxattr(0, argv[1], &xattr);

    fprintf(stderr, "mode    = 0x%04x\n", xattr.mode);
    fprintf(stderr, "index   = %ld\n", xattr.index);
    fprintf(stderr, "dev     = %d\n", xattr.dev);
    fprintf(stderr, "nlink   = %d\n", xattr.nlink);
    fprintf(stderr, "uid     = %d\n", xattr.uid);
    fprintf(stderr, "gid     = %d\n", xattr.gid);
    fprintf(stderr, "size    = %ld\n", xattr.size);
    fprintf(stderr, "blksize = %ld\n", xattr.blksize);
    fprintf(stderr, "nblocks = %ld\n", xattr.nblocks);
    fprintf(stderr, "mtime   = %s\n", time_str(xattr.mtime));
    fprintf(stderr, "mdate   = %s\n", date_str(xattr.mdate));
    fprintf(stderr, "atime   = %s\n", time_str(xattr.atime));
    fprintf(stderr, "adate   = %s\n", date_str(xattr.adate));
    fprintf(stderr, "ctime   = %s\n", time_str(xattr.ctime));
    fprintf(stderr, "cdate   = %s\n", date_str(xattr.cdate));
    fprintf(stderr, "attr    = 0x%04x\n", xattr.attr);
    fprintf(stderr, "err     = %d\n", err);
  }
  else
  {
    fprintf(stderr, "%s takes one argument: file name\n", argv[0]);
  }

  return 0;
}
