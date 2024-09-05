void main(void);//  __attribute((section(".init")));

extern int lib_call(const char* str);

void main(void) 
{
    static int hello = 5;

    hello = lib_call("Hello from .rodata");
}
