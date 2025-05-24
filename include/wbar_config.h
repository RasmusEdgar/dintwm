// Copyright 2024 Rasmus Edgar

static struct TextExtent teinit = {
	.te_Width = 0,
	.te_Height = 0,
	.te_Extent.MinY = 0,
	.te_Extent.MaxY = 0,
	.te_Extent.MinX = 0,
	.te_Extent.MaxX = 0
};

static struct IntuiText wbartext = {
	.TopEdge = 4,
	.LeftEdge = 0,
	.ITextFont = NULL,
	.DrawMode = JAM2,
	.FrontPen = 0,
	.BackPen = 0,
	.IText = NULL,
	.NextText = NULL
};

static struct IntuiText wstext_five;
static struct IntuiText wstext_four;
static struct IntuiText wstext_three;
static struct IntuiText wstext_two;
static struct IntuiText wstext_one;
static struct IntuiText wstext_zero;
static struct IntuiText wbarsep_one;
static struct IntuiText wbarsep_two;
static struct IntuiText wbarwtitle;
static struct IntuiText wbarmodetext;

static struct Border barb = {
	.TopEdge = 0,
	.LeftEdge = 0,
	.FrontPen = 1,
	.BackPen = 0,
	.DrawMode = JAM1,
	.NextBorder = NULL,
	.XY = NULL,
	.Count = 5
};

static unsigned char *padwbartext(Bar_Text * b, enum bar_texts x);
static short int wbartextwidth(int lei, unsigned char *it);
