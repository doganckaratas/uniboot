asm (".code16gcc");
asm ("jmp stage2");

void putchar(char c)
{
  asm ("int $0x0010" : : "a"(0x0e00 | c));
}

void puts(char* s)
{
  for (; *s; ++s)
    putchar(*s);
}

void stage2()
{
  puts("Stage 2 loaded.\r\n");
  while (1);
}
