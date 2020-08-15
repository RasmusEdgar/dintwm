# dintwm

Tiling window "manager" for AmigaOS 3.1.x.

**NOTE**
Dintwm is under rather heavy development, it is very much beta, is a hobby project and breakage may occur.

## Building from source

Install crosscompiler:  
https://github.com/bebbo/amiga-gcc

### Compilation with GCC on AmigaOS (requires ADE)
Not working atm. Use crosscompiler from bebbo.

### Cross-compilation on Linux for AmigaOS (requires bebbo's m68k-amigaos-gcc fork)
```
m68k-amigaos-gcc -o dintwm defkeyfuncs.c dintwm.c commodity.c -Wall -Wextra -Wtype-limits -noixemul
```
## Running dintwm as a commodity

Dintwm will ignore the workbench window and backdropped windows.

To run dintwm as a commodity:

1. Create a default tooltype (icon) for dintwm
2. Add options you need, the following are available:
  * POPKEY\_TILE=, which is default set to "rawkey control lcommand t", tile with master window layout (lcommand is Left Amiga Key, windows key in {Win,fs-}uae)
  * POPKEY\_HGRID=, which is default set to "rawkey control lcommand g", grid layout
  * POPKEY\_SPIRAL=, which is default set to "rawkey control lcommand f", fibonacci variant layout
  * POPKEY\_DWINDLE=, which is default set to "rawkey control lcommand d", another fibonacci variant layout
  * POPKEY\_RESTORE=, which is default set to "rawkey control lcommand r", enables restore of initial window placement
  * POPKEY\_SWITCHF=, which is default set to "rawkey control lcommand s", flip through layouts forwards
  * POPKEY\_SWITCHB=, which is default set to "rawkey control lcommand s", flip through layouts backwards
  * POPKEY\_SHELL=, spawns default newshell
  * TOPGAP=, which is default set to 0, sets the top gap of the screen to show workbench bar
  * DEFAULT\_TOPGAP=, which is default set to 0, sets automatically calculated top gap.
  * EXCL\_WTYPE=, accepts window title, which default is set to nothing, exludes windows based on title
  * INCL\_WTYPE=, accepts window title, which default is set to nothing, includes windows based on title
  * AUTO=, set to TRUE  (or anything really) to enable auto tiling. All new windows will be tiled.
3. Test it out using the following command: ```dintwm -C``` or simply run dintwm without options.
4. When done drag the dintwm icon file to wbstartup and dintwm should run after reboot.
5. Check its status in System->Tools->Commodities->Exchange, you can also kill dintwm from here.

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
- ~~Make dintwm dynamical, by saving position of windows before tiling.~~
- ~~Float -> Tile -> Float behaviour~~
- ~~Enable auto tiling when new windows appear.~~
- Add leftgap, rightgap and bottomgap
- Add dintwm bar
- Create dmenu like feature if no suitable menu is found on aminet

## Note on licensing

Some parts of the code (tile and fibonacci functions) are adapted from dwm, a DWM-LICENSE is therefore included. 

Ketopt.h from https://github.com/attractivechaos/klib is licensed under the MIT/X11 license.
