void putchar(char c)
{
	asm("int $0x0010" : : "a"(0x0e00 | c));
}

void puts(const char *s)
{
	for (; *s; ++s)
		putchar(*s);
}
