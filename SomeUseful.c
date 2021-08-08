#define KiB(__v)  ((__v) << 10)
#define MiB(__v)  ((__v) << 20)
#define GiB(__v)  ((__v) << 30)


Sample code to check the function inside a library by calling dlopen.
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void) {
    void * func_addr_default = dlsym(RTLD_DEFAULT, "malloc");
    void * libname= dlopen("lib-name.so", RTLD_NOW);
    void * func_addr_dlopen = dlsym(lib-name, "funcname");

    printf("func fptr: %p\n", funcname);
    printf("func default: %p\n", func_addr_default);
    printf("func dlopen:  %p\n", func_addr_dlopen);

    dlclose(libname);
    return 0;
}

Some notes on compilation
The short answer is that you can't do that, because static libraries don't work that way.  Let me explain why:

I'm going to break things down into three stages: compilation (going from a .c file to a .o file), linking (going from one or more .o, plus libraries, to an executable) and loading (running that executable under QNX Neutrino).

When you compile a source file (e.g. qcc -c myfile.c -o myfile.o) and the compiler gets to a function call, it first checks if it can resolve it locally (is it in the same function) -- if so, it resolves it immediately.  If it can't do so, it "remembers", that is, adds some information to the .o file that says it needs that func (say funcA).  By the time it gets to the end of the .c file there'll generally be a whole list of these missing bits, "unresolved symbols".

Now, for the link stage (e.g. qcc myfile1.o myfile2.o -lmylib1 -lmylib2 -o myexecutable),  The linker starts with the list of unresolved symbols from myfile1.o, then starts processing myfile2.o -- if any of those unresolved symbols are in myfile2.o, the code gets copied into the executable, and they're taken off the list of unresolved symbols.  Of course, myfile2.o may have further unresolved symbols, and they get added to the list to resolve.  When we get to mylib1, let's assume it is a static library (libmylib1.a), in this case it is, essentially, treated like a myfile.o (or, really, a list of libfile1.o, libfile2.o, etc), and if any of those internal files in the library resolve symbols, they're added to the executable, and any symbols they need are added to the "unresolved" list.  Now if mylib2 is a shared object (libmylib2.so), then if any symbols are resolved in that shared library, instead of copying the code into the executable, instead the linker "remembers" that the symbols are resolved in the shared library libfile2.so, and the executable will have a "NEEDED" record for libfile2.so.  After all the explicitly listed libraries, next will be resolved the automatic libraries -- libc.so, libgcc.so, etc.

Then when you run the process, the runtime loader will look at all the NEEDED records, try to resolve them in your LD_LIBRARY_PATH and resolve the symbols from them.

Ok, now, why does libmylib2.so have "NEEDED" records?  Because shared objects are actually assembled by a linking stage -- symbols are resolved.
But libmylib1.a doesn't go that way, as I said, the various myfile.o files are just copied in (with a bit of header and wrapper data).

Now, back to your problem... how can you make sure that funcB() uses jemalloc when it calls malloc()?  Well, you can't enforce that in the code by just calling malloc(), but what might work would be for you to explicitly dlopen() libjemalloc.so and explicitly dlsym() the functions you want/need from there, and then call them through function pointers, rather than hoping the automatic linking will get the "right" ones.


