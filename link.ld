ENTRY(_start)
SECTIONS
{
  . = 0x00100000;
  
  .text : {
    code = .; _code = .; __code = .;
	*(.__mbHeader)
    *(.text)
    . = ALIGN(4096);
  }
  
  .data : {
    data = .; _data = .; __data = .;
    *(.data)
    . = ALIGN(4096);
  }
  
  .bss : {
    bss = .; _bss = .; __bss = .;
	*(COMMON)
    *(.bss)
    . = ALIGN(4096);
  }
  
  end = .; _end = .; __end = .;
}
