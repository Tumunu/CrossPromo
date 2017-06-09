//
//  CrossPromoManager.cpp
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

#include "CrossPromoManager.h"

#include <json/rapidjson.h>
#include <json/document.h>

using namespace rapidjson;

CrossPromoManager *CrossPromoManager::crossPromoManagerSingleton = NULL;

CrossPromoManager::CrossPromoManager()
: buttonSound(nullptr)
{ }

CrossPromoManager::~CrossPromoManager() { }

CrossPromoManager *CrossPromoManager::getInstance() {
  if (NULL == crossPromoManagerSingleton) {
    crossPromoManagerSingleton = new CrossPromoManager();
    crossPromoManagerSingleton->getCrossPromos();
    crossPromoManagerSingleton->viewSize = Director::getInstance()->getWinSize();
  }
  
  return crossPromoManagerSingleton;
}

bool CrossPromoManager::canShowCrossPromo() {
  return crossPromos.size() > 0;
}

void CrossPromoManager::showCrossPromoOnLayer(Layer &target, string &analyticsReference) {
  // Get random promo!
  auto promoData = crossPromos.at(randomIndexUpTo(crossPromos.size()));
  
  // Create a dark layer.
  darkLayer = LayerColor::create(Color4B(0, 0, 0, 0));
  darkLayer->setIgnoreAnchorPointForPosition(false);
  darkLayer->setAnchorPoint(Vec2(.5, .5));
  target.addChild(darkLayer, 100);
  darkLayer->setPosition(Vec2(target.getBoundingBox().size.width * .5, target.getBoundingBox().size.height * .5));
  
  // Create the white layer, which acts as the margin.
  marginLayer = LayerColor::create(Color4B::WHITE, (viewSize.width * PROMO_WIDTH) + PROMO_MARGIN, (viewSize.height * PROMO_HEIGHT) + PROMO_MARGIN);
  marginLayer->setIgnoreAnchorPointForPosition(false);
  marginLayer->setAnchorPoint(Vec2(.5, .5));
  darkLayer->addChild(marginLayer, kLayerFrame);
  marginLayer->setPosition(posInWindowFrom(.5, .5));
  
  // Create the promo layer.
  auto promoLayer = CrossPromo::create(promoData, buttonSound);
  marginLayer->addChild((Node*)promoLayer, kLayerPromo);
  ((Node*)promoLayer)->setPosition(Vec2(marginLayer->getBoundingBox().size.width * .5, marginLayer->getBoundingBox().size.height * .5));
  
  // Create the exit button.
  exitButton = Button::create("crosspromo-exit.png");
  exitButton->setEnabled(false);
  marginLayer->addChild(exitButton, kLayerExitButton);
  exitButton->setPosition(Vec2(PROMO_MARGIN/4, PROMO_MARGIN/4));
  exitButton->addTouchEventListener([&, promoLayer](Ref *sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
      return;
    }
    
    promoLayer->setEnabled(false);
    
    if (buttonSound != nullptr) {
      buttonSound();
    }
    
    CrossPromoManager::getInstance()->promoClosedWithState(ExitDimissed);
  });
  
  // Fade in dark layer to 127.5 alpha, and animate in cross promo layer.
  marginLayer->setScale(0);
  darkLayer->runAction(FadeTo::create(.5, 127.5));
  
  // Determine reference for analytics.
  analyticsReference = promoLayer->getCrossPromoReference();
  
  marginLayer->runAction(Sequence::create(EaseSineInOut::create(EaseBackOut::create(ScaleTo::create(.5, 1))),
                                              DelayTime::create(.5),
                                              CallFunc::create([&, promoLayer] {
    // Dispatch an event, to let listeners know a CrossPromo is now presented on screen.
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_PRESENTED);
    
    exitButton->setEnabled(true);
    promoLayer->setEnabled(true);
                                              }), NULL));
}

void CrossPromoManager::promoClosedWithState(PromoExitStates exitState) {
  // Animate out the foreground items, fade out dark layer.
  darkLayer->runAction(FadeTo::create(.5, 0));
  
  marginLayer->runAction(EaseSineInOut::create(EaseBackIn::create(ScaleTo::create(.5, 0))));
  
  // When done, remove all objects!
  Director::getInstance()->getScheduler()->schedule([&,exitState](float dt) {
    darkLayer->removeAllChildren();
    darkLayer->removeFromParent();
    
    // Dispatch an event, to let listeners know the exit state of the CrossPromo.
    if (exitState == ExitDimissed) {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_DISMISSED);
    } else {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_CLICKED);
    }
  }, this, 0, 0, .5, false, "removepromo");
}

void CrossPromoManager::getCrossPromos() {
  // Read the CrossPromo file to a string object.
  string crossPromosString = FileUtils::getInstance()->getStringFromFile(CROSS_PROMO_FILE);
  
  Document crossPromosFile;
  crossPromosFile.Parse(crossPromosString.c_str());
  
  rapidjson::Value &advertArray = crossPromosFile[PROMOS.c_str()];
  
  if (advertArray.IsNull()) {
    return;
  }
  
  auto currentPlatform = 0;
  
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
  currentPlatform = 1;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  currentPlatform = 2;
#endif
  
  crossPromos.reserve(getNumberOfPromosForPlatform(advertArray, currentPlatform));
  
  for (rapidjson::SizeType i = 0; i < advertArray.Size(); i++) {
    auto targetPlatform = getIntFrom(advertArray[i], TARGET_PLATFORM, 0);
    
    if (targetPlatform == 0 || (targetPlatform == currentPlatform)) {
      // Populate an array of advert arrays! We'll use these when they're called to populate the layer. We don't do this on startup!
      crossPromos.emplace_back(getPromoDataFor(advertArray[i].GetObject()));
    }
  }
}

