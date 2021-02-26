// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
unsigned char	ws_zero[TT_MAX_LENGTH] = "W0",
		ws_one[TT_MAX_LENGTH] = "W1",
		ws_two[TT_MAX_LENGTH] = "W2",
		ws_three[TT_MAX_LENGTH] = "W3",
		ws_four[TT_MAX_LENGTH] = "W4",
		ws_five[TT_MAX_LENGTH] = "W5",
		wbarspace[TT_MAX_LENGTH] = " ",
		mode_tile[TT_MAX_LENGTH] = "Tile",
		mode_grid[TT_MAX_LENGTH] = "Grid",
		mode_dwindle[TT_MAX_LENGTH] = "FibD",
		mode_spiral[TT_MAX_LENGTH] = "FibS",
		wbar_sep_one[TT_MAX_LENGTH] = ":",
		wbar_sep_two[TT_MAX_LENGTH] = ":",
		wbar_err[TT_MAX_LENGTH] = " Fail ";

struct TextExtent teinit = {
	.te_Width = 0,
	.te_Height = 0,
	.te_Extent.MinY = 0,
	.te_Extent.MaxY = 0,
	.te_Extent.MinX = 0,
	.te_Extent.MaxX = 0
};

struct IntuiText wbartext =
{
	.TopEdge = 4,
	.LeftEdge = 0,
	.ITextFont = NULL,
	.DrawMode = JAM2, //-V2568
	.FrontPen = 0, //-V2568
	.BackPen = 0, //-V2568
	.IText = NULL,
	.NextText = NULL
};

struct IntuiText wstext_five;
struct IntuiText wstext_four;
struct IntuiText wstext_three;
struct IntuiText wstext_two;
struct IntuiText wstext_one;
struct IntuiText wstext_zero;
struct IntuiText wbarsep_one;
struct IntuiText wbarsep_two;
struct IntuiText wbarwtitle;
struct IntuiText wbarmodetext;

short barbdata[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct Border barb =
{
	.TopEdge = 0,
	.LeftEdge = 0,
	.FrontPen = 1, //-V2568
	.BackPen = 0, //-V2568
	.DrawMode = JAM1, //-V2568
	.NextBorder = NULL,
	.XY = NULL,
	.Count = 5
};

unsigned char awintitle[TT_MAX_LENGTH];
