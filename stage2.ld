SECTIONS
{
  . = 0x0700;

  .text :
  {
    *(.stage2);
    *(.text);
  }

  .data :
  {
    *(.rodata);
  }

  /DISCARD/ :
  {
    *(.eh_frame);
  }
}
