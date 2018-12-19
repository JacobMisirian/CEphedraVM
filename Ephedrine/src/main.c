#include <lib/cpu.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
    FILE * in = fopen (argv [1], "rb");
    fseek (in, 0, SEEK_END);
    int ossize = ftell (in);
    rewind (in);
    
    char os [ossize];
    fread (&os, ossize, 1, in);

    cpustate_t * cpu = cpu_init (os, ossize, 0xFFFF);
    cpu_power (cpu);
    cpu_free (cpu);
}
