cd %1

mklink /D cocos2dx E:\cocos2d-2.0-x-2.0.2\cocos2dx
mklink /D CocosDenshion E:\cocos2d-2.0-x-2.0.2\CocosDenshion
mklink /D extensions E:\cocos2d-2.0-x-2.0.2\extensions
mklink /D external E:\cocos2d-2.0-x-2.0.2\external

md Debug.win32
xcopy E:\cocos2d-2.0-x-2.0.2\Debug.win32\*.dll Debug.win32
