MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

OUTPUT_FORMAT("elf32-littlearm")

ENTRY(__reset)

SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
    } > FLASH


    .ARM.extab :
    {
        *(.ARM.extab*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH
    __exidx_end = .;

    .data :
    {
        *(.data*)
    } > RAM

    .bss :
    {
        *(.bss*)
    } > RAM

    end = .;
    __stack = ORIGIN(RAM) + LENGTH(RAM);
}