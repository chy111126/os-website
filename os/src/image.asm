;
; A minimal boot sector that prints a 'Hello, world' message
;

; Initialize the stack pointer to the address 0x8000
mov bp, 0x8000
mov sp, bp

; Our boot sector has been loaded into memory at 0x7c00
; Set our es segment register to 0x07c0
; the address to print is calculated (segment * 16 + offset)
mov ax, 0x07c0
mov es, ax

; Call BIOS service to print a string
mov bp, hello
mov ah, 0x13        ; BIOS function 13 - print a string to the screen
mov al, 0x01        ; argument, update cursor after printing
mov bl, 0x0b        ; argument, text color - magenta
mov cx, 12          ; argument, length of string (including the null terminator)
mov dh, 2           ; argument, row to put string
mov dl, 0           ; argument, column to put string
int 10h             ; call BIOS service

; Loop until the end of time
jmp $

hello db 'Hello World'

; Padding and magic BIOS number
times 510-($-$$) db 0
dw 0xaa55

; So far we've filled 512 bytes. Fill more until we have an entire floppy-size worth's.
; Reference: https://blog.benjdoherty.com/2017/08/07/Writing-a-minimal-boot-sector-for-the-v86-emulator/
times (163840-512) db 0
