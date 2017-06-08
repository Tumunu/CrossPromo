//
//  CrossPromo.h
//
//  Copyright (c) 2017 Ryan Bourne. All rights reserved.
//  https://www.ryanbournedev.wordpress.com & https://twitter.com/ryanbourne28
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef CrossPromo_h
#define CrossPromo_h

#include "cocos2d.h"

#include "CrossPromoConstants.h"

using namespace std;
using namespace cocos2d;

class CrossPromo: LayerGradient {
public:
  CrossPromo();
  ~CrossPromo();
  
  static CrossPromo *create(PromoData crossPromo, function<void()> &buttonSound);
  
  string getCrossPromoReference();
  
  void setEnabled(bool enabled);
  
private:
  virtual bool init(PromoData crossPromo, function<void()> &buttonSound);
  
  void findObjectsIn(vector<PromoObjectData> promoObjectData);
  void addObjectFrom(PromoObjectData promoObjectData);
  
  cocos2d::Label *createLabelFrom(PromoObjectData data);
  cocos2d::Sprite *createSpriteFrom(PromoObjectData data);
  ui::Button *createButtonFrom(PromoObjectData data);
  
  Color4B getColor4BFrom(ColorData data);
  string generateFontFileNameFrom(string fontName);
  string generateSpriteFileNameFrom(string filename);
  
  Vec2 posInViewFrom(float x, float y);
  Vec2 posInViewRelativeToObjWith(int tag, float x, float y);
  
  string reference;
  
  CrossPromoType promoType;
  
  Size viewSize;
  
  function<void()> buttonSoundCallback;
  
  Vector<ui::Button*> promoButtons;
};

#endif /* CrossPromo_h */
