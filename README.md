# CrossPromo

## (document is work in progress)
This document is currently being worked on, and isn't entirely complete yet. Please do take care!

When you download a game, one of the worst things to see is some horrendously low-quality advert presented at every possible opportunity. Not only does it make the game seem of a lower quality, but it can drive audiences away from your product to other games! And we don't want that... do we?

**Welcome *CrossPromo*!** The aim of *CrossPromo* is to remove the need for using third-party advertisments all of the time, replacing the lower-paying bad quality adverisements, with high-quality native Cocos2d-x advertisements showing off aspects of your own brand or business.

Want to show off your other games? *CrossPromo* is for that.

Want to promote your game's or business' social media accounts? *CrossPromo* can help with that.

Want to tell your players when there's a special offer on regarding to one of your other games - such as a 50% off sale on the download price? *CrossPromo* can do that!

Here's a sneak peek, at a very quickly thrown together sample!

![CrossPromo preview](https://github.com/ryanbourneuk/CrossPromo/blob/develop/CrossPromoDemo.png "CrossPromo Preview")

(yaay for downscaling by 50%!...)

## Here be dragons!

I am still learning C++. This may have underlying issues which will be fixed later or not be as efficient as it could be. However, that's part of the learning process! Feel free to suggest improvements or fixes!

## So how does CrossPromo work?!

CrossPromo is a group of classes, which reads from a .json file which defines each 'promotion' object. Within the promotion object, on-screen objects are defined as a set of properties which when read allows them to be presented onto the screen!

Currently, the .json file has to be created by hand, however a sample .json file is provided for you to use as a base to work upon! There is also a reference file, to help understand the purpose of each of the properties.

When the CrossPromo classes are included within your project, the cross_promo.json file is included and the required assets are added in, you should be able to use CrossPromo at any time of your choosing - when your ad network isn't yet ready to present adverts or is otherwise unavailable.

## GIVE ME DA CODES!

Right. Fine. Here we go!

1. Download the CrossPromo.m/.h, CrossPromoManager.m/.h and CrossPromoConstants.h files, and make sure they are included within your project.
2. Create your cross_promo.json file and add it to your project, also being sure to add in any graphical assets your promotions use also into your project. Make sure you also include the crosspromo-exit assets, so there can be an exit button to leave the CrossPromo!
3. When your application starts up, wake up CrossPromoManager so it'll read the cross_promo.json file and start to populate Promo objects. I do this when I create the buttonSound callback - which will be called whenever a button is pressed to play a sound effect:

```cpp
CrossPromoManager::getInstance()->buttonSound = []{
  // Call your button sound effect here!
};
```

With the promo objects loaded, you're now free to show your promotions as and when you need to!

4. Create event listeners for these CrossPromo events, just before you present a CrossPromo:
  * CROSS_PROMO_PRESENTED
  * CROSS_PROMO_DISMISSED
  * CROSS_PROMO_CLICKED
  
  You'll need to be listening to these events so you'll be able to send analytics and be aware of when the Promo is removed from the screen so you can progress further in your game - when the Promo is closed the event will be triggered so you can re-enable your own UI and complete any further actions.
  
  ```cpp
  EventListenerCustom *presentListener = EventListenerCustom::create(EVENT_PRESENTED, [&](EventCustom *event) {
    // Promo has been presented!
  });
  
  ...
  
  // Add the presentation listener.
  Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(presentListener, 1);
  ```
  
5. Disable any of your own UI (as currently CrossPromo doesn't swallow touch inputs!) and present a CrossPromo object! We pass in a string object too, so we can determine the promotion being presented for analytics.

```cpp
// Disable touch inputs here on main UI objects.

string analyticsRef;
CrossPromoManager::getInstance()->showCrossPromoOnLayer(presentationLayer, analyticsRef);
```

6. Yaay! *CrossPromo* is on the screen! When it is closed, you will receive the dismissed/clicked event via your listeners, and be able to complete any further actions of your choosing! Be sure to remove your listeners when CrossPromo is closed!

## What are the plans for the future?!
The roadmap is clear and (relatively) simple:
* Colors from hex strings, as well as from objects.
* Cleanup of code to allow for smoother handling of non-existant null properties.
* Editing of cross_promo.json via a GUI-based application, instead of by text editor.

## (in)Frequently asked questions!!

### Why doesn't this allow for updating this file via the Internet?!
I haven't added this in, as either you have your own solution for this or you do not, and it isn't the place of CrossPromo to dictate to you how you should manage that system. It wouldn't be too difficult for you to create a system to update the cross_promo.json file and the required assets file, but it isn't something that should be part of this.

### How can I ever repay you for this amazingness?!

If this has been useful to you, then great! Feel free to send me a tweet [@ryanbournedev](twitter.com/ryanbournedev) to let me know! If you really really liked it and a nice tweet just won't do, then feel free to help fund my hot chocolate cravings here: https://www.paypal.me/ryanbourne - although donations are not necessary! :)
