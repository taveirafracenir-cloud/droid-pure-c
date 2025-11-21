%macro DEFINE_STRING 1
section .rodata
%1:  db %2,  0
;MSG: NO KERNEL FOUND!
DEFINE_STRING msg_no_kernel, "No Kernel Found!"