int CrossPromoManager::getNumberOfPromosForPlatform(rapidjson::Value &jsonObject, int currentPlatform) {
  auto count = 0;
  
  for (rapidjson::SizeType i = 0; i < jsonObject.Size(); i++) {
    auto targetPlatform = getIntFrom(jsonObject[i], TARGET_PLATFORM, 0);
    
    if (targetPlatform == 0 || (targetPlatform == currentPlatform)) {
      count++;
    }
  }
  
  return count;
}

PromoData CrossPromoManager::getPromoDataFor(rapidjson::Value jsonObject) {
  PromoData promoData = PromoData();
  
  promoData.reference = getStringFrom(jsonObject, REFERENCE);
  promoData.crossPromoType = (CrossPromoType)getIntFrom(jsonObject, PROMO_TYPE, 0);
  
  rapidjson::Value backgroundData = jsonObject[BACKGROUND.c_str()].GetObject();
  promoData.background.start = getColorFrom(backgroundData, START_COLOR);
  promoData.background.end = getColorFrom(backgroundData, END_COLOR);
  
  auto promoObjects = jsonObject[OBJECTS.c_str()].GetArray();
  
  promoData.promoObjectData.reserve(promoObjects.Size());
  
  for (rapidjson::SizeType i = 0; i < promoObjects.Size(); i++) {
    promoData.promoObjectData.push_back(getPromoObjectDataFor(promoObjects[i]));
  }
  
  return promoData;
}

PromoObjectData CrossPromoManager::getPromoObjectDataFor(rapidjson::Value &jsonObject) {
  PromoObjectData promoObjectData;
  
  promoObjectData.objectType = (ObjectType)getIntFrom(jsonObject, OBJECT_TYPE, 0);
  promoObjectData.buttonType = (ButtonType)getIntFrom(jsonObject, BUTTON_TYPE, PROMO_INVALID_INT);
  
  promoObjectData.spriteName = getStringFrom(jsonObject, SPRITE_NAME);
  
  promoObjectData.title = getStringFrom(jsonObject, TITLE);
  promoObjectData.font = getStringFrom(jsonObject, TITLE_FONT);
  promoObjectData.fontSize = getIntFrom(jsonObject, FONT_SIZE, PROMO_INVALID_INT);
  
  promoObjectData.posRelativeTo = getIntFrom(jsonObject, POS_RELATIVE_TO, PROMO_INVALID_INT);
  promoObjectData.position = getPositionFrom(jsonObject, POS);
  promoObjectData.anchorPoint = getPositionFrom(jsonObject, ANCHOR_POINT);
  promoObjectData.rotation = getIntFrom(jsonObject, ROTATION, 0);
  
  promoObjectData.dimensions = getSizeFrom(jsonObject, DIMENSIONS);
  
  promoObjectData.color = getColorFrom(jsonObject, COLOR);
  
  promoObjectData.zOrder = getIntFrom(jsonObject, Z_ORDER, 0);
  
  promoObjectData.iOSUrl = getStringFrom(jsonObject, IOS_URL);
  promoObjectData.androidUrl = getStringFrom(jsonObject, ANDROID_URL);
  
  promoObjectData.objectTag = getIntFrom(jsonObject, OBJECT_TAG, 0);
  
  return promoObjectData;
}
  
string CrossPromoManager::getStringFrom(rapidjson::Value &jsonObject, string key) {
  if (!jsonObject[key.c_str()].IsString()) {
    return "";
  }
  
  return jsonObject[key.c_str()].GetString();
}

int CrossPromoManager::getIntFrom(rapidjson::Value &jsonObject, string key, int defaultValue) {
  if (!jsonObject[key.c_str()].IsInt()) {
    return defaultValue;
  }
  
  return jsonObject[key.c_str()].GetInt();
}

float CrossPromoManager::getFloatFrom(rapidjson::Value &jsonObject, string key, float defaultValue) {
  if (!jsonObject[key.c_str()].IsFloat()) {
    return defaultValue;
  }
  
  return jsonObject[key.c_str()].GetFloat();
}

ColorData CrossPromoManager::getColorFrom(rapidjson::Value &jsonObject, string key) {
  ColorData colorData;
  
  colorData.r = getIntFrom(jsonObject[key.c_str()], RED, 0);
  colorData.g = getIntFrom(jsonObject[key.c_str()], GREEN, 0);
  colorData.b = getIntFrom(jsonObject[key.c_str()], BLUE, 0);
  
  return colorData;
}

PositionData CrossPromoManager::getPositionFrom(rapidjson::Value &jsonObject, string key) {
  PositionData positionData;
  
  positionData.x = getFloatFrom(jsonObject[key.c_str()], X_POS, 0.0);
  positionData.y = getFloatFrom(jsonObject[key.c_str()], Y_POS, 0.0);
  
  return positionData;
}

SizeData CrossPromoManager::getSizeFrom(rapidjson::Value &jsonObject, string key) {
  SizeData sizeData;
  
  sizeData.width = -1;
  sizeData.height = -1;
  
  if (jsonObject[key.c_str()].IsNull()) {
    return sizeData;
  }
  
  sizeData.width = getFloatFrom(jsonObject[key.c_str()], WIDTH, -1.0);
  sizeData.height = getFloatFrom(jsonObject[key.c_str()], HEIGHT, -1.0);
  
  return sizeData;
}

Vec2 CrossPromoManager::posInWindowFrom(float x, float y) {
  return Vec2(viewSize.width * x,viewSize.height * y);
}

int CrossPromoManager::randomIndexUpTo(int n) {
  return ((rand()%n));
}
