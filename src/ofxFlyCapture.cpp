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
		ofLogWarning() << "sorry currently only format7 is supported.";
		width = height = 0;
		pixelFormat = OF_PIXELS_GRAY;
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

bool ofxFlyCapture::isFrameNew() const {
	return bIsFrameNew;
}

void ofxFlyCapture::close() {
	if (camera != nullptr) {
		camera->Disconnect();
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