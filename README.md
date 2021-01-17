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
m68k-amigaos-gcc -o dintwm timer.c dintwm.c commodity.c -Wall -Wextra -Wtype-limits -noixemul
```

## Demo

Vanilla without any tooltype tweaks:

![Vanilla](https://github.com/RasmusEdgar/dintwm/blob/master/demo/vanilla.gif)

Force tiling into specific screen area by setting tooltypes:  
TOPGAP=320   
LEFTGAP=320 

![Gap tiling into area](https://github.com/RasmusEdgar/dintwm/blob/master/demo/gap_to_area.gif)

## Running dintwm as a commodity

Dintwm will ignore the workbench window and backdropped windows.

To run dintwm as a commodity:

1. Create a default tooltype (icon) for dintwm
2. Add options you need, the following are available:
  * POPKEY\_TILE=, which is default set to "rawkey control lcommand t", tile with master window layout (lcommand is Left Amiga Key, windows key in {Win,fs-}uae)
  * POPKEY\_HGRID=, which is default set to "rawkey control lcommand g", grid layout
  * POPKEY\_SPIRAL=, which is default set to "rawkey control lcommand f", fibonacci variant layout
  * POPKEY\_DWINDLE=, which is default set to "rawkey control lcommand d", another fibonacci variant layout
  * POPKEY\_TAKESNAPSHOT=, which is default set to "rawkey control lcommand p", takes snapshot of window layout
  * POPKEY\_CLEANSNAPSHOT=, which is default set to "rawkey control lcommand p", deletes snapshot of window layout
  * POPKEY\_RESTORE=, which is default set to "rawkey control lcommand r", restores snapshot of window layout
  * POPKEY\_SWITCHF=, which is default set to "rawkey control lcommand s", flip through layouts forwards
  * POPKEY\_SWITCHB=, which is default set to "rawkey control lcommand s", flip through layouts backwards
  * POPKEY\_CMD_0..9=, spawns custom commands. Defaults set to "rawkey control lcommand 0..9"
  * POPKEY\_CXM_EXIT=, which is default set to "rawkey control lcommand q", exits commodity
  * TOPGAP=, which is default set to 0, sets the top gap of the screen to show workbench bar
  * RIGHTGAP=, which is default set to 0, sets a right gap 
  * LEFTGAP=, which is default set to 0, sets a left gap
  * BOTTOMGAP=, which is default set to 0, sets a bottom gap
  * DEFAULT\_TOPGAP=, which is default set to 0, sets automatically calculated top gap.
  * EXCL\_WTYPE\_0..9=, Enumerated option. Accepts window title, which default is set to nothing, excludes windows based on title
  * INCL\_WTYPE\_0..9=, Enumerated option. Accepts window title, which default is set to nothing, includes windows based on title
  * AUTO=, set to TRUE  (or anything really) to enable auto tiling. All new windows will be tiled.
  * AUTO_INTERVAL_MICRO=, Set frequency for auto tiling. Default is 20000 microseconds
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
dintwm -g -u
``` 

With custom gaps all around:
```
dintwm -U 45 -B 45 -L 45 -R 45 -t
```

That is, you could basically set tiling to occur where ever you decide on the screen.

```
dintwm -U 320 -L 320 -t
```

## TODO

- ~~Create an optional Dintwm commodity to enable hotkeys~~
- ~~Make dintwm dynamical, by saving position of windows before tiling.~~
- ~~Float -> Tile -> Float behaviour~~
- ~~Enable auto tiling when new windows appear.~~
- ~~Add leftgap, rightgap and bottomgap~~
- Add dintwm bar
- Create dmenu like feature if no suitable menu is found on aminet
- Stop commodity with hotkey
- Use github actions to build and package dintwm release

## Note on licensing

Some parts of the code (tile and fibonacci functions) are adapted from dwm, a DWM-LICENSE is therefore included. 

Ketopt.h from https://github.com/attractivechaos/klib is licensed under the MIT/X11 license.
