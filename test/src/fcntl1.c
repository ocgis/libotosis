#include <fcntl.h>
#include <mintbind.h>
#include <osbind.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>

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


typedef struct
{
  long          high_time;
  long          time;           /* This has to be signed!  */
  unsigned long mikroseconds;
} StatTime;

typedef struct
{
  unsigned long long dev;            /* inode's device */
  unsigned long      ino;            /* inode's number */
  unsigned long      mode;           /* inode protection mode */
  unsigned long      nlink;          /* number of hard links */
  unsigned long      uid;            /* user ID of the file's owner */
  unsigned long      gid;            /* group ID of the file's group */
  unsigned long long rdev;           /* device type */
  StatTime           atime;          /* time of last access, UTC */
  StatTime           mtime;          /* time of last data modification, UTC */
  StatTime           ctime;          /* time of last file status change, UTC */
  unsigned long long size;           /* file size, in bytes */
  unsigned long long blocks;         /* blocks allocated for file */
  unsigned long      blksize;        /* optimal blocksize for I/O */
  unsigned long      flags;          /* user defined flags for file */
  unsigned long      gen;            /* file generation number */
  
  long               res[7];         /* sizeof = 128 bytes */
} Stat;


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
  Stat   stat;
  int    err;

  if(argc == 2)
  {
    int fd;

    fd = Fopen(argv[1], O_RDONLY);

    if(fd < 0)
    {
      fprintf(stderr, "Fopen returned %d\n", fd);
    }
    else
    {
      err = Fcntl(fd, (long)&xattr, FSTAT);
      
      fprintf(stderr, "*** FSTAT ***\n");
      fprintf(stderr, "err     = %d\n", err);
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


      err = Fcntl(fd, (long)&stat, FSTAT64);
      
      fprintf(stderr, "\n*** FSTAT64 ***\n");
      fprintf(stderr, "err     = %d\n", err);
      fprintf(stderr, "dev     = %lld\n", stat.dev);
      fprintf(stderr, "ino     = %ld\n", stat.ino);
      fprintf(stderr, "mode    = 0x%04lx\n", stat.mode);
      fprintf(stderr, "nlink   = %ld\n", stat.nlink);
      fprintf(stderr, "uid     = %ld\n", stat.uid);
      fprintf(stderr, "gid     = %ld\n", stat.gid);
      fprintf(stderr, "rdev    = 0x%llx\n", stat.rdev);
      fprintf(stderr, "atime   = %s", ctime((time_t *)&stat.atime.time));
      fprintf(stderr, "mtime   = %s", ctime((time_t *)&stat.mtime.time));
      fprintf(stderr, "ctime   = %s", ctime((time_t *)&stat.ctime.time));
      fprintf(stderr, "size    = %lld\n", stat.size);
      fprintf(stderr, "blocks  = %lld\n", stat.blocks);
      fprintf(stderr, "blksize = %ld\n", stat.blksize);
      fprintf(stderr, "flags   = 0x%08lx\n", stat.flags);
      fprintf(stderr, "gen     = %ld\n", stat.gen);
    }
  }
  else
  {
    fprintf(stderr, "%s takes one argument: file name\n", argv[0]);
  }

  return 0;
}
