#pragma once 

// Versions enumeration:
// {Release} . {Debug} . {Concept Idea} . {Bugfix} 
// {Concept Idea}s:
// = 1 - Flow operations
// = 2 - User Flow operations separated from main application (implemented in dlls)
// = 3 - Layout viewer
// ? 4 - Verlog, LEF, DEF, separate inputs and outputs
#define VERSION_STRING              QString("v0.0.3.3")

// https://www.canva.com/colors/color-palette-generator/
// https://www.canva.com/colors/color-palettes/retro-punch/
#define PIN_COLOR_LAYOUT_EMPTY      QColor(245, 23, 32)
#define PIN_COLOR_LAYOUT_CONNECTED  QColor(145, 23, 32)

// https://www.canva.com/colors/color-palettes/sea-of-serenity/
#define PIN_COLOR_LEF_EMPTY         QColor(5, 157, 192)
#define PIN_COLOR_LEF_CONNECTED     QColor(2,67,118)

// https://www.canva.com/colors/color-palettes/sea-of-serenity/
#define PIN_COLOR_DEF_EMPTY         QColor(236, 248, 127)
#define PIN_COLOR_DEF_CONNECTED     QColor(89, 152, 26)

// https://www.canva.com/colors/color-palettes/under-the-ferns/
#define PIN_COLOR_VERILOG_EMPTY     QColor(0, 165, 88)
#define PIN_COLOR_VERILOG_CONNECTED QColor(17, 101, 48)

