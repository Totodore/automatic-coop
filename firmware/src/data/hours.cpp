#include <stdint.h>

constexpr uint16_t maxMonth[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
constexpr uint8_t sunrise[52][2] = {
		{9, 48},	// 09:48
		{9, 46},	// 09:46
		{9, 43},	// 09:43
		{9, 37},	// 09:37
		{9, 29},	// 09:29
		{9, 19},	// 09:19
		{9, 8},		// 09:08
		{8, 55},	// 08:55
		{8, 42},	// 08:42
		{8, 28},	// 08:28
		{8, 14},	// 08:14
		{7, 59},	// 07:59
		{7, 45},	// 07:45
		{7, 30},	// 07:30
		{7, 16},	// 07:16
		{7, 2},		// 07:02
		{6, 48},	// 06:48
		{6, 36},	// 06:36
		{6, 24},	// 06:24
		{6, 14},	// 06:14
		{6, 6},		// 06:06
		{5, 59},	// 05:59
		{5, 54},	// 05:54
		{5, 51},	// 05:51
		{5, 50},	// 05:50
		{5, 52},	// 05:52
		{5, 55},	// 05:55
		{6, 1},		// 06:01
		{6, 8},		// 06:08
		{6, 16},	// 06:16
		{6, 24},	// 06:24
		{6, 34},	// 06:34
		{6, 43},	// 06:43
		{6, 53},	// 06:53
		{7, 3},		// 07:03
		{7, 13},	// 07:13
		{7, 23},	// 07:23
		{7, 32},	// 07:32
		{7, 42},	// 07:42
		{7, 52},	// 07:52
		{8, 3},		// 08:03
		{8, 13},	// 08:13
		{8, 24},	// 08:24
		{8, 35},	// 08:35
		{8, 46},	// 08:46
		{8, 58},	// 08:58
		{9, 9},		// 09:09
		{9, 19},	// 09:19
		{9, 28},	// 09:28
		{9, 36},	// 09:36
		{9, 42},	// 09:42
		{9, 46}}; // 09:46
constexpr uint8_t sunset[52][2] = {
		{17, 59},
		{18, 7},
		{18, 16},
		{18, 26},
		{18, 38},
		{18, 49},
		{19, 1},
		{19, 12},
		{19, 24},
		{19, 35},
		{19, 46},
		{19, 57},
		{20, 7},
		{20, 18},
		{20, 28},
		{20, 38},
		{20, 49},
		{20, 59},
		{21, 9},
		{21, 19},
		{21, 28},
		{21, 36},
		{21, 43},
		{21, 48},
		{21, 52},
		{21, 53},
		{21, 53},
		{21, 50},
		{21, 45},
		{21, 38},
		{21, 29},
		{21, 19},
		{21, 7},
		{20, 54},
		{20, 41},
		{20, 27},
		{20, 12},
		{19, 57},
		{19, 42},
		{19, 27},
		{19, 13},
		{18, 59},
		{18, 45},
		{18, 32},
		{18, 21},
		{18, 11},
		{18, 2},
		{17, 56},
		{17, 51},
		{17, 49},
		{17, 50},
		{17, 53},
};