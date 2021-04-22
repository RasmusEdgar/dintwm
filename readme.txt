dintwm

Tiling window “manager” for AmigaOS 3.1.x.

NOTE Dintwm is under rather heavy development, it is a hobby project and breakage may occur.

Building from source

Install crosscompiler:
https://github.com/bebbo/amiga-gcc

Cross-compilation on Linux for AmigaOS (requires bebbo’s m68k-amigaos-gcc fork)

    make all

Demo

Check demos on: https://github.com/RasmusEdgar/dintwm

Running dintwm as a commodity

Dintwm will ignore the workbench window and backdropped windows.

To run dintwm as a commodity:

1.  Add the tooltypes you need to the included dintwm.info file. See Tooltype Options Table section.
2.  Test it out using the following command: dintwm -C or simply run dintwm without options.
3.  When done drag the dintwm icon file to wbstartup and dintwm should run after reboot.
4.  Check its status in System->Tools->Commodities->Exchange, you can also kill dintwm from here.
5.  Dintwm can be exited at any time with Ctrl+Lcommand+q (lcommand is also known as Left Amiga Modifier Key)

The tooltypes in the dintwm file could look like this:

    POPKEY_TILE=rawkey control shift z
    POPKEY_HGRID=rawkey control shift x
    POPKEY_SPIRAL=rawkey control shift c
    POPKEY_DWINDLE=rawkey control shift v
    TOPGAP=30
    EXCL_WTYPE_0=CX V2.3.0: Hot Key = <ctrl alt help>

Tooltype Options Table

  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Tooltype              Default                                   Description                                                                               Related Tooltype(s)
  --------------------- ----------------------------------------- ----------------------------------------------------------------------------------------- -----------------------------
  POPKEY_TILE           rawkey control lcommand t                 tile with master window layout                                                            TILE_FACT

  POPKEY_HGRID          rawkey control lcommand g                 grid layout                                                                               NA

  POPKEY_SPIRAL         rawkey control lcommand f                 fibonacci variant layout                                                                  NA

  POPKEY_DWINDLE        rawkey control lcommand d                 another fibonacci variant layout                                                          NA

  POPKEY_SWITCHF        rawkey control lcommand s                 flip through layouts forwards                                                             NA

  POPKEY_SWITCHB        rawkey control lcommand x                 flip through layouts backwards                                                            NA

  POPKEY_INCTOPGAP      rawkey control lcommand cursor_up         increase topgap while running                                                             GAP_CHANGE_VALUE

  POPKEY_INCLEFTGAP     rawkey control lcommand cursor_left       increase leftgap while running                                                            GAP_CHANGE_VALUE

  POPKEY_INCRIGHTGAP    rawkey control lcommand cursor_right      increase rightgap while running                                                           GAP_CHANGE_VALUE

  POPKEY_DECTOPGAP      rawkey control shift cursor_up            decrease topgap while running                                                             GAP_CHANGE_VALUE

  POPKEY_DECBOTTOMGAP   rawkey control shift cursor_down          decrease bottomgap while running                                                          GAP_CHANGE_VALUE

  POPKEY_DECLEFTGAP     rawkey control shift cursor_left          decrease leftgap while running                                                            GAP_CHANGE_VALUE

  POPKEY_DECRIGHTGAP    rawkey control shift cursor_right         decrease rightgap while running                                                           GAP_CHANGE_VALUE

  POPKEY_INCALLGAPS     rawkey control lcommand numeric_pad +     increase all gaps while running                                                           GAP_CHANGE_VALUE

  POPKEY_DECALLGAPS     rawkey control lcommand numeric_pad –     (double dash to escape special function of dash) decrease all gaps while running          GAP_CHANGE_VALUE

  POPKEY_CMD_0..9       spawns custom commands                    rawkey control lcommand 0..9                                                              CMD_0..9, CONLINE_0..9

  POPKEY_WS_0..5        change to virtual workspace               rawkey control lcommand numpad 0..5                                                       POPKEY_CWS_0..5

  POPKEY_CWS_0..5       move active window to virtual workspace   rawkey control shift numpad 0..5                                                          POPKEY_CWS_0..5

  POPKEY_CXM_EXIT       rawkey control lcommand q                 exits commodity                                                                           NA

  POPKEY_TILE_OFF       rawkey control lcommand o                 disables tiling globally                                                                  NA

  TOPGAP                0                                         sets the top gap of the screen                                                            NA

  BOTTOMGAP             0                                         sets a bottom gap                                                                         NA

  LEFTGAP               0                                         sets a left gap                                                                           NA

  RIGHTGAP              0                                         sets a right gap                                                                          NA

  GAP_CHANGE_VALUE      20                                        sets the increment/decrement value for gap adjustment                                     *GAP

  DEFAULT_TOPGAP        TRUE                                      set automatically calculated gap                                                          NA

  CMD_0..9              empty                                     sets custom commands to spawn, like a shell or a program                                  CONLINE_0..9

  CONLINE_0..9          emtpy                                     sets custom conline for the matching CMD_0..9                                             CMD_0..9

  EXCL_WTYPE_0..9       empty                                     excludes windows based on window->Title                                                   deactivates INCL_WTYPE_0..9

  INCL_WTYPE_0..9       empty                                     includes windows based on window->Title                                                   deactivates EXCL_WTYPE_0..9

  AUTO                  empty                                     enable auto tiling, all non-filtered new windows will be tiled                            AUTO_INTERVAL_DELAY

  AUTO_INTERVAL_DELAY   1                                         Auto tile refresh rate, how often it checks for new windows. Set to 50 ticks per second   AUTO

  TILE_FACT             550                                       size factor of master window (val / 1000) in tiling function                              POPKEY_TILE

  BAR                   empty                                     Bar on or off                                                                             BAR_*

  BAR_BG_COL            3 (color 3 on palette)                    Dintwm bar bg color                                                                       BAR

  BAR_FPW_COL           1                                         Dintwm bar frontpen color                                                                 BAR

  BAR_BPW_COL           3                                         Dintwm bar backpen color                                                                  BAR

  BAR_FPCURW_COL        139                                       Dintwm bar active workspace color frontpen                                                BAR

  BAR_BPCURW_COL        3                                         Dintwm bar active workspace color backpen                                                 BAR

  BAR_FPTM_COL          1                                         Dintwm bar tilemode frontpen color                                                        BAR

  BAR_BPTM_COL          3                                         Dintwm bar tilemode backpen color                                                         BAR

  BAR_FPTI_COL          1                                         Dintwm bar active window title frontpen color                                             BAR

  BAR_BPTI_COL          3                                         Dintwm bar active window title backpen color                                              BAR

  BAR_FPSEP_ONE_COL     1                                         Dintwm bar seperator one frontpen color                                                   BAR

  BAR_BPSEP_ONE_COL     3                                         Dintwm bar seperator one backpen color                                                    BAR

  BAR_FPSEP_TWO_COL     1                                         Dintwm bar seperator two frontpen color                                                   BAR

  BAR_BPSEP_TWO_COL     3                                         Dintwm bar seperator two backpen color                                                    BAR

  BAR_HIDE_EMPTY        empty                                     Hide bar if workspace is empty                                                            BAR

  INFO_OFF              empty                                     Supress info windows - not recommended                                                    NA

  VWS_ON                FALSE                                     Enable virtual workspaces                                                                 NA
  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Features explained

Here is a short outline of the different features and their limitations.

Virtual workspaces

Creates layers of window sets, which could also be seen as virtual workspaces and change be between using the POPKEY_WS_0..5 hotkeys.

Move active window to another virtual workspace using the POPKEY_CWS_0..5 hotkeys.

Since virtual workspaces depend on moving windows to be the back of the window stack, using it with a backdropped Workbench window will not work.

To enable do not backdrop the workbench window and set tooltype VWS_ON=TRUE

Dintwm bar

The bar is at the moment only available in the bottom of screen. It shows tilemode, active window title and indicates current active workspace.

Default ToolTypes

The following tooltypes are enabled in info file by default:

-   AUTO tiling is enabled
-   VWS_ON i.e. virtual workspaces are enabled
-   BAR is enabled
-   DONOTWAIT is enabled to remove nag dialog box on some systems
-   CMD_0 is set to Newshell by default <ctrl lcommand 0>

Running dintwm from CLI

Again Dintwm will ignore the workbench window, backdropped windows and gimmezerozero windows.

Auto tile for obvious reasons does not work in this mode.

Virtual workspaces do not work either.

Open some windows.

Run to see available options:

    dintwm -h

Try tiling them:

    dintwm -t

With a gap for the workbench titlebar.

    dintwm -g -u

With custom gaps all around:

    dintwm -U 45 -B 45 -L 45 -R 45 -t

That is, you could basically set tiling to occur where ever you decide on the screen.

    dintwm -U 320 -L 320 -t

It is of course possible to use something as FKey to bind the above commands to your own HotKeys and thereby using Dintwm as manual tiling utility.

Options are:

-   -d: Fibonacci dwindle
-   -g: Horizontal grid
-   -t: Tile with left master
-   -s: Fibonacci spiral
-   <other arg> -u: Add workbench top bar gap
-   <other arg> -U<int>: Add custom top gap
-   <other arg> -B<int>: Add custom bottom gap
-   <other arg> -L<int>: Add custom left gap
-   <other arg> -R<int>: Add custom right gap
-   -h: This message

TODO

-   |DONE|Create an optional Dintwm commodity to enable hotkeys|DONE|
-   |DONE|Make dintwm dynamical, by saving position of windows before tiling.|DONE| Dropped
-   |DONE|Float -> Tile -> Float behaviour|DONE|
-   |DONE|Enable auto tiling when new windows appear.|DONE|
-   |DONE|Add leftgap, rightgap and bottomgap|DONE|
-   |DONE|Virtual workspaces|DONE|
-   |DONE|Add dintwm bar|DONE|
-   |DONE|Stop commodity with hotkey|DONE|
-   |DONE|Use github actions to build and package dintwm release|DONE|
-   Implement dedicated dintwm screen
-   Builtin dmenu like launcher
-   Ensure AROS compatibility

Note on licensing

Some parts of the code (tile and fibonacci functions) are adapted from dwm, a DWM-LICENSE is therefore included.

Ketopt.h from https://github.com/attractivechaos/klib is licensed under the MIT/X11 license.