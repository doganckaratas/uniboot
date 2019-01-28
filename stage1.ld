SECTIONS
{
	. = 0x7c00;

	.data : {
		*(.data1);
		*(.load);
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
