#ifndef FONT_H
#define FONT_H

#include "SDL.h"
#include <string>

// *** Code ***
//
//   font->drawStr( screen, 150, 260, "Zeile 1" ); 
//   font->drawStr( screen, 150, 290, "Zeile 2", FONT_ALIGN_CENTERED ); 
//   font->drawStr( screen, 150, 320, "Zeile 3", FONT_ALIGN_RIGHT ); 
//   font->drawInt( screen, 150, 350, 123, 0 ); 
//   font->drawInt( screen, 150, 380, 123, 0, FONT_ALIGN_FILL_ZERO ); 
//   font->drawInt( screen, 150, 410, 123, 6 ); 
//   font->drawInt( screen, 150, 440, 123, 6, FONT_ALIGN_FILL_ZERO ); 
//   font->drawInt( screen, 150, 400, 123, 6, FONT_ALIGN_CENTERED ); 
//   font->drawInt( screen, 150, 425, 123, 6, FONT_ALIGN_CENTERED | FONT_ALIGN_FILL_ZERO ); 
//   font->drawInt( screen, 150, 350, 123, 6, FONT_ALIGN_RIGHT ); 
//   font->drawInt( screen, 150, 375, 123, 6, FONT_ALIGN_RIGHT | FONT_ALIGN_FILL_ZERO ); 
//
// *** Result ***
//
//          Zeile 1
//      Zeile 2
//   Zeile 3
//          123
//          123
//             123
//          000123
//          123
//       000123
//       123
//    000123

const int FONT_ALIGN_FILL_ZERO = (1<<0);  // fill with leading zeros  
const int FONT_ALIGN_CENTERED  = (1<<1);  // text centered around posx/posy
const int FONT_ALIGN_RIGHT     = (1<<2);  // text aligned right (on the left side of posx)
const int FONT_MONOSPACE       = (1<<3);

class Font
{
private:
	SDL_Surface* m_pSprite;
	int m_iSprite;
	int m_iCharWidth;
	int m_iCharHeight;
	std::string m_sCharset;

public:
	Font(std::string fn);
	~Font();

	void SetCharWidth(int width);
	int GetCharWidth();
	int GetCharHeight();
	void DrawInt(SDL_Surface *screen, int posx, int posy, int val, int alignDigitCnt, int flags = 0);
	void DrawStr(SDL_Surface *screen, int posx, int posy, const std::string &text, int flags = 0);
};

#endif
