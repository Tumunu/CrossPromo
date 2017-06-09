//
//  CrossPromoConstants.h
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

#ifndef CrossPromoConstants_h
#define CrossPromoConstants_h

using namespace std;

static const string &CROSS_PROMO_FILE = "cross_promo.json";

static const string &EVENT_PRESENTED = "CROSS_PROMO_PRESENTED";
static const string &EVENT_DISMISSED = "CROSS_PROMO_DISMISSED";
static const string &EVENT_CLICKED = "CROSS_PROMO_CLICKED";

static const string &PROMOS = "promos";
static const string &REFERENCE = "reference";
static const string &TARGET_PLATFORM = "targetplatform";
static const string &START_DATE = "startdate";
static const string &END_DATE = "enddate";
static const string &PROMO_TYPE = "promotype";
static const string &BACKGROUND = "background";
static const string &START_COLOR = "startcolor";
static const string &END_COLOR = "endcolor";
static const string &OBJECTS = "objects";
static const string &OBJECT_TYPE = "objecttype";
static const string &BUTTON_TYPE = "buttontype";
static const string &SPRITE_NAME = "spritename";
static const string &TITLE = "title";
static const string &TITLE_FONT = "font";
static const string &FONT_SIZE = "fontsize";
static const string &POS_RELATIVE_TO = "posrelativeto";
static const string &POS = "pos";
static const string &ANCHOR_POINT = "anchorpoint";
static const string &X_POS = "x";
static const string &Y_POS = "y";
static const string &ROTATION = "rotation";
static const string &DIMENSIONS = "dimensions";
static const string &WIDTH = "width";
static const string &HEIGHT = "height";
static const string &COLOR = "color";
static const string &RED = "r";
static const string &GREEN = "g";
static const string &BLUE = "b";
static const string &Z_ORDER = "z";
static const string &IOS_URL = "iOS-url";
static const string &ANDROID_URL = "android-url";
static const string &OBJECT_TAG = "objecttag";

static const float &PROMO_WIDTH = .7;
static const float &PROMO_HEIGHT = .85;
static const int &PROMO_MARGIN = 12;

static const int &PROMO_INVALID_INT = -1;

typedef enum {
  ExitDimissed,
  ExitClicked
} PromoExitStates;

typedef enum {
  kLayerDarkBase = 100,
  kLayerFrame = 101,
  kLayerPromo = 102,
  kLayerExitButton = 103
} LayerZOrders;

typedef enum {
  BothPlatforms,
  OnlyAndroid,
  OnlyiOS
} TargetPlatform;

typedef enum {
  Twitter,
  Facebook,
  Game
} CrossPromoType;

typedef enum {
  Label,
  Sprite,
  Button
} ObjectType;

typedef enum {
  Invalid = -1,
  Exit = 0,
  OpenURL = 1
} ButtonType;

struct ColorData {
  int r;
  int g;
  int b;
};

struct BackgroundGradientData {
  ColorData start;
  ColorData end;
};

struct PositionData {
  float x;
  float y;
};

struct SizeData {
  float width;
  float height;
};

struct PromoObjectData {
  ObjectType objectType;
  ButtonType buttonType;
  string spriteName;
  string title;
  string font;
  int fontSize;
  int posRelativeTo;
  PositionData position;
  PositionData anchorPoint;
  int rotation;
  SizeData dimensions;
  ColorData color;
  int zOrder;
  string iOSUrl;
  string androidUrl;
  int objectTag;
};

struct PromoData {
  string reference;
  CrossPromoType crossPromoType;
  BackgroundGradientData background;
  vector<PromoObjectData> promoObjectData;
};

#endif /* CrossPromoConstants_h */
