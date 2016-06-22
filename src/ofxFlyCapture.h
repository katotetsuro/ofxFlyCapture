#pragma once

#include "ofVideoGrabber.h"

namespace FlyCapture2 {
	class Camera;
}

class ofxFlyCapture : public ofBaseVideoGrabber
{
public:
	ofxFlyCapture();
	virtual ~ofxFlyCapture();

	// from ofBaseVideoGrabber
	/// \brief Get a list of available video grabber devices.
	/// \returns a std::vector of ofVideoDevice objects.
	vector<ofVideoDevice>	listDevices() const;

	/// \brief Set up the grabber with the requested width and height.
	///
	/// Some video grabbers may take the requested width and height as
	/// a hint and choose the closest dimensions to those requested.
	/// Users can check the actual width and height by calling getWidth() and
	/// getHeight() respectively after a successful setup.
	///
	/// \param w the requested width.
	/// \param h the requested height.
	/// \returns true if the video grabber was set up successfully.
	bool setup(int w, int h);

	/// \brief Get the video grabber's height.
	/// \returns the video grabber's height.
	float getHeight() const;

	/// \brief Get the video grabber's width.
	/// \returns the video grabber's width.
	float getWidth() const;

	/// \brief Set the video grabber's device ID.
	///
	/// In most cases, a user can choose a specific grabber source by ID.  This
	/// device ID information should be available to the user via the
	/// listDevices() method.
	///
	/// \param deviceID The device ID provided by listDevices().
	void setDeviceID(int deviceID);

	// from ofBaseVideo
	/// \returns true if the pixel data was updated since the last call to update().
	bool isFrameNew() const;

	/// \brief Close the video source.
	void close();

	/// \brief Determine if the video source is initialized.
	///
	/// Video sources such as cameras are often initialized with a
	/// setup() method.  Video sources such as movie players are often
	/// initialized with a load() method.
	///
	/// \returns true if the video source is initialized.
	bool isInitialized() const;

	/// \brief Set the requested ofPixelFormat.
	/// \param pixelFormat the requested ofPixelFormat.
	/// \returns true if the format was successfully changed.
	bool setPixelFormat(ofPixelFormat pixelFormat);

	/// \returns the current ofPixelFormat.
	ofPixelFormat getPixelFormat() const;

	// from ofBaseHasPixels
	/// \brief Get a reference to the underlying ofPixels.
	/// \returns a reference the underlying ofPixels.
	ofPixels & getPixels();

	/// \brief Get a const reference to the underlying ofPixels.
	/// \returns a const reference the underlying ofPixels.
	const ofPixels & getPixels() const;

	// from ofBaseUpdates
	/// \brief Update the object's state.
	void update();

private:
	bool bChooseDevice;
	int deviceID;
	shared_ptr<class FlyCapture2::Camera> camera;
	int width;
	int height;
	ofPixelFormat pixelFormat;
	bool bIsFrameNew;
	bool bGrabberInitied;
	ofPixels pixels;
};

class ofxFlyCaptureGrabber : public ofVideoGrabber {
public:
	ofxFlyCaptureGrabber();
};