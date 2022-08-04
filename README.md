# dintwm

Tiling window "manager" for AmigaOS 3.1.x.

**NOTE**
Dintwm is under rather heavy development, it is a hobby project and breakage may occur.

## Changelog

* v0.6
  - Fixed bug launching Dintwm from workbench introduced in v0.5
  - Fixed bug when running without BAR TT enabled
  - Add documentation on MagicMenu "Non Blocking" incompatibility

* v0.5
  - Fix autotiling bug that prevented program launches from wb icons
  - Add feature to tab through windows on current workspace
  - Auto select first active window in workspace when changing workspace
  - Auto select first active window when moving a window to other workspace
  - Fix initial barwidth when launching LEFTGAP / RIGHTGAP TT.

* v0.4
  - Make text items in bar configurable
  - Make bar color items behave internally as text items
  - Really fix off by one memleak bug
  - Set better defaults for docmd (conline and newshell)
  - Create hash of window title snips to keep track of them
  - Much better window change tracking
  - Window checking handled in subtask
  - Closing windows always triggers retile
  - Much lower CPU foot print
  - Fix bug with tile mode in bar
  - fbaserel no longer prodcues working binary, dropped

* v0.3 DUD RELEASE BUGS FOUND

* v0.2
  - Add TT VWS_ON to enable virtual workspaces
  - Add INFO_OFF TT to disable info boxes
  - Disable tiling with POPKEY_TILE_OFF
  - Ignore all WS shifting business when vws's are off
  - Fix bug with text not fitting in info windows
  - Quit if backdropped window is detected and vws's are on
  - Don't show workspaces on bar if vws's are off
  - Add version option to cli

* v0.1
  - initial release

## Building from source

Install crosscompiler:  
https://github.com/bebbo/amiga-gcc

### Cross-compilation on Linux for AmigaOS (requires bebbo's m68k-amigaos-gcc fork)

```
make all
```
## Demo

[//]: # (pancut)  

Vanilla without any tooltype tweaks:

![Vanilla](https://github.com/RasmusEdgar/dintwm/blob/master/demo/vanilla.gif)

Force tiling into specific screen area by setting tooltypes:  
TOPGAP=320   
LEFTGAP=320 

![Gap tiling into area](https://github.com/RasmusEdgar/dintwm/blob/master/demo/gap_to_area.gif)

Change between virtual workspaces and resize tiling area dynamically:

![Virtual WSs and dynamic gaps](https://github.com/RasmusEdgar/dintwm/blob/master/demo/virtual_ws_gaps.gif)

Dintwm also has a bar:
![Bar demo](https://github.com/RasmusEdgar/dintwm/blob/master/demo/bar.gif)

[//]: # (panend)  

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
| POPKEY\_WS_0..5 | change to virtual workspace | rawkey control lcommand numpad 0..5 | POPKEY_CWS_0..5 |
| POPKEY\_CWS_0..5 | move active window to virtual workspace | rawkey control shift numpad 0..5 | POPKEY_CWS_0..5 |
| POPKEY\_TAB\_NEXT | rawkey control lcommand tab | tabs through windows on current workspace | NA |
| POPKEY\_CXM\_EXIT | rawkey control lcommand q | exits commodity | NA |
| POPKEY\_TILE\_OFF | rawkey control lcommand o | disables tiling globally | NA |
| TOPGAP | 0 | sets the top gap of the screen | NA |
| BOTTOMGAP | 0 | sets a bottom gap | NA |
| LEFTGAP | 0 | sets a left gap | NA |
| RIGHTGAP | 0 | sets a right gap | NA |
| GAP\_CHANGE\_VALUE | 20 | sets the increment/decrement value for gap adjustment | *GAP |
| DEFAULT\_TOPGAP | TRUE | set automatically calculated gap | NA |
| CMD\_0..9 | NewShell | sets custom commands to spawn, like a shell or a program | CONLINE\_0..9 |
| CONLINE\_0..9 | CON:0/0/550/350/Dintwm Shell/AUTO/CLOSE/WAIT | sets custom conline for the matching CMD\_0..9 | CMD\_0..9 |
| EXCL\_WTYPE\_0..9 | empty | excludes windows based on window->Title | deactivates INCL\_WTYPE\_0..9 |
| INCL\_WTYPE\_0..9 | empty | includes windows based on window->Title | deactivates EXCL\_WTYPE\_0..9 |
| AUTO | empty | enable auto tiling, all non-filtered new windows will be tiled | AUTO\_INTERVAL\_DELAY |
| AUTO\_INTERVAL\_DELAY | 1 | Auto tile refresh rate, how often it checks for new windows. Set to 50 ticks per second | AUTO |
| TILE\_FACT | 550 | size factor of master window (val / 1000)  in tiling function | POPKEY_TILE |
| BAR | empty | Bar on or off | BAR_* |
| BAR\_BG\_COL | 3 (color 3 on palette) |  Dintwm bar bg color | BAR |
| BAR\_FPW\_COL | 1 | Dintwm bar frontpen color | BAR |
| BAR\_BPW\_COL | 3 | Dintwm bar backpen color | BAR |
| BAR\_FPCURW\_COL | 6 | Dintwm bar active workspace color frontpen | BAR |
| BAR\_BPCURW\_COL | 3 | Dintwm bar active workspace color backpen | BAR | 
| BAR\_FPTM\_COL | 1 | Dintwm bar tilemode frontpen color | BAR |
| BAR\_BPTM\_COL | 3 | Dintwm bar tilemode backpen color | BAR |
| BAR\_FPTI\_COL | 1 | Dintwm bar active window title frontpen color | BAR |
| BAR\_BPTI\_COL | 3 | Dintwm bar active window title backpen color | BAR |
| BAR\_FPSEP\_ONE\_COL | 1 | Dintwm bar seperator one frontpen color | BAR |
| BAR\_BPSEP\_ONE\_COL | 3 | Dintwm bar seperator one backpen color | BAR |
| BAR\_FPSEP\_TWO\_COL | 1 | Dintwm bar seperator two frontpen color | BAR |
| BAR\_BPSEP\_TWO\_COL | 3 | Dintwm bar seperator two backpen color | BAR |
| BAR\_TEXT\_WS0 | WS0 | Dintwm bar text for ws0 | BAR |
| BAR\_TEXT\_WS1 | WS1 | Dintwm bar text for ws1 | BAR |
| BAR\_TEXT\_WS2 | WS2 | Dintwm bar text for ws2 | BAR |
| BAR\_TEXT\_WS3 | WS3 | Dintwm bar text for ws3 | BAR |
| BAR\_TEXT\_WS4 | WS4 | Dintwm bar text for ws4 | BAR |
| BAR\_TEXT\_WS5 | WS5 | Dintwm bar text for ws5 | BAR |
| BAR\_TEXT\_TILE | Tile | Dintwm bar text for tile mode | BAR |
| BAR\_TEXT\_GRID | Grid | Dintwm bar text for grid mode | BAR |
| BAR\_TEXT\_DWINDLE | FibD | Dintwm bar text for dwindle mode | BAR |
| BAR\_TEXT\_SPIRAL | FibS | Dintwm bar text for spiral mode | BAR |
| BAR\_TEXT\_SEP\_1 | \| | Dintwm bar text for sepator one | BAR |
| BAR\_TEXT\_SEP\_2 | \| | Dintwm bar text for sepator one | BAR |
| BAR\_HIDE\_EMPTY | empty | Hide bar if workspace is empty | BAR |
| INFO\_OFF | empty | Supress info windows - not recommended | NA |
| VWS\_ON | FALSE | Enable virtual workspaces | NA |

## Features explained

Here is a short outline of the different features and their limitations.

### Virtual workspaces

Creates layers of window sets, which could also be seen as virtual workspaces and change between them using the POPKEY\_WS\_0..5 hotkeys.

Move active window to another virtual workspace using the POPKEY\_CWS\_0..5 hotkeys.

Since virtual workspaces depend on moving windows to be the back of the window stack, using it with a backdropped Workbench window will not work.

**To enable _do not_ backdrop the workbench window and set tooltype VWS\_ON=TRUE**

### Dintwm bar

The bar is at the moment only available in the bottom of the tiling area. It shows tilemode, active window title and indicates the current active workspace. 

### Default ToolTypes

The following tooltypes are enabled in info file by default:

* AUTO tiling is enabled
* VWS\_ON i.e. virtual workspaces are enabled
* BAR is enabled
* DONOTWAIT is enabled to remove nag dialog box on some systems
* CMD\_0 is set to Newshell by default <ctrl lcommand 0>

## Known problems and workarounds:

### MagicMenu

MagicMenu with "Non Blocking" enabled in combination with DintWM will dead lock/hang workbench. To use DintWM with MagicMenu, please disable "Non Blocking" in MagicMenu prefs.

## Running dintwm from CLI

Again Dintwm will ignore the workbench window, backdropped windows and gimmezerozero windows.

Auto tile for obvious reasons does not work in this mode.

Virtual workspaces do not work either.

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

It is of course possible to use something as FKey to bind the above commands to your own HotKeys and use Dintwm as a manual tiling utility.

Options are:

* `-d`: Fibonacci dwindle
* `-g`: Horizontal grid
* `-t`: Tile with left master
* `-s`: Fibonacci spiral
* `<other arg> -u`: Add workbench top bar gap
* `<other arg> -U<int>`: Add custom top gap
* `<other arg> -B<int>`: Add custom bottom gap
* `<other arg> -L<int>`: Add custom left gap
* `<other arg> -R<int>`: Add custom right gap
* `-h`: This message

## TODO

- ~~Create an optional Dintwm commodity to enable hotkeys~~
- ~~Make dintwm dynamical, by saving position of windows before tiling.~~ *Dropped*
- ~~Float -> Tile -> Float behaviour~~
- ~~Enable auto tiling when new windows appear.~~
- ~~Add leftgap, rightgap and bottomgap~~
- ~~Virtual workspaces~~
- ~~Add dintwm bar~~
- ~~Stop commodity with hotkey~~
- ~~Use github actions to build and package dintwm release~~
- ~~Make static bar text fields configurable~~
- ~~Implement dedicated dintwm screen~~ *Dropped*
- ~~Builtin dmenu like launcher~~ * See http://aminet.net/package/util/wb/tabexec *
- ~~Ensure AROS compatibility (works without bar atm)~~ *Dropped ftb*

## Note on licensing

Some parts of the code (tile and fibonacci functions) are adapted from dwm, a DWM-LICENSE is therefore included. 

Ketopt.h from https://github.com/attractivechaos/klib is licensed under the MIT/X11 license.
