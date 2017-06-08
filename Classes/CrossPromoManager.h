//
//  CrossPromoManager.h
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

#ifndef CrossPromoManager_h
#define CrossPromoManager_h

#include "cocos2d.h"
#include "CrossPromo.h"
#include "CrossPromoConstants.h"

#include <json/rapidjson.h>
#include <json/document.h>

using namespace std;
using namespace cocos2d;
using namespace ui;

class CrossPromoManager {
private:
  static CrossPromoManager *crossPromoManagerSingleton;
  
  vector<PromoData> crossPromos;
  
public:
  CrossPromoManager();
  ~CrossPromoManager();
  
  static CrossPromoManager *getInstance();
  
  bool canShowCrossPromo();
  void showCrossPromoOnLayer(Layer &target, string &analyticsReference);
  void promoClosedWithState(PromoExitStates exitState);
  
  void getCrossPromos();
  
  int getNumberOfPromosForPlatform(rapidjson::Value &jsonObject, int currentPlatform);
  
  PromoData getPromoDataFor(rapidjson::Value jsonObject);
  PromoObjectData getPromoObjectDataFor(rapidjson::Value &jsonObject);
  
  string getStringFrom(rapidjson::Value &jsonObject);
  int getIntFrom(rapidjson::Value &jsonObject, int defaultValue);
  float getFloatFrom(rapidjson::Value &jsonObject, float defaultValue);
  
  ColorData getColorFrom(rapidjson::Value &jsonObject);
  PositionData getPositionFrom(rapidjson::Value &jsonObject);
  SizeData getSizeFrom(rapidjson::Value &jsonObject);
  
  Vec2 posInWindowFrom(float x, float y);
  int randomIndexUpTo(int n);
  
  Size viewSize;
  
  LayerColor *darkLayer;
  LayerColor *marginLayer;
  
  ui::Button *exitButton;
  
  function<void()> buttonSound;
};

#endif /* CrossPromoManager_h */
