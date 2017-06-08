//
//  CrossPromo.cpp
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

#include "CrossPromo.h"
#include "CrossPromoManager.h"

CrossPromo::CrossPromo() { }

CrossPromo::~CrossPromo() {
  
}

CrossPromo *CrossPromo::create(PromoData crossPromo, function<void()> &buttonSound) {
  CrossPromo *pRet = new(nothrow) CrossPromo();
  
  if(!pRet->init(crossPromo, buttonSound)) {
    return nullptr;
  }
  
  return pRet;
}

bool CrossPromo::init(PromoData crossPromo, function<void()> &buttonSound) {
  if(!LayerGradient::initWithColor(getColor4BFrom(crossPromo.background.start), getColor4BFrom(crossPromo.background.end))) {
    return false;
  }
  
  auto visibleSize = Director::getInstance()->getWinSize();
  
  this->setContentSize(Size(visibleSize.width * PROMO_WIDTH, visibleSize.height * PROMO_HEIGHT));
  
  // Get the promo reference and type.
  reference = crossPromo.reference;
  promoType = crossPromo.crossPromoType;
  
  this->setIgnoreAnchorPointForPosition(false);
  this->setAnchorPoint(Vec2(.5, .5));
  this->setCascadeOpacityEnabled(true);
  
  // Get the size of the promo view, so we can position objects.
  viewSize = this->getBoundingBox().size;
  
  buttonSoundCallback = buttonSound;
  
  // Pass the objects array onwards so we can find the objects we need to add, and add them!
  findObjectsIn(crossPromo.promoObjectData);
  
  return true;
}

string CrossPromo::getCrossPromoReference() {
  return StringUtils::format("Cross Promo (%s)", reference.c_str());
}

void CrossPromo::findObjectsIn(vector<PromoObjectData> promoObjectData) {
  for (int i = 0; i < promoObjectData.size(); i++) {
    addObjectFrom(promoObjectData.at(i));
  }
}

void CrossPromo::addObjectFrom(PromoObjectData promoObjectData) {
  // Check the object type, depending on this, pass the object onwards to be added to the promo layer!
  auto positionOb = promoObjectData.position;
  auto anchorOb = promoObjectData.anchorPoint;
  
  auto position = Vec2(positionOb.x, positionOb.y);
  auto anchorPoint = Vec2(anchorOb.x, anchorOb.y);
  
  Node *object;
  
  switch (promoObjectData.objectType) {
    case ObjectType::Label:
      object = createLabelFrom(promoObjectData);
      break;
      
    case ObjectType::Sprite:
      object = createSpriteFrom(promoObjectData);
      break;
      
    case ObjectType::Button:
      object = createButtonFrom(promoObjectData);
      break;
      
    default:
      break;
  }
  
  // Set the anchor point of the object.
  object->setAnchorPoint(anchorPoint);
  object->setRotation(promoObjectData.rotation);
  
  this->addChild(object, promoObjectData.zOrder, promoObjectData.objectTag);
  
  // See if the position is relative to another object, and if so try to get it. Otherwise get a position relative to the promo background!
  if (promoObjectData.posRelativeTo != -1) {
    object->setPosition(posInViewRelativeToObjWith(promoObjectData.posRelativeTo, position.x, position.y));
  } else {
    object->setPosition(posInViewFrom(position.x, position.y));
  }
}

cocos2d::Label *CrossPromo::createLabelFrom(PromoObjectData data) {
  auto label = Label::createWithTTF(data.title, generateFontFileNameFrom(data.font), data.fontSize);
  
  label->setColor(Color3B(getColor4BFrom(data.color)));
  
  if (data.dimensions.width != -1 && data.dimensions.height != -1) {
    auto dimensionsOb = data.dimensions;
    
    label->setOverflow(Label::Overflow::SHRINK);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setDimensions(viewSize.width * dimensionsOb.width, viewSize.height * dimensionsOb.height);
  }
  
  return label;
}

cocos2d::Sprite *CrossPromo::createSpriteFrom(PromoObjectData data) {
  return Sprite::createWithSpriteFrameName(generateSpriteFileNameFrom(data.spriteName));
}

cocos2d::ui::Button *CrossPromo::createButtonFrom(PromoObjectData data) {
  ui::Button *btn;
  
  if (data.spriteName.empty()) {
    btn = ui::Button::create("");
    btn->setTitleText(data.title);
    btn->setTitleFontName(generateFontFileNameFrom(data.font));
    btn->setTitleFontSize(data.fontSize);
    btn->setTitleColor(Color3B(getColor4BFrom(data.color)));
  } else {
    btn = ui::Button::create(generateSpriteFileNameFrom(data.spriteName));
  }
  
  btn->setEnabled(false);
  
  btn->addTouchEventListener([&,data](Ref *sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
      return;
    }
    
    this->setEnabled(false);
    
    if (buttonSoundCallback != nullptr) {
      buttonSoundCallback();
    }
    
    // Determine the button type, and complete different actions based on it.
    auto buttonType = data.buttonType;
    
    if (buttonType == Exit) {
      CrossPromoManager::getInstance()->promoClosedWithState(ExitDimissed);
    } else {
      string url;
      
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
      url = data.androidUrl;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
      url = data.iOSUrl;
#endif
      
      Application::getInstance()->openURL(url);
      
      CrossPromoManager::getInstance()->promoClosedWithState(ExitClicked);
    }
  });
  
  btn->retain();
  
  promoButtons.pushBack(btn);
  
  return btn;
}

void CrossPromo::setEnabled(bool enabled) {
  for (int i = 0; i < promoButtons.size(); i++) {
    promoButtons.at(i)->setEnabled(enabled);
  }
}

Color4B CrossPromo::getColor4BFrom(ColorData data) {
  return Color4B(data.r, data.g, data.b, 255);
}

string CrossPromo::generateFontFileNameFrom(string fontName) {
  return StringUtils::format("fonts/%s.ttf",fontName.c_str());
}

string CrossPromo::generateSpriteFileNameFrom(string filename) {
  return StringUtils::format("%s.png",filename.c_str());
}

Vec2 CrossPromo::posInViewFrom(float x, float y) {
  return Vec2(viewSize.width * x,viewSize.height * y);
}

Vec2 CrossPromo::posInViewRelativeToObjWith(int tag, float x, float y) {
  auto objectForTag = this->getChildByTag(tag);
  if (objectForTag == nullptr) {
    return Vec2(0, 0);
  }
  
  auto taggedObjectPos = objectForTag->getPosition();
  
  return Vec2(taggedObjectPos.x + (viewSize.width * x), taggedObjectPos.y + (viewSize.height * y));
}
