/**
 * @file	stage2.c
 * @brief	uniboot - the universal bootloader project
 * @version	v1.0
 * @author	dogan c. karatas
 * @date	29/01/2019
 **/

/* Tell gcc to emit 16-bit code */
asm (".code16gcc");

void stage2() __attribute__((section (".stage2")));
void putchar(char c);
void puts(const char *s);

void putchar(char c)
{
	asm("int $0x0010" : : "a"(0x0e00 | c));
}

void puts(const char *s)
{
	for (; *s; ++s)
		putchar(*s);
}

void stage2()
{
	puts("Stage 2 loaded.\r\n");
	while (1);
}