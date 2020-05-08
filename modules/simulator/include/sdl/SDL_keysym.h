/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifndef _SDL_keysym_h
#define _SDL_keysym_h

#include "LADIS_keys.h"

/** What we really want is a mapping of every raw key on the keyboard.
 *  To support international keyboards, we use the range 0xA1 - 0xFF
 *  as international virtual keycodes.  We'll follow in the footsteps of X11...
 *  @brief The names of the keys
 */
/*
typedef enum {
	SDLK_UNKNOWN		= 0,
	SDLK_FIRST		= 0,
	SDLK_BACKSPACE		= 8,
	SDLK_TAB		= 9,
	SDLK_CLEAR		= 12,
	SDLK_RETURN		= 13,
	SDLK_PAUSE		= 19,
	SDLK_ESCAPE		= 27,
	SDLK_SPACE		= 32,
	SDLK_EXCLAIM		= 33,
	SDLK_QUOTEDBL		= 34,
	SDLK_HASH		= 35,
	SDLK_DOLLAR		= 36,
	SDLK_AMPERSAND		= 38,
	SDLK_QUOTE		= 39,
	SDLK_LEFTPAREN		= 40,
	SDLK_RIGHTPAREN		= 41,
	SDLK_ASTERISK		= 42,
	SDLK_PLUS		= 43,
	SDLK_COMMA		= 44,
	SDLK_MINUS		= 45,
	SDLK_PERIOD		= 46,
	SDLK_SLASH		= 47,
	SDLK_0			= 48,
	SDLK_1			= 49,
	SDLK_2			= 50,
	SDLK_3			= 51,
	SDLK_4			= 52,
	SDLK_5			= 53,
	SDLK_6			= 54,
	SDLK_7			= 55,
	SDLK_8			= 56,
	SDLK_9			= 57,
	SDLK_COLON		= 58,
	SDLK_SEMICOLON		= 59,
	SDLK_LESS		= 60,
	SDLK_EQUALS		= 61,
	SDLK_GREATER		= 62,
	SDLK_QUESTION		= 63,
	SDLK_AT			= 64,
	SDLK_LEFTBRACKET	= 91,
	SDLK_BACKSLASH		= 92,
	SDLK_RIGHTBRACKET	= 93,
	SDLK_CARET		= 94,
	SDLK_UNDERSCORE		= 95,
	SDLK_BACKQUOTE		= 96,
	SDLK_a			= 97,
	SDLK_b			= 98,
	SDLK_c			= 99,
	SDLK_d			= 100,
	SDLK_e			= 101,
	SDLK_f			= 102,
	SDLK_g			= 103,
	SDLK_h			= 104,
	SDLK_i			= 105,
	SDLK_j			= 106,
	SDLK_k			= 107,
	SDLK_l			= 108,
	SDLK_m			= 109,
	SDLK_n			= 110,
	SDLK_o			= 111,
	SDLK_p			= 112,
	SDLK_q			= 113,
	SDLK_r			= 114,
	SDLK_s			= 115,
	SDLK_t			= 116,
	SDLK_u			= 117,
	SDLK_v			= 118,
	SDLK_w			= 119,
	SDLK_x			= 120,
	SDLK_y			= 121,
	SDLK_z			= 122,
	SDLK_DELETE		= 127,
	SDLK_WORLD_0		= 160,
	SDLK_WORLD_1		= 161,
	SDLK_WORLD_2		= 162,
	SDLK_WORLD_3		= 163,
	SDLK_WORLD_4		= 164,
	SDLK_WORLD_5		= 165,
	SDLK_WORLD_6		= 166,
	SDLK_WORLD_7		= 167,
	SDLK_WORLD_8		= 168,
	SDLK_WORLD_9		= 169,
	SDLK_WORLD_10		= 170,
	SDLK_WORLD_11		= 171,
	SDLK_WORLD_12		= 172,
	SDLK_WORLD_13		= 173,
	SDLK_WORLD_14		= 174,
	SDLK_WORLD_15		= 175,
	SDLK_WORLD_16		= 176,
	SDLK_WORLD_17		= 177,
	SDLK_WORLD_18		= 178,
	SDLK_WORLD_19		= 179,
	SDLK_WORLD_20		= 180,
	SDLK_WORLD_21		= 181,
	SDLK_WORLD_22		= 182,
	SDLK_WORLD_23		= 183,
	SDLK_WORLD_24		= 184,
	SDLK_WORLD_25		= 185,
	SDLK_WORLD_26		= 186,
	SDLK_WORLD_27		= 187,
	SDLK_WORLD_28		= 188,
	SDLK_WORLD_29		= 189,
	SDLK_WORLD_30		= 190,
	SDLK_WORLD_31		= 191,
	SDLK_WORLD_32		= 192,
	SDLK_WORLD_33		= 193,
	SDLK_WORLD_34		= 194,
	SDLK_WORLD_35		= 195,
	SDLK_WORLD_36		= 196,
	SDLK_WORLD_37		= 197,
	SDLK_WORLD_38		= 198,
	SDLK_WORLD_39		= 199,
	SDLK_WORLD_40		= 200,
	SDLK_WORLD_41		= 201,
	SDLK_WORLD_42		= 202,
	SDLK_WORLD_43		= 203,
	SDLK_WORLD_44		= 204,
	SDLK_WORLD_45		= 205,
	SDLK_WORLD_46		= 206,
	SDLK_WORLD_47		= 207,
	SDLK_WORLD_48		= 208,
	SDLK_WORLD_49		= 209,
	SDLK_WORLD_50		= 210,
	SDLK_WORLD_51		= 211,
	SDLK_WORLD_52		= 212,
	SDLK_WORLD_53		= 213,
	SDLK_WORLD_54		= 214,
	SDLK_WORLD_55		= 215,
	SDLK_WORLD_56		= 216,
	SDLK_WORLD_57		= 217,
	SDLK_WORLD_58		= 218,
	SDLK_WORLD_59		= 219,
	SDLK_WORLD_60		= 220,
	SDLK_WORLD_61		= 221,
	SDLK_WORLD_62		= 222,
	SDLK_WORLD_63		= 223,
	SDLK_WORLD_64		= 224,
	SDLK_WORLD_65		= 225,
	SDLK_WORLD_66		= 226,
	SDLK_WORLD_67		= 227,
	SDLK_WORLD_68		= 228,
	SDLK_WORLD_69		= 229,
	SDLK_WORLD_70		= 230,
	SDLK_WORLD_71		= 231,
	SDLK_WORLD_72		= 232,
	SDLK_WORLD_73		= 233,
	SDLK_WORLD_74		= 234,
	SDLK_WORLD_75		= 235,
	SDLK_WORLD_76		= 236,
	SDLK_WORLD_77		= 237,
	SDLK_WORLD_78		= 238,
	SDLK_WORLD_79		= 239,
	SDLK_WORLD_80		= 240,
	SDLK_WORLD_81		= 241,
	SDLK_WORLD_82		= 242,
	SDLK_WORLD_83		= 243,
	SDLK_WORLD_84		= 244,
	SDLK_WORLD_85		= 245,
	SDLK_WORLD_86		= 246,
	SDLK_WORLD_87		= 247,
	SDLK_WORLD_88		= 248,
	SDLK_WORLD_89		= 249,
	SDLK_WORLD_90		= 250,
	SDLK_WORLD_91		= 251,
	SDLK_WORLD_92		= 252,
	SDLK_WORLD_93		= 253,
	SDLK_WORLD_94		= 254,
	SDLK_WORLD_95		= 255,
	SDLK_KP0		= 256,
	SDLK_KP1		= 257,
	SDLK_KP2		= 258,
	SDLK_KP3		= 259,
	SDLK_KP4		= 260,
	SDLK_KP5		= 261,
	SDLK_KP6		= 262,
	SDLK_KP7		= 263,
	SDLK_KP8		= 264,
	SDLK_KP9		= 265,
	SDLK_KP_PERIOD		= 266,
	SDLK_KP_DIVIDE		= 267,
	SDLK_KP_MULTIPLY	= 268,
	SDLK_KP_MINUS		= 269,
	SDLK_KP_PLUS		= 270,
	SDLK_KP_ENTER		= 271,
	SDLK_KP_EQUALS		= 272,
	SDLK_UP			= 273,
	SDLK_DOWN		= 274,
	SDLK_RIGHT		= 275,
	SDLK_LEFT		= 276,
	SDLK_INSERT		= 277,
	SDLK_HOME		= 278,
	SDLK_END		= 279,
	SDLK_PAGEUP		= 280,
	SDLK_PAGEDOWN		= 281,
	SDLK_F1			= 282,
	SDLK_F2			= 283,
	SDLK_F3			= 284,
	SDLK_F4			= 285,
	SDLK_F5			= 286,
	SDLK_F6			= 287,
	SDLK_F7			= 288,
	SDLK_F8			= 289,
	SDLK_F9			= 290,
	SDLK_F10		= 291,
	SDLK_F11		= 292,
	SDLK_F12		= 293,
	SDLK_F13		= 294,
	SDLK_F14		= 295,
	SDLK_F15		= 296,
	SDLK_NUMLOCK		= 300,
	SDLK_CAPSLOCK		= 301,
	SDLK_SCROLLOCK		= 302,
	SDLK_RSHIFT		= 303,
	SDLK_LSHIFT		= 304,
	SDLK_RCTRL		= 305,
	SDLK_LCTRL		= 306,
	SDLK_RALT		= 307,
	SDLK_LALT		= 308,
	SDLK_RMETA		= 309,
	SDLK_LMETA		= 310,
	SDLK_LSUPER		= 311,
	SDLK_RSUPER		= 312,
	SDLK_MODE		= 313,
	SDLK_COMPOSE		= 314,
	SDLK_HELP		= 315,
	SDLK_PRINT		= 316,
	SDLK_SYSREQ		= 317,
	SDLK_BREAK		= 318,
	SDLK_MENU		= 319,
	SDLK_POWER		= 320,
	SDLK_EURO		= 321,
	SDLK_UNDO		= 322,
	SDLK_LAST
} SDLKey;
*/

