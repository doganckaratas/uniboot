;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                          ;;
;;       "BootProg" Loader v 1.4 by Alexei A. Frounze (c) 2000-2005         ;;
;;                                                                          ;;
;;                                                                          ;;
;;                            Contact Information:                          ;;
;;                            ~~~~~~~~~~~~~~~~~~~~                          ;;
;; E-Mail:   alexfru@chat.ru                                                ;;
;; Homepage: http://alexfru.chat.ru                                         ;;
;; Mirror:   http://alexfru.narod.ru                                        ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                  Thanks:                                 ;;
;;                                  ~~~~~~~                                 ;;
;;      Thanks Thomas Kjoernes (aka NowhereMan) for his excelent idea       ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                 Features:                                ;;
;;                                 ~~~~~~~~~                                ;;
;; - FAT32 supported using BIOS int 13h function 42h (i.e. it will only     ;;
;;   work with modern BIOSes supporting HDDs bigger than 8 GB)              ;;
;;                                                                          ;;
;; - Loads particular COM or EXE file placed to the root directory of a disk;;
;;   ("ProgramName" variable holds name of a file to be loaded)             ;;
;;                                                                          ;;
;; - Provides simple information about errors occured during load process   ;;
;;   ("E" message for "Read Error" or "file Not Found")                     ;;
;;                                                                          ;;
;;                                                                          ;;
;;                             Known Limitations:                           ;;
;;                             ~~~~~~~~~~~~~~~~~~                           ;;
;; - Works only on the 1st MBR partition which must be a PRI DOS partition  ;;
;;   with FAT32 (File System ID: 0Bh,0Ch)                                   ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                Known Bugs:                               ;;
;;                                ~~~~~~~~~~~                               ;;
;; - All bugs are fixed as far as I know. The boot sector tested on my      ;;
;;   HDD.                                                                   ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                Memory Map:                               ;;
;;                                ~~~~~~~~~~~                               ;;
;;                 ┌────────────────────────┐                               ;;
;;                 │ Interrupt Vector Table │ 0000                          ;;
;;                 ├────────────────────────┤                               ;;
;;                 │     BIOS Data Area     │ 0040                          ;;
;;                 ├────────────────────────┤                               ;;
;;                 │ PrtScr Status / Unused │ 0050                          ;;
;;                 ├────────────────────────┤                               ;;
;;                 │   Image Load Address   │ 0060                          ;;
;;                 ├────────────────────────┤                               ;;
;;                 │    Available Memory    │ nnnn                          ;;
;;                 ├────────────────────────┤                               ;;
;;                 │     2KB Boot Stack     │ A000 - 512 - 2KB              ;;
;;                 ├────────────────────────┤                               ;;
;;                 │       Boot Sector      │ A000 - 512                    ;;
;;                 ├────────────────────────┤                               ;;
;;                                            A000                          ;;
;;                                                                          ;;
;;                                                                          ;;
;;                   Boot Image Startup (register values):                  ;;
;;                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                  ;;
;;  dl = boot drive number                                                  ;;
;;  cs:ip = program entry point                                             ;;
;;  ss:sp = program stack (don't confuse with boot sector's stack)          ;;
;;  COM program defaults: cs = ds = es = ss = 50h, sp = 0, ip = 100h        ;;
;;  EXE program defaults: ds = es = 50h, other stuff depends on EXE header  ;;
;;                                                                          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]

?                       equ     0
ImageLoadSeg            equ     60h     ; <=07Fh because of "push byte ImageLoadSeg" instructions

[SECTION .text]

[ORG 0]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Boot sector starts here ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        jmp     short   start
        nop
bsOemName               DB      "BootProg"      ; 0x03

;;;;;;;;;;;;;;;;;;;;;;
;; BPB1 starts here ;;
;;;;;;;;;;;;;;;;;;;;;;

bpbBytesPerSector       DW      ?               ; 0x0B
bpbSectorsPerCluster    DB      ?               ; 0x0D
bpbReservedSectors      DW      ?               ; 0x0E
bpbNumberOfFATs         DB      ?               ; 0x10
bpbRootEntries          DW      ?               ; 0x11
bpbTotalSectors         DW      ?               ; 0x13
bpbMedia                DB      ?               ; 0x15
bpbSectorsPerFAT        DW      ?               ; 0x16
bpbSectorsPerTrack      DW      ?               ; 0x18
bpbHeadsPerCylinder     DW      ?               ; 0x1A
bpbHiddenSectors        DD      ?               ; 0x1C
bpbTotalSectorsBig      DD      ?               ; 0x20

;;;;;;;;;;;;;;;;;;;;
;; BPB1 ends here ;;
;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;
;; BPB2 starts here ;;
;;;;;;;;;;;;;;;;;;;;;;

bsSectorsPerFAT32               DD      ?               ; 0x24
bsExtendedFlags                 DW      ?               ; 0x28
bsFSVersion                     DW      ?               ; 0x2A
bsRootDirectoryClusterNo        DD      ?               ; 0x2C
bsFSInfoSectorNo                DW      ?               ; 0x30
bsBackupBootSectorNo            DW      ?               ; 0x32
bsreserved             times 12 DB      ?               ; 0x34
bsDriveNumber                   DB      ?               ; 0x40
bsreserved1                     DB      ?               ; 0x41
bsExtendedBootSignature         DB      ?               ; 0x42
bsVolumeSerialNumber            DD      ?               ; 0x43
bsVolumeLabel                   DB      "NO NAME    "   ; 0x47
bsFileSystemName                DB      "FAT32   "      ; 0x52

;;;;;;;;;;;;;;;;;;;;
;; BPB2 ends here ;;
;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Boot sector code starts here ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start:
        cld

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; How much RAM is there? ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        int     12h             ; get conventional memory size (in KBs)
        shl     ax, 6           ; and convert it to paragraphs

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reserve some memory for the boot sector and the stack ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        sub     ax, 512 / 16    ; reserve 512 bytes for the boot sector code
        mov     es, ax          ; es:0 -> top - 512

        sub     ax, 2048 / 16   ; reserve 2048 bytes for the stack
        mov     ss, ax          ; ss:0 -> top - 512 - 2048
        mov     sp, 2048        ; 2048 bytes for the stack

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copy ourself to top of memory ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        mov     cx, 256
        mov     si, 7C00h
        xor     di, di
        mov     ds, di
        rep     movsw

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Jump to relocated code ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        push    es
        push    byte main
        retf

main:
        push    cs
        pop     ds

        mov     [bsDriveNumber], dl     ; store boot drive number

        and     byte [bsRootDirectoryClusterNo+3], 0Fh ; mask cluster value
        mov     esi, [bsRootDirectoryClusterNo] ; esi=cluster # of root dir

RootDirReadContinue:
        push    byte ImageLoadSeg
        pop     es
        xor     bx, bx
        call    ReadCluster             ; read one cluster of root dir
        push    esi                     ; save esi=next cluster # of root dir
        pushf                           ; save carry="not last cluster" flag

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Look for a COM/EXE program to be load and run ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        push    byte ImageLoadSeg
        pop     es
        xor     di, di                  ; es:di -> root entries array
        mov     si, ProgramName         ; ds:si -> program name

        mov     al, [bpbSectorsPerCluster]
        cbw
        mul     word [bpbBytesPerSector]; ax = bytes per cluster
        shr     ax, 5
        mov     dx, ax                  ; dx = # of dir entries to search in

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Looks for a file with particular name ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Input:  DS:SI -> file name (11 chars) ;;
;;         ES:DI -> root directory array ;;
;;         DX = number of root entries   ;;
;; Output: ESI = cluster number          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FindName:
        mov     cx, 11
FindNameCycle:
        cmp     byte [es:di], ch
        jne     FindNameNotEnd
        jmp     ErrFind                 ; end of root directory (NULL entry found)
FindNameNotEnd:
        pusha
        repe    cmpsb
        popa
        je      FindNameFound
        add     di, 32
        dec     dx
        jnz     FindNameCycle           ; next root entry
        popf                            ; restore carry="not last cluster" flag
        pop     esi                     ; restore esi=next cluster # of root dir
        jc      RootDirReadContinue     ; continue to the next root dir cluster
        jmp     ErrFind                 ; end of root directory (dir end reached)
FindNameFound:
        push    word [es:di+14h]
        push    word [es:di+1Ah]
        pop     esi                     ; si = cluster no.

;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load entire a program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

        push    byte ImageLoadSeg
        pop     es
        xor     bx, bx
FileReadContinue:
        call    ReadCluster             ; read one cluster of root dir
        jc      FileReadContinue

;;;;;;;;;;;;;;;;;;;
;; Type checking ;;
;;;;;;;;;;;;;;;;;;;

        push    byte ImageLoadSeg
        pop     ds
        mov     ax, ds                  ; ax=ds=seg the program is loaded to

        cmp     word [0], 5A4Dh         ; "MZ" signature?

        je      RelocateEXE             ; yes, it's an EXE program

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Setup and Run COM program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        sub     ax, 10h                 ; "org 100h" stuff :)
        mov     es, ax
        mov     ds, ax
        mov     ss, ax
        xor     sp, sp
        push    es
        push    word 100h
        jmp     short Run

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Relocate, setup and run EXE program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RelocateEXE:

        add     ax, [08h]               ; ax = image base
        mov     cx, [06h]               ; cx = reloc items
        mov     bx, [18h]               ; bx = reloc table pointer

        jcxz    RelocationDone

