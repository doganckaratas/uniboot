;vanilla bootloader code
;powerkernel experimental
;2017

;0x0000 - 0x0200 disk physical location.
;512 bytes - sector 0
;one stage loader.


BITS    16
ORG     0x7C00
Start:
   jmp   Skip
   nop

;****************************************;
; ----- OEM Parameter block, FAT32 ----- ;
;****************************************;

bOemName      db   "MSDOS4.0"   ; 8 bytes

; ----- FAT32 Bios Parameter Block ----- ;
wBlockSize      dw   0
bSecPerClus      db   0
wReservedSecs      dw   0
bNumFats      db   0
wRootEntries      dw   0
wSectors16      dw   0
bMediaType      db   0
wFATSz16      dw   0
wSecPerTrack      dw   0
wNumHeads      dw   0
lSecCache      dd   0   ; lHiddenSecs
lSectors32      dd   0

;*******************************************;
; ----- Extended BIOS Parameter Block ----- ;
;*******************************************;

lFATSz32      dd   0
wExtFlags      dw   0   ; Bits 0-3 = Active FAT, 7 = !FAT
wFSVer         dw   0
lRootCluster      dd   0
wFSInfo         dw   0
wBkBootSec      dw   0
BytesPerCluster      dd   0   ;Reserved 1
CurrentCluster      dd   0   ;Reserved 2
lDataSector      dd   0   ;Reserved 3
bDriveNum      db   0
bReserved      db   0
bBootSig      db   0
lVolSerial      dd   0xDEADF007
szVolLabel      db   "-----------"
bSysID         db   "COPYHERE"

; ----- Booting process pro ----- ;
Skip:
; ----- Adjust registers ----- ;
   cli
   xor ax, ax
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

; ----- Display load message ----- ;
   mov si, msgLoading
   call print_s
   extern kernel_main
   mov si, msgFound
   call print_s
   call kernel_main
   cli
   jmp $
   hlt

; ----- Predefined functions ----- ;
print_s:
   lodsb         ; get next character
   or al, al      ; check, if null-terminator
   jz end         ; yes? -> printing done
   mov ah, 0eh      ; no? -> print the character
   int 10h         ; call BIOS
   jmp print_s      ; repeat until null-terminator found

end:         ; Return upon function exe completion
   ret

; ----- Data ----- ;
msgLoading db "PowerKernel Bootloader v0.3 Initializing...", 0
msgFound db "Kernel Found! Booting...", 0

TIMES 510-($-$$) db 0
DW 0xAA55
