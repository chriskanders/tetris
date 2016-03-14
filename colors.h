/* colors.h is only processed once */
#ifndef COLORS_H
#define COLORS_H

/* block types (NA is "empty") */
enum block_type {NA, I, O, T, Z, S, J, L};

/* different colors blocks can be */
enum block_color {BLUE_GREEN, GOLD, PURPLE, RED,
		  LIGHT_GREEN, BLUE, RED_ORANGE};

/* other colors used for things besides blocks */
enum other_color {BLACK, BACKGROUND, GRAY, FOREGROUND, WHITE};

/* stores rgb data */
struct color {
    float r; /* red val */
    float g; /* green val */
    float b; /* blue val */
};

/* function declarations */
internal color type_to_color(block_type);
internal color get_block_color(block_color);
internal color get_other_color(other_color);

/* get color for a specific block type */
internal color
type_to_color(block_type t)
{
   switch (t)
   {
		case NA:
		{
			return get_other_color(BACKGROUND);
		} break;
		case I:
		{
			return get_block_color(BLUE_GREEN);
		} break;
		case O:
		{
			return get_block_color(GOLD);
		} break;
		case T:
		{
			return get_block_color(PURPLE);
		} break;
		case Z:
		{
			return get_block_color(RED);
		} break;
		case S:
		{
			return get_block_color(LIGHT_GREEN);
		} break;
		case J:
		{
			return get_block_color(BLUE);
		} break;
		case L:
		{
			return get_block_color(RED_ORANGE);
		} break;
		default:
		{
			return get_block_color(block_color(-1));
		} break;
   }
}

/* get color struct from a block color */
internal color
get_block_color(block_color c)
{
    color color_v;
    switch (c)
    {
		case BLUE_GREEN:
		{
			color_v.r = float(91)/255;
			color_v.g = float(203)/255;
			color_v.b = float(196)/255; 
		} break;
		case GOLD:
		{
			color_v.r = float(255)/255;
			color_v.g = float(196)/255;
			color_v.b = float(0)/255; 
		} break;
		case PURPLE:
		{
			color_v.r = float(199)/255;
			color_v.g = float(146)/255;
			color_v.b = float(234)/255; 
		} break;
		case RED:
		{
			color_v.r = float(255)/255;
			color_v.g = float(81)/255;
			color_v.b = float(109)/255; 
		} break;
		case LIGHT_GREEN:
		{
			color_v.r = float(194)/255;
			color_v.g = float(233)/255;
			color_v.b = float(130)/255; 
		} break;
		case BLUE:
		{
			color_v.r = float(116)/255;
			color_v.g = float(177)/255;
			color_v.b = float(255)/255; 
		} break;
		case RED_ORANGE:
		{
			color_v.r = float(247)/255;
			color_v.g = float(118)/255;
			color_v.b = float(105)/255; 
		} break;
		default:
		{
			color_v.r = -1.0f;
			color_v.g = -1.0f;
			color_v.b = -1.0f;
		} break;
    }
    return color_v;
}

/* get color struct from *other* color */
internal color 
get_other_color(other_color c)
{
    color color_v;
    switch (c)
    {
		case BLACK:
		{
			color_v.r = float(0)/255;
			color_v.g = float(0)/255;
			color_v.b = float(0)/255; 
		} break;
		case BACKGROUND:
		{
			color_v.r = float(38)/255;
			color_v.g = float(50)/255;
			color_v.b = float(56)/255; 
		} break;
		case GRAY:
		{
			color_v.r = float(84)/255;
			color_v.g = float(109)/255;
			color_v.b = float(122)/255; 
		} break;
		case FOREGROUND:
		{
			color_v.r = float(205)/255;
			color_v.g = float(211)/255;
			color_v.b = float(188)/255; 
		} break;
		case WHITE:
		{
			color_v.r = float(255)/255;
			color_v.g = float(255)/255;
			color_v.b = float(255)/255; 
		} break;
		default:
		{
			color_v.r = -1.0f;
			color_v.g = -1.0f;
			color_v.b = -1.0f;
		} break;
    }
    return color_v;
}
#endif
