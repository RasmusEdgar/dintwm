# dintwm

Tiling window manager for AmigaOS 3.1.x

## Building from source

### Compilation SAS/C on AmigaOS
```
cd dintwm
mkmk
smake
```

### Compilation with GCC on AmigaOS (requires ADE)
```
cd dintwm
gcc -o dintwm dintwm.c -noixemul -Wall -Werror
```

## Running dintwm

Dintwm will ignore the workbench window and backdropped windows.

Open some windows.

Run to see available options:
```
dintwm -h
```

Try tiling them:
```
dintwm -t
```

With a gap for the workbench titlebar.
```
dintwm -g -b
``` 
