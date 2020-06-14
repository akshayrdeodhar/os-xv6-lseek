# os-xv6-lseek

* lseek system call in xv6. Does not handle "holes" which create sparse files (because that would required modifying \read to supply 0x00 when someone reads from a hole.

* lseek1.c, and corresponding executable

* lseek2.c, when filesize is not a multiple of 10, then it arbitrarily decides that the first size % 10 chunks will be 1 byte larger than the other chunks, and the chunks get copied accordingly. (Nice application of prefix sum!)

* test.txt contains data with which checks can be done easily.
