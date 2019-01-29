/**
 * @file	stage2.ld
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

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
