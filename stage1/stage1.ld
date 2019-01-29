/**
 * @file	stage1.ld
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

SECTIONS
{
	. = 0x7c00;

	.boot : {
		*(.boot_sector);
		*(.init_fn);
	}

	.text : {
		*(.text);
	}

	.rodata : {
		*(.rodata);
	}

	.bss : {
		*(.bss);
	}

	. = 0x7dfe;

	.signature : {
		SHORT(0xaa55);
	}

	/DISCARD/ :
	{
		*(.eh_frame);
	}

}
