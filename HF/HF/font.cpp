using namespace std;

#include "font.h"
#include "surfaceDB.h"
#include "video.h"
#include <iostream>

Font::Font(string fn) {
  m_iSprite = TextureManager::GetInstance()->LoadSurface( fn );
  m_pSprite = TextureManager::GetInstance()->GetTextureById( m_iSprite );
  m_sCharset = " ABCDEFGHIJKLMNOPQRSTUVWXYZÜÄÖabcdefghijklmnopqrstuvwxyzüäöß0123456789!\"§$%&/()=?*+'#,.-;:_@°\\";
  // 94 Zeichen
  m_iCharWidth = m_pSprite->w / 94;
  m_iCharHeight = m_pSprite->h;
}

Font::~Font() {

}

void Font::SetCharWidth(int width) {
  m_iCharWidth = width;
}

int Font::GetCharWidth() {
  return m_iCharWidth;
}

int Font::GetCharHeight() {
  return m_pSprite->h;
}

void Font::DrawInt(SDL_Surface *screen, int posx, int posy, int val, int alignDigitCnt, int flags) {
  int indent = 0;
  int digitCnt = 1;
  int i=1;
  while ( val >= i*10 ) {
    digitCnt++;
    i *= 10;
  }

//   cout << endl << "drawInt.val=" << val << endl;
//   cout << "drawInt.digitCnt=" << digitCnt << endl;
//   cout << "drawInt.alignDigitCnt-old=" << alignDigitCnt << endl;

  if (alignDigitCnt < digitCnt) {
    alignDigitCnt = digitCnt;
  }
//   cout << "drawInt.alignDigitCnt-new=" << alignDigitCnt << endl;
  
  if (flags & FONT_ALIGN_CENTERED) {
    indent = -(alignDigitCnt * m_iCharWidth) / 2;
  }
  if (flags & FONT_ALIGN_RIGHT) {
    indent = -(alignDigitCnt * m_iCharWidth);
  }

  SDL_Rect destR;
  SDL_Rect srcR;
  while (alignDigitCnt > 0) {
    if ((digitCnt > 0) ||
	((flags & FONT_ALIGN_FILL_ZERO) != 0)) {
      
      destR.x = indent + posx + (alignDigitCnt-1) * m_iCharWidth;
      destR.y = posy;
      destR.w = m_iCharWidth;
      destR.h = m_iCharHeight;
      
      unsigned int charsetpos = m_sCharset.find( (char)((val % 10) + '0') );
      if (charsetpos != string::npos ) {
	srcR.x = charsetpos * m_iCharWidth;
      } else {
	srcR.x = m_iCharWidth;
      }
      //      srcR.x = (1 + 2*26 + (val % 10)) * charWidth;
      srcR.y = 0;
      srcR.w = m_iCharWidth;
      srcR.h = m_iCharHeight;
      
      SDL_BlitSurface( m_pSprite, &srcR, screen, &destR );
    }
    val /= 10;
    digitCnt--;
    alignDigitCnt--;
  }
}


  
void Font::DrawStr(SDL_Surface *screen, int posx, int posy, const string &text, int flags) {

  int indent = 0;
  if ( flags & (FONT_ALIGN_CENTERED | FONT_ALIGN_RIGHT) ) {
    for(unsigned int i=0; i < text.size(); ++i) {
      if (!(flags & FONT_MONOSPACE) && text[i] == ' ') {
	indent += ((m_iCharWidth * 2) / 3);
      } else {
	indent += m_iCharWidth;
      }
    }
    if (flags & FONT_ALIGN_CENTERED) {
      indent = -indent / 2;
    } else {
      indent = -indent;
    }
  }

  SDL_Rect destR;
  SDL_Rect srcR;
  
  int x = 0;
  unsigned int charsetpos;
  for(unsigned int i=0; i < text.size(); ++i) {
    x = 0;
    charsetpos = m_sCharset.find(text[i]);
    if (charsetpos != string::npos ) {
      x = charsetpos * m_iCharWidth;
    }
    
    destR.x = posx + indent;
    destR.y = posy;
    destR.w = m_iCharWidth;
    destR.h = m_pSprite->h;
      
    srcR.x = x;
    srcR.y = 0;
    srcR.w = m_iCharWidth;
    srcR.h = m_pSprite->h;
      
    Video::GetInstance()->DrawRect( m_iSprite, &srcR, &destR );

    if (!(flags & FONT_MONOSPACE) && text[i] == ' ') {
      posx += ((m_iCharWidth * 2) / 3);
    } else {
      posx += m_iCharWidth;
    }
  }
}

