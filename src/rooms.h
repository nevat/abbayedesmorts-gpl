/* rooms.h */

#pragma once

enum rooms {
	ROOM_NULL = 0,
	ROOM_PRAYER,
	ROOM_TOWER,
	ROOM_WINE,
	ROOM_THEEND,
	ROOM_ESCAPE, // 5
	ROOM_CLOSE,
	ROOM_CHURCH,
	ROOM_ALTAR,
	ROOM_HANGMAN,
	ROOM_BEAST, // 10
	ROOM_CAVE,
	ROOM_RUINS,
	ROOM_CATACOMBS,
	ROOM_GARDEN,
	ROOM_TUNELS, // 15
	ROOM_LAKE,
	ROOM_WHEEL,
	ROOM_BANQUET,
	ROOM_RIVER,
	ROOM_GATE, // 20
	ROOM_EVIL,
	ROOM_SOULS,
	ROOM_ASHES,
	ROOM_SATAN,
	ROOM_INVALID // 25
};

#ifdef INSIDE_GAME_C
const char* room_names[] = {
	"NULL",
	"A prayer of hope",
	"Tower of the Bell",
	"Wine supplies",
	"THE END",
	"Escape!!!",
	"Death is close",
	"Abandoned church",
	"The Altar",
	"Hangman Tree",
	"Pestilent Beast",
	"Cave of illusions",
	"Plagued ruins",
	"Catacombs",
	"Hiiden garden",
	"Gloomy tunels",
	"Lake of despair",
	"The wheel of faith",
	"Banquet of Death",
	"Underground river",
	"Unexpected gate",
	"Evil church",
	"Tortured souls",
	"Ashes to ashes",
	"Satan !!!"
};
#else
extern const char* room_names[];
#endif