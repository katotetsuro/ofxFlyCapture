#include "ofxFlyCapture.h"
#include "FlyCapture2.h"

using namespace FlyCapture2;

ofxFlyCapture::ofxFlyCapture() : 
	bChooseDevice(false),
	bGrabberInitied(false)
{
}


ofxFlyCapture::~ofxFlyCapture()
{
}

vector<ofVideoDevice> ofxFlyCapture::listDevices() const {
	std::vector<ofVideoDevice> devices;

	unsigned int numCameras = 0;
	BusManager busMgr;
	auto error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		ofLogError() << error.GetDescription();
		return devices;
	}

	for (auto i = 0u; i < numCameras; i++)
	{
		PGRGuid guid;
		error = busMgr.GetCameraFromIndex(i, &guid);
		if (error != PGRERROR_OK)
		{
			ofLogError() << error.GetDescription();
			break;
		}
		
		Camera c;
		if (c.Connect(&guid) == PGRERROR_OK) {
			CameraInfo ci;
			c.GetCameraInfo(&ci);
			ofVideoDevice d;
			d.id = i;
			d.deviceName = ci.modelName;
			d.hardwareName = ci.modelName;
			d.serialID = ofToString(ci.serialNumber);
			d.bAvailable = true;
			// todo: fill formats property

			devices.push_back(d);
			c.Disconnect();
		}
	}

	return devices;
}

std::tuple<int, int, ofPixelFormat> parseVideoMode(VideoMode vm) {
	switch (vm) {
	case VIDEOMODE_640x480RGB:
		return make_tuple<int, int, ofPixelFormat>(640, 480, OF_PIXELS_RGB);
	case VIDEOMODE_640x480Y8:
		return make_tuple<int, int, ofPixelFormat>(640, 480, OF_PIXELS_GRAY);
	case VIDEOMODE_800x600RGB:
		return make_tuple<int, int, ofPixelFormat>(800, 600, OF_PIXELS_RGB);
	case VIDEOMODE_800x600Y8:
		return make_tuple<int, int, ofPixelFormat>(800, 600, OF_PIXELS_GRAY);
	case VIDEOMODE_1024x768RGB:
		return make_tuple<int, int, ofPixelFormat>(1024, 768, OF_PIXELS_RGB);
	case VIDEOMODE_1024x768Y8:
		return make_tuple<int, int, ofPixelFormat>(1024, 768, OF_PIXELS_GRAY);
	case VIDEOMODE_1280x960RGB:
		return make_tuple<int, int, ofPixelFormat>(1280, 960, OF_PIXELS_RGB);
	case VIDEOMODE_1280x960Y8:
		return make_tuple<int, int, ofPixelFormat>(1280, 960, OF_PIXELS_GRAY);
	case VIDEOMODE_1600x1200RGB:
		return make_tuple<int, int, ofPixelFormat>(1600, 1200, OF_PIXELS_RGB);
	case VIDEOMODE_1600x1200Y8:
		return make_tuple<int, int, ofPixelFormat>(1600, 1200, OF_PIXELS_GRAY);
	default:
		return make_tuple<int, int, ofPixelFormat>(0, 0, OF_PIXELS_GRAY);
	}
}

bool ofxFlyCapture::setup(int w, int h) {
	camera = make_shared<Camera>();
	if (bChooseDevice) {
		BusManager busMgr;
		PGRGuid guid;
		busMgr.GetCameraFromIndex(deviceID, &guid);
		if (camera->Connect(&guid) != PGRERROR_OK) {
			return false;
		}
	}
	else {
		if (camera->Connect() != PGRERROR_OK) {
			return false;
		}
	}

	// todo set frame size to desired (w, h)
	VideoMode vm;
	FrameRate fr;
	camera->GetVideoModeAndFrameRate(&vm, &fr);
	if (vm == VIDEOMODE_FORMAT7 && fr == FRAMERATE_FORMAT7) {
		Format7ImageSettings fmt7is;
		unsigned int ps = 0;
		float pct = 0;
		camera->GetFormat7Configuration(&fmt7is, &ps, &pct);
		width = fmt7is.width;
		height = fmt7is.height;
		switch (fmt7is.pixelFormat) {
		case PIXEL_FORMAT_MONO8:
			pixelFormat = OF_PIXELS_GRAY;
			break;
		case PIXEL_FORMAT_RGB8:
			pixelFormat = OF_PIXELS_RGB;
			break;

		default:
			ofLogWarning() << "unknown pixel format. " << fmt7is.pixelFormat;
		}
	}
	else {
		auto fmt = parseVideoMode(vm);
		width = std::get<0>(fmt);
		height = std::get<1>(fmt);
		pixelFormat = std::get<2>(fmt);
	}

	bGrabberInitied = camera->StartCapture() == PGRERROR_OK;
	return bGrabberInitied;
}

float ofxFlyCapture::getHeight() const {
	return height;
}

float ofxFlyCapture::getWidth() const {
	return width;
}

void ofxFlyCapture::setDeviceID(int deviceID) {
	this->deviceID = deviceID;
	bChooseDevice = true;
}

bool ofxFlyCapture::isFrameNew() const {
	return bIsFrameNew;
}

void ofxFlyCapture::close() {
	if (camera != nullptr) {
		camera->Disconnect();
		camera.reset();
		bGrabberInitied = false;
	}
}

bool ofxFlyCapture::isInitialized() const {
	return bGrabberInitied;
}

bool ofxFlyCapture::setPixelFormat(ofPixelFormat pixelFormat) {
	switch (pixelFormat)
	{
	case OF_PIXELS_GRAY:
	case OF_PIXELS_RGB:
		return this->pixelFormat == pixelFormat;
		
	default:
		ofLogWarning() << "setPixelFormat(): requested pixel format not supported";
		return false;
	}
}

ofPixelFormat ofxFlyCapture::getPixelFormat() const {
	return pixelFormat;
}

ofPixels & ofxFlyCapture::getPixels() {
	return pixels;
}

const ofPixels & ofxFlyCapture::getPixels() const {
	return pixels;
}

void ofxFlyCapture::update() {
	if (!camera) {
		return;
	}

	FlyCapture2::Image tmpBuffer;
	Error e = camera->RetrieveBuffer(&tmpBuffer);
	if (e != PGRERROR_OK) {
		ofLogError() << e.GetDescription();
		bIsFrameNew = false;
	}
	else {
		if (!pixels.isAllocated()
			|| pixels.getWidth() != width
			|| pixels.getHeight() != height
			|| pixels.getPixelFormat() != pixelFormat) {
			pixels.allocate(width, height, pixelFormat);
		}
		pixels.setFromPixels(tmpBuffer.GetData(), width, height, pixelFormat);
		bIsFrameNew = true;
	}
}

ofxFlyCaptureGrabber::ofxFlyCaptureGrabber() {
	this->setGrabber(make_shared<ofxFlyCapture>());
}