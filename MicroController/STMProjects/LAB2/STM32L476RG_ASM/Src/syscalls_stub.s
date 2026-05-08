.syntax unified
.thumb
.cpu cortex-m4

.global _write
.thumb_func
_write:
    bx lr

.global _read
.thumb_func
_read:
    bx lr

.global _close
.thumb_func
_close:
    bx lr

.global _lseek
.thumb_func
_lseek:
    bx lr
