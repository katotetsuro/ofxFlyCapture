openFrameworks addon for capturing video from Point Grey Research's Camera using FlyCapture SDK.

https://www.ptgrey.com/flycapture-sdk

This addon is tested under Windows only.
(note:SDK for OSX is not provided.)

features not implemented currently
* request frame size
* pixel format

# How to use

* Install FlyCapture SDK windows 32bit
* copy header files inside C:\Program Files (x86)\Point Grey Research\FlyCapture2\include\ to ofxFlyCapture\libs\FlyCapture2\include
* sub-folder "C" and "FC1" are not necessary.
* copy library file C:\Program Files (x86)\Point Grey Research\FlyCapture2\lib\FlyCapture2.lib to ofxFlyCapture\libs\FlyCapture2\libvs\Win32\

![](https://i.gyazo.com/977d45d33c98e66cf8acfd3909daf314.png)


* I recommend to use Project Generator. It is easy to setup include and library path
* To launch your app, C:\Program Files (x86)\Point Grey Research\FlyCapture2\bin\ should in your PATH environment variable, or FlyCapture2.dll should be in same folder of your exe file.

I don't know much about software license, but it seems better not to redistribute headers and libraries.
http://www.ptgrey.com/Content/Images/uploaded/FlyCapture2Help/flycapture/07legal/softwarelicenseagreement.html
