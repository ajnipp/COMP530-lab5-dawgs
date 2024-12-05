// PID: 730477560
// PID: 730461954
// I pledge the COMP530 honor code.

#define READ_M 1
#define WRITE_M 0
#define DEFAULT_HDD_DISK_PATH "/dev/sda"
#define DEFAULT_SDD_DISK_PATH "/dev/sdb1"
#define DEFAULT_PATTERN "P"

/* Sets the disk path to read/write to */
int set_disk_path(char *path);

/* Sets the write pattern to use */
int set_write_pattern(char *pattern);

/* Sequentially write a range of logical blocks up to total_size. 

*  read: 1 for read, 0 for write
*  total_size: the total size of the range to write in bytes
*  granularity: the size of each write in bytes

*  Returns total time taken for the entire procedure, in milliseconds.
*/
long io_size(int read, long total_size, long granularity);

/* Sequentially write a range of logical blocks up to total_size, striding by stride.

*  read: 1 for read, 0 for write
*  total_size: the total number of bytes to write
*  granularity: the size of each write in bytes
*  stride: the number of bytes to stride by, measured from beginning of each write 

*  Returns total time taken for the entire procedure, in milliseconds.
*/
long io_stride(int read, long total_size, long granularity, long stride);

/* Randomly write to a range of logical blocks between min_offset and max_offset, up to total_size.

*  read: 1 for read, 0 for write
*  total_size: the total number of bytes to write
*  granularity: the size of each write in bytes
*  min_offset: the minimum offset in bytes to write to
*  max_offset: the maximum offset in bytes to write to

*  Returns total time taken for the entire procedure, in milliseconds.
*/
long random_io(int read, long total_size, long granularity, long min_offset, long max_offset);

