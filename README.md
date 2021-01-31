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
or

```
make all
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

1. Add the tooltypes you need to the included dintwm.info file. See Tooltype Options Table section.
2. Test it out using the following command: ```dintwm -C``` or simply run dintwm without options.
3. When done drag the dintwm icon file to wbstartup and dintwm should run after reboot.
4. Check its status in System->Tools->Commodities->Exchange, you can also kill dintwm from here.
5. Dintwm can be exited at any time with Ctrl+Lcommand+q (lcommand is also known as Left Amiga Modifier Key)

The tooltypes in the dintwm file could look like this:  

```
POPKEY_TILE=rawkey control shift z
POPKEY_HGRID=rawkey control shift x
POPKEY_SPIRAL=rawkey control shift c
POPKEY_DWINDLE=rawkey control shift v
TOPGAP=30
EXCL_WTYPE_0=CX V2.3.0: Hot Key = <ctrl alt help>
```

## Tooltype Options Table

| Tooltype | Default | Description | Related Tooltype(s) |
| --- | --- | --- | --- |
| POPKEY\_TILE    | rawkey control lcommand t | tile with master window layout | TILE_FACT |
| POPKEY\_HGRID   | rawkey control lcommand g | grid layout | NA |
| POPKEY\_SPIRAL  | rawkey control lcommand f | fibonacci variant layout | NA |
| POPKEY\_DWINDLE | rawkey control lcommand d | another fibonacci variant layout | NA |
| POPKEY\_TAKESNAPSHOT | rawkey control lcommand p | takes snapshot of current window layout | NA |
| POPKEY\_CLEANSNAPSHOT | rawkey control lcommand c | deletes snapshot of window layout | NA |
| POPKEY\_RESTORE | rawkey control lcommand r | restores snapshot of window layout | NA |
| POPKEY\_SWITCHF | rawkey control lcommand s | flip through layouts forwards | NA |
| POPKEY\_SWITCHB | rawkey control lcommand x | flip through layouts backwards | NA |
| POPKEY\_INCTOPGAP | rawkey control lcommand cursor\_up | increase topgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_INCLEFTGAP | rawkey control lcommand cursor\_left | increase leftgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_INCRIGHTGAP | rawkey control lcommand cursor\_right | increase rightgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_DECTOPGAP | rawkey control shift cursor\_up | decrease topgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_DECBOTTOMGAP | rawkey control shift cursor\_down | decrease bottomgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_DECLEFTGAP | rawkey control shift cursor\_left | decrease leftgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_DECRIGHTGAP | rawkey control shift cursor\_right | decrease rightgap while running | GAP\_CHANGE\_VALUE |
| POPKEY\_INCALLGAPS | rawkey control lcommand numeric\_pad + | increase all gaps while running | GAP\_CHANGE\_VALUE |
| POPKEY\_DECALLGAPS | rawkey control lcommand numeric\_pad -- | (double dash to escape special function of dash) decrease all gaps while running | GAP\_CHANGE\_VALUE |
| POPKEY\_CMD_0..9 | spawns custom commands | rawkey control lcommand 0..9 | CMD_0..9, CONLINE_0..9 |
| POPKEY\_CXM_EXIT | rawkey control lcommand q | exits commodity | NA |
| TOPGAP | 0 | sets the top gap of the screen | NA |
| BOTTOMGAP | 0 | sets a bottom gap | NA |
| LEFTGAP | 0 | sets a left gap | NA |
| RIGHTGAP | 0 | sets a right gap | NA |
| GAP\_CHANGE\_VALUE | 20 | sets the increment/decrement value for gap adjustment | *GAP |
| DEFAULT\_TOPGAP | FALSE | set automatically calculated gap | NA |
| CMD\_0..9 | empty | sets custom commands to spawn, like a shell or a program | CONLINE\_0..9 |
| CONLINE\_0..9 | emtpy | sets custom conline for the matching CMD\_0..9 | CMD\_0..9 |
| EXCL\_WTYPE\_0..9 | empty | excludes windows based on window->Title | deactivates INCL\_WTYPE\_0..9 |
| INCL\_WTYPE\_0..9 | empty | includes windows based on window->Title | deactivates EXCL\_WTYPE\_0..9 |
| AUTO | empty | enable auto tilingr, all non-filtered new windows will be tiled | AUTO\_INTERVAL\_MICRO |
| AUTO\_INTERVAL\_MICRO | 100000 | Auto tile refresh rate, how often it checks for new windoes. Set to 100k microseconds | AUTO |
| TILE\_FACT | 550 | size factor of master window (val / 1000)  in tiling function | POPKEY_TILE |

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

It is of course possible to use something as FKey to bind the above commands to your own HotKeys and thereby using Dintwm as manual tiling utility.

## TODO

- ~~Create an optional Dintwm commodity to enable hotkeys~~
- ~~Make dintwm dynamical, by saving position of windows before tiling.~~
- ~~Float -> Tile -> Float behaviour~~
- ~~Enable auto tiling when new windows appear.~~
- ~~Add leftgap, rightgap and bottomgap~~
- Add dintwm bar
- Create dmenu like feature if no suitable menu is found on aminet
- ~~Stop commodity with hotkey~~
- Use github actions to build and package dintwm release

## Note on licensing

Some parts of the code (tile and fibonacci functions) are adapted from dwm, a DWM-LICENSE is therefore included. 

Ketopt.h from https://github.com/attractivechaos/klib is licensed under the MIT/X11 license.
