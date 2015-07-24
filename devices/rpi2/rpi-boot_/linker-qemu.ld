ENTRY (Start)

SECTIONS
{
	. = 0x10000;
	_start = .;
	_text_start = .;
	.text :
	{
		KEEP(*(.text.boot))
		*(.text)
	}
	. = ALIGN(4096);
	_text_end = .;
	
	_rodata_start = .;
	.rodata :
	{
		*(.rodata)
	}
	. = ALIGN(4096);
	_rodata_end = .;
	
	_data_start = .;
	.data :
	{
		*(.data)
	}
	. = ALIGN(4096);
	_data_end = .;

	_bss_start = .;
	.bss :
	{
		bss = .;
		*(.bss)
	}
	. = ALIGN(4096);
	_bss_end = .;
	_end = .;
}

