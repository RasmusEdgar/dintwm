# dintwm

Tiling window "manager" for AmigaOS 3.1.x.

**NOTE**
Dintwm is under rather heavy development, it is very much beta, is a hobby project and breakage may occur.

## Building from source

### Compilation with GCC on AmigaOS (requires ADE)
```
cd dintwm
gcc -o dintwm dintwm.c -noixemul -Wall -Werror
```

### Cross-compilation on Linux for AmigaOS (requires bebbo's m68k-amigaos-gcc fork)
```
m68k-amigaos-gcc -o dintwm dintwm.c commodity.c -Wall -Wextra -Wtype-limits -noixemul
```

## Running dintwm as a commodity

Dintwm will ignore the workbench window and backdropped windows.

To run dintwm as a commodity:

1. Create a default tooltype (icon) for dintwm
2. Add options you need, the following are available:
  * POPKEY\_TILE=, which is default set to "rawkey control lshift t"
  * POPKEY\_HGRID=, which is default set to "rawkey control lshift g"
  * POPKEY\_SPIRAL=, which is default set to "rawkey control lshift f"
  * POPKEY\_DWINDLE=, which is default set to "rawkey control lshift d"
  * TOPGAP=, which is default set to 0
  * EXCL\_WTYPE=, accepts window title, which is default set to nothing
3. Test it out using the following command: ```dintwm -C```
4. When done drag the dintwm icon file to wbstartup and dintwm should run after reboot.
5. Check its status in System->Tools->Commodities->Exchange, you can also kill from here.

The tooltypes in the dintwm file could look like this:  
```
POPKEY_TILE=rawkey control shift z
POPKEY_HGRID=rawkey control shift x
POPKEY_SPIRAL=rawkey control shift c
POPKEY_DWINDLE=rawkey control shift v
TOPGAP=30
EXCL_WTYPE=Commodities
```

## Running dintwm from CLI

Again Dintwm will ignore the workbench window and backdropped windows.

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

## TODO

- ~~Create an optional Dintwm commodity to enable hotkeys~~
- Make dintwm dynamical, by saving position of windows before tiling. WIP
- Float -> Tile -> Float behaviour
- Think about other ways to include/exclude based on other things than title
- Add dintwm bar

## Note

Some parts of the code (tile and fibonacci functions) are adapted from dwm, a DWM-LICENSE is therefore included. 