ReloCycle:
        mov     di, [bx]                ; di = item ofs
        mov     dx, [bx+2]              ; dx = item seg (rel)
        add     dx, ax                  ; dx = item seg (abs)

        push    ds
        mov     ds, dx                  ; ds = dx
        add     [di], ax                ; fixup
        pop     ds

        add     bx, 4                   ; point to next entry
        loop    ReloCycle

RelocationDone:

        mov     bx, ax
        add     bx, [0Eh]
        mov     ss, bx                  ; ss for EXE
        mov     sp, [10h]               ; sp for EXE

        add     ax, [16h]               ; cs
        push    ax
        push    word [14h]              ; ip
Run:
        mov     dl, [cs:bsDriveNumber]  ; let program know boot drive

        ; set the magic number so the program knows who has loaded it:
        mov     si, 16381 ; prime number 2**14-3
        mov     di, 32749 ; prime number 2**15-19
        mov     bp, 65521 ; prime number 2**16-15

        retf

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reads a FAT32 cluster        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Inout:  ES:BX -> buffer      ;;
;;           ESI = cluster no   ;;
;; Output:   ESI = next cluster ;;
;;         ES:BX -> next addr   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadCluster:
        mov     ax, [bpbBytesPerSector]
        shr     ax, 2                           ; ax=# of FAT32 entries per sector
        cwde
        mov     ebp, esi                        ; ebp=esi=cluster #
        xchg    eax, esi
        cdq
        div     esi                             ; eax=FAT sector #, edx=entry # in sector
        movzx   edi, word [bpbReservedSectors]
        add     edi, [bpbHiddenSectors]
        add     eax, edi

        push    dx                              ; save dx=entry # in sector on stack
        mov     cx, 1
        call    ReadSectorLBA                   ; read 1 FAT32 sector

        pop     si                              ; si=entry # in sector
        add     si, si
        add     si, si
        and     byte [es:si+3], 0Fh             ; mask cluster value
        mov     esi, [es:si]                    ; esi=next cluster #

        lea     eax, [ebp-2]
        movzx   ecx, byte [bpbSectorsPerCluster]
        mul     ecx
        mov     ebp, eax

        movzx   eax, byte [bpbNumberOfFATs]
        mul     dword [bsSectorsPerFAT32]

        add     eax, ebp
        add     eax, edi

        call    ReadSectorLBA

        mov     ax, [bpbBytesPerSector]
        shr     ax, 4                   ; ax = paragraphs per sector
        mul     cx                      ; ax = paragraphs read

        mov     cx, es
        add     cx, ax
        mov     es, cx                  ; es:bx updated

        cmp     esi, 0FFFFFF8h          ; carry=0 if last cluster, and carry=1 otherwise
        ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reads a sector using BIOS Int 13h fn 42h ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Input:  EAX = LBA                        ;;
;;         CX    = sector count             ;;
;;         ES:BX -> buffer address          ;;
;; Output: CF = 1 if error                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadSectorLBA:
        pushad

ReadSectorLBANext:
        pusha

        push    byte 0
        push    byte 0
        push    eax
        push    es
        push    bx
        push    byte 1
        push    byte 16

        mov     ah, 42h
        mov     dl, [bsDriveNumber]
        mov     si, sp
        push    ss
        pop     ds
        int     13h
        push    cs
        pop     ds

        add     sp, 16

        jc      short ErrRead

        popa
        dec     cx
        jz      ReadSectorLBADone2      ; last sector

        add     bx, [bpbBytesPerSector] ; adjust offset for next sector
        add     eax, byte 1             ; adjust LBA for next sector
        jmp     short ReadSectorLBANext

ReadSectorLBADone2:
        popad
        ret

;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error Messaging Code ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;

ErrRead:
ErrFind:
        mov     ax, 0E00h+'E'
        mov     bx, 7
        int     10h
        jmp     short $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Fill free space with zeroes ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                times (512-13-($-$$)) db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Name of a program to be load and run ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ProgramName     db      "STARTUP BIN"   ; name and extension must be padded
                                        ; with spaces (11 bytes total)

;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End of the sector ID ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;

                dw      0AA55h