typedef enum {
	SDLK_UNKNOWN = LADIS_KEY_UNKNOWN,
	SDLK_FIRST = LADIS_KEY_FIRST,
	SDLK_BACKSPACE = LADIS_KEY_BACKSPACE,
	SDLK_TAB = LADIS_KEY_TAB,
	SDLK_CLEAR = LADIS_KEY_CLEAR,
	SDLK_RETURN = LADIS_KEY_RETURN,
	SDLK_PAUSE = LADIS_KEY_PAUSE,
	SDLK_ESCAPE = LADIS_KEY_ESCAPE,
	SDLK_SPACE = LADIS_KEY_SPACE,
	SDLK_EXCLAIM = LADIS_KEY_EXCLAIM,
	SDLK_QUOTEDBL = LADIS_KEY_QUOTEDBL,
	SDLK_HASH = LADIS_KEY_HASH,
	SDLK_DOLLAR = LADIS_KEY_DOLLAR,
	SDLK_AMPERSAND = LADIS_KEY_AMPERSAND,
	SDLK_QUOTE = LADIS_KEY_QUOTE,
	SDLK_LEFTPAREN = LADIS_KEY_LEFTPAREN,
	SDLK_RIGHTPAREN = LADIS_KEY_RIGHTPAREN,
	SDLK_ASTERISK = LADIS_KEY_ASTERISK,
	SDLK_PLUS = LADIS_KEY_PLUS,
	SDLK_COMMA = LADIS_KEY_COMMA,
	SDLK_MINUS = LADIS_KEY_MINUS,
	SDLK_PERIOD = LADIS_KEY_PERIOD,
	SDLK_SLASH = LADIS_KEY_SLASH,
	SDLK_0 = LADIS_KEY_0,
	SDLK_1 = LADIS_KEY_1,
	SDLK_2 = LADIS_KEY_2,
	SDLK_3 = LADIS_KEY_3,
	SDLK_4 = LADIS_KEY_4,
	SDLK_5 = LADIS_KEY_5,
	SDLK_6 = LADIS_KEY_6,
	SDLK_7 = LADIS_KEY_7,
	SDLK_8 = LADIS_KEY_8,
	SDLK_9 = LADIS_KEY_9,
	SDLK_COLON = LADIS_KEY_COLON,
	SDLK_SEMICOLON = LADIS_KEY_SEMICOLON,
	SDLK_LESS = LADIS_KEY_LESS,
	SDLK_EQUALS = LADIS_KEY_EQUALS,
	SDLK_GREATER = LADIS_KEY_GREATER,
	SDLK_QUESTION = LADIS_KEY_QUESTION,
	SDLK_AT = LADIS_KEY_AT,
	SDLK_LEFTBRACKET = LADIS_KEY_LEFTBRACKET,
	SDLK_BACKSLASH = LADIS_KEY_BACKSLASH,
	SDLK_RIGHTBRACKET = LADIS_KEY_RIGHTBRACKET,
	SDLK_CARET = LADIS_KEY_CARET,
	SDLK_UNDERSCORE = LADIS_KEY_UNDERSCORE,
	SDLK_BACKQUOTE = LADIS_KEY_BACKQUOTE,
	SDLK_a = LADIS_KEY_a,
	SDLK_b = LADIS_KEY_b,
	SDLK_c = LADIS_KEY_c,
	SDLK_d = LADIS_KEY_d,
	SDLK_e = LADIS_KEY_e,
	SDLK_f = LADIS_KEY_f,
	SDLK_g = LADIS_KEY_g,
	SDLK_h = LADIS_KEY_h,
	SDLK_i = LADIS_KEY_i,
	SDLK_j = LADIS_KEY_j,
	SDLK_k = LADIS_KEY_k,
	SDLK_l = LADIS_KEY_l,
	SDLK_m = LADIS_KEY_m,
	SDLK_n = LADIS_KEY_n,
	SDLK_o = LADIS_KEY_o,
	SDLK_p = LADIS_KEY_p,
	SDLK_q = LADIS_KEY_q,
	SDLK_r = LADIS_KEY_r,
	SDLK_s = LADIS_KEY_s,
	SDLK_t = LADIS_KEY_t,
	SDLK_u = LADIS_KEY_u,
	SDLK_v = LADIS_KEY_v,
	SDLK_w = LADIS_KEY_w,
	SDLK_x = LADIS_KEY_x,
	SDLK_y = LADIS_KEY_y,
	SDLK_z = LADIS_KEY_z,
	SDLK_DELETE = LADIS_KEY_DELETE,
	SDLK_WORLD_0 = LADIS_KEY_WORLD_0,
	SDLK_WORLD_1 = LADIS_KEY_WORLD_1,
	SDLK_WORLD_2 = LADIS_KEY_WORLD_2,
	SDLK_WORLD_3 = LADIS_KEY_WORLD_3,
	SDLK_WORLD_4 = LADIS_KEY_WORLD_4,
	SDLK_WORLD_5 = LADIS_KEY_WORLD_5,
	SDLK_WORLD_6 = LADIS_KEY_WORLD_6,
	SDLK_WORLD_7 = LADIS_KEY_WORLD_7,
	SDLK_WORLD_8 = LADIS_KEY_WORLD_8,
	SDLK_WORLD_9 = LADIS_KEY_WORLD_9,
	SDLK_WORLD_10 = LADIS_KEY_WORLD_10,
	SDLK_WORLD_11 = LADIS_KEY_WORLD_11,
	SDLK_WORLD_12 = LADIS_KEY_WORLD_12,
	SDLK_WORLD_13 = LADIS_KEY_WORLD_13,
	SDLK_WORLD_14 = LADIS_KEY_WORLD_14,
	SDLK_WORLD_15 = LADIS_KEY_WORLD_15,
	SDLK_WORLD_16 = LADIS_KEY_WORLD_16,
	SDLK_WORLD_17 = LADIS_KEY_WORLD_17,
	SDLK_WORLD_18 = LADIS_KEY_WORLD_18,
	SDLK_WORLD_19 = LADIS_KEY_WORLD_19,
	SDLK_WORLD_20 = LADIS_KEY_WORLD_20,
	SDLK_WORLD_21 = LADIS_KEY_WORLD_21,
	SDLK_WORLD_22 = LADIS_KEY_WORLD_22,
	SDLK_WORLD_23 = LADIS_KEY_WORLD_23,
	SDLK_WORLD_24 = LADIS_KEY_WORLD_24,
	SDLK_WORLD_25 = LADIS_KEY_WORLD_25,
	SDLK_WORLD_26 = LADIS_KEY_WORLD_26,
	SDLK_WORLD_27 = LADIS_KEY_WORLD_27,
	SDLK_WORLD_28 = LADIS_KEY_WORLD_28,
	SDLK_WORLD_29 = LADIS_KEY_WORLD_29,
	SDLK_WORLD_30 = LADIS_KEY_WORLD_30,
	SDLK_WORLD_31 = LADIS_KEY_WORLD_31,
	SDLK_WORLD_32 = LADIS_KEY_WORLD_32,
	SDLK_WORLD_33 = LADIS_KEY_WORLD_33,
	SDLK_WORLD_34 = LADIS_KEY_WORLD_34,
	SDLK_WORLD_35 = LADIS_KEY_WORLD_35,
	SDLK_WORLD_36 = LADIS_KEY_WORLD_36,
	SDLK_WORLD_37 = LADIS_KEY_WORLD_37,
	SDLK_WORLD_38 = LADIS_KEY_WORLD_38,
	SDLK_WORLD_39 = LADIS_KEY_WORLD_39,
	SDLK_WORLD_40 = LADIS_KEY_WORLD_40,
	SDLK_WORLD_41 = LADIS_KEY_WORLD_41,
	SDLK_WORLD_42 = LADIS_KEY_WORLD_42,
	SDLK_WORLD_43 = LADIS_KEY_WORLD_43,
	SDLK_WORLD_44 = LADIS_KEY_WORLD_44,
	SDLK_WORLD_45 = LADIS_KEY_WORLD_45,
	SDLK_WORLD_46 = LADIS_KEY_WORLD_46,
	SDLK_WORLD_47 = LADIS_KEY_WORLD_47,
	SDLK_WORLD_48 = LADIS_KEY_WORLD_48,
	SDLK_WORLD_49 = LADIS_KEY_WORLD_49,
	SDLK_WORLD_50 = LADIS_KEY_WORLD_50,
	SDLK_WORLD_51 = LADIS_KEY_WORLD_51,
	SDLK_WORLD_52 = LADIS_KEY_WORLD_52,
	SDLK_WORLD_53 = LADIS_KEY_WORLD_53,
	SDLK_WORLD_54 = LADIS_KEY_WORLD_54,
	SDLK_WORLD_55 = LADIS_KEY_WORLD_55,
	SDLK_WORLD_56 = LADIS_KEY_WORLD_56,
	SDLK_WORLD_57 = LADIS_KEY_WORLD_57,
	SDLK_WORLD_58 = LADIS_KEY_WORLD_58,
	SDLK_WORLD_59 = LADIS_KEY_WORLD_59,
	SDLK_WORLD_60 = LADIS_KEY_WORLD_60,
	SDLK_WORLD_61 = LADIS_KEY_WORLD_61,
	SDLK_WORLD_62 = LADIS_KEY_WORLD_62,
	SDLK_WORLD_63 = LADIS_KEY_WORLD_63,
	SDLK_WORLD_64 = LADIS_KEY_WORLD_64,
	SDLK_WORLD_65 = LADIS_KEY_WORLD_65,
	SDLK_WORLD_66 = LADIS_KEY_WORLD_66,
	SDLK_WORLD_67 = LADIS_KEY_WORLD_67,
	SDLK_WORLD_68 = LADIS_KEY_WORLD_68,
	SDLK_WORLD_69 = LADIS_KEY_WORLD_69,
	SDLK_WORLD_70 = LADIS_KEY_WORLD_70,
	SDLK_WORLD_71 = LADIS_KEY_WORLD_71,
	SDLK_WORLD_72 = LADIS_KEY_WORLD_72,
	SDLK_WORLD_73 = LADIS_KEY_WORLD_73,
	SDLK_WORLD_74 = LADIS_KEY_WORLD_74,
	SDLK_WORLD_75 = LADIS_KEY_WORLD_75,
	SDLK_WORLD_76 = LADIS_KEY_WORLD_76,
	SDLK_WORLD_77 = LADIS_KEY_WORLD_77,
	SDLK_WORLD_78 = LADIS_KEY_WORLD_78,
	SDLK_WORLD_79 = LADIS_KEY_WORLD_79,
	SDLK_WORLD_80 = LADIS_KEY_WORLD_80,
	SDLK_WORLD_81 = LADIS_KEY_WORLD_81,
	SDLK_WORLD_82 = LADIS_KEY_WORLD_82,
	SDLK_WORLD_83 = LADIS_KEY_WORLD_83,
	SDLK_WORLD_84 = LADIS_KEY_WORLD_84,
	SDLK_WORLD_85 = LADIS_KEY_WORLD_85,
	SDLK_WORLD_86 = LADIS_KEY_WORLD_86,
	SDLK_WORLD_87 = LADIS_KEY_WORLD_87,
	SDLK_WORLD_88 = LADIS_KEY_WORLD_88,
	SDLK_WORLD_89 = LADIS_KEY_WORLD_89,
	SDLK_WORLD_90 = LADIS_KEY_WORLD_90,
	SDLK_WORLD_91 = LADIS_KEY_WORLD_91,
	SDLK_WORLD_92 = LADIS_KEY_WORLD_92,
	SDLK_WORLD_93 = LADIS_KEY_WORLD_93,
	SDLK_WORLD_94 = LADIS_KEY_WORLD_94,
	SDLK_WORLD_95 = LADIS_KEY_WORLD_95,
	SDLK_KP0 = LADIS_KEY_KP0,
	SDLK_KP1 = LADIS_KEY_KP1,
	SDLK_KP2 = LADIS_KEY_KP2,
	SDLK_KP3 = LADIS_KEY_KP3,
	SDLK_KP4 = LADIS_KEY_KP4,
	SDLK_KP5 = LADIS_KEY_KP5,
	SDLK_KP6 = LADIS_KEY_KP6,
	SDLK_KP7 = LADIS_KEY_KP7,
	SDLK_KP8 = LADIS_KEY_KP8,
	SDLK_KP9 = LADIS_KEY_KP9,
	SDLK_KP_PERIOD = LADIS_KEY_KP_PERIOD,
	SDLK_KP_DIVIDE = LADIS_KEY_KP_DIVIDE,
	SDLK_KP_MULTIPLY = LADIS_KEY_KP_MULTIPLY,
	SDLK_KP_MINUS = LADIS_KEY_KP_MINUS,
	SDLK_KP_PLUS = LADIS_KEY_KP_PLUS,
	SDLK_KP_ENTER = LADIS_KEY_KP_ENTER,
	SDLK_KP_EQUALS = LADIS_KEY_KP_EQUALS,
	SDLK_UP = LADIS_KEY_UP,
	SDLK_DOWN = LADIS_KEY_DOWN,
	SDLK_RIGHT = LADIS_KEY_RIGHT,
	SDLK_LEFT = LADIS_KEY_LEFT,
	SDLK_INSERT = LADIS_KEY_INSERT,
	SDLK_HOME = LADIS_KEY_HOME,
	SDLK_END = LADIS_KEY_END,
	SDLK_PAGEUP = LADIS_KEY_PAGEUP,
	SDLK_PAGEDOWN = LADIS_KEY_PAGEDOWN,
	SDLK_F1 = LADIS_KEY_F1,
	SDLK_F2 = LADIS_KEY_F2,
	SDLK_F3 = LADIS_KEY_F3,
	SDLK_F4 = LADIS_KEY_F4,
	SDLK_F5 = LADIS_KEY_F5,
	SDLK_F6 = LADIS_KEY_F6,
	SDLK_F7 = LADIS_KEY_F7,
	SDLK_F8 = LADIS_KEY_F8,
	SDLK_F9 = LADIS_KEY_F9,
	SDLK_F10 = LADIS_KEY_F10,
	SDLK_F11 = LADIS_KEY_F11,
	SDLK_F12 = LADIS_KEY_F12,
	SDLK_F13 = LADIS_KEY_F13,
	SDLK_F14 = LADIS_KEY_F14,
	SDLK_F15 = LADIS_KEY_F15,
	SDLK_NUMLOCK = LADIS_KEY_NUMLOCK,
	SDLK_CAPSLOCK = LADIS_KEY_CAPSLOCK,
	SDLK_SCROLLOCK = LADIS_KEY_SCROLLOCK,
	SDLK_RSHIFT = LADIS_KEY_RSHIFT,
	SDLK_LSHIFT = LADIS_KEY_LSHIFT,
	SDLK_RCTRL = LADIS_KEY_RCTRL,
	SDLK_LCTRL = LADIS_KEY_LCTRL,
	SDLK_RALT = LADIS_KEY_RALT,
	SDLK_LALT = LADIS_KEY_LALT,
	SDLK_RMETA = LADIS_KEY_RMETA,
	SDLK_LMETA = LADIS_KEY_LMETA,
	SDLK_LSUPER = LADIS_KEY_LSUPER,
	SDLK_RSUPER = LADIS_KEY_RSUPER,
	SDLK_MODE = LADIS_KEY_MODE,
	SDLK_COMPOSE = LADIS_KEY_COMPOSE,
	SDLK_HELP = LADIS_KEY_HELP,
	SDLK_PRINT = LADIS_KEY_PRINT,
	SDLK_SYSREQ = LADIS_KEY_SYSREQ,
	SDLK_BREAK = LADIS_KEY_BREAK,
	SDLK_MENU = LADIS_KEY_MENU,
	SDLK_POWER = LADIS_KEY_POWER,
	SDLK_EURO = LADIS_KEY_EURO,
	SDLK_UNDO = LADIS_KEY_UNDO,
	SDLK_LAST = LADIS_KEY_LAST,
} SDLKey;

/** Enumeration of valid key mods (possibly OR'd together) */
typedef enum {
	KMOD_NONE  = 0x0000,
	KMOD_LSHIFT= 0x0001,
	KMOD_RSHIFT= 0x0002,
	KMOD_LCTRL = 0x0040,
	KMOD_RCTRL = 0x0080,
	KMOD_LALT  = 0x0100,
	KMOD_RALT  = 0x0200,
	KMOD_LMETA = 0x0400,
	KMOD_RMETA = 0x0800,
	KMOD_NUM   = 0x1000,
	KMOD_CAPS  = 0x2000,
	KMOD_MODE  = 0x4000,
	KMOD_RESERVED = 0x8000
} SDLMod;

#define KMOD_CTRL	(KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT	(KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT	(KMOD_LALT|KMOD_RALT)
#define KMOD_META	(KMOD_LMETA|KMOD_RMETA)

#endif /* _SDL_keysym_h */
