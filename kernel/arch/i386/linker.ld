/* The bootloader will look at this image and start execution at the symbol
   designated asthe entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
    /* Begin putting sections at 1 MiB, a conventonal place for kernels to be 
       loaded at by the bootloader. */
    . = 1M;

    /* First put the multiboot header, as it is required to be put very early
       in the image or the bootloader won't recognize the file format. Next
       we'll put the .text section. */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read only data. */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* The compiler may produce other sections, by default it will put them in a 
       segment with the same name. Smiply add stuff here as needed. */
}
