// Camera Controller
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 09:20:09 CDT 2008

#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "camera.h"
#include "colorspace.h"
#include "raster.h"
#include <string>
#include <QDebug>
#include <QImage>
#include <QObject>
#include <QString>
#include <QTimer>

namespace flying_dragon
{

/// @brief High level camera controller
///
/// Enhances a Camera interface by adding signals/slots and
/// better error handling.
class CameraController : public QObject
{
    Q_OBJECT

    public slots:
    /// @brief Open a camera
    /// @param device_name The device name
    ///
    /// Open and setup a camera device.  For example
    /// "/dev/video".
    void Open (const QString &device_name)
    {
        //camera_.SetLog (std::clog);
        camera_.Open (device_name.toStdString ().c_str ());
        camera_.InitBuffers (WIDTH_HINT, HEIGHT_HINT);
        unsigned w, h;
        jsp::Camera::PixelFormat p;
        jsp::Camera::Colorspace c;
        camera_.GetCaptureFormat (w, h, p, c);
        if (p != jsp::Camera::PixelFormatYUYV)
            throw jsp::CameraGeneralException ("Unsupported pixel format");
        if (c != jsp::Camera::ColorspaceSRGB)
            throw jsp::CameraGeneralException ("Unsupported colorspace");
        Resize (w, h);
        QObject::connect (&timer_, SIGNAL(timeout()),
            this, SLOT(GetFrame()));
    }
    /// @brief Close a camera
    void Close ()
    {
        camera_.Close ();
    }
    /// @brief Start capturing
    /// @param timer_msec Min msec between frames
    ///
    /// The amount of time required between frames depends
    /// on the device and how it is set up.
    void StartCapture (int timer_msec = 10)
    {
        camera_.StartCapture ();
        timer_.start (timer_msec);
    }
    /// @brief Stop capturing
    void StopCapture ()
    {
        timer_.stop ();
        camera_.StopCapture ();
    }

    signals:
    /// @brief A new frame is available
    /// @param frame The frame
    ///
    /// The referenced frame is temporary storage.  You have
    /// a limited amount of time to process it.
    void NewFrame (const QImage &frame);
    /// @brief A new icon is available
    /// @param icon The icon
    ///
    /// The referenced icon is temporary storage.  You have
    /// a limited amount of time to process it.
    void NewIcon (const QImage &icon);

    public:
    /// @brief Get a pointer to the camera
    jsp::Camera *GetCamera ()
    { return &camera_; }
    /// @brief Get the frame width
    ///
    /// You must call Open() before you call this function
    size_t GetWidth () const
    { return y_frame_.cols (); }
    /// @brief Get the frame height
    ///
    /// You must call Open() before you call this function
    size_t GetHeight () const
    { return y_frame_.rows (); }

    private slots:
    void GetFrame ()
    {
        const unsigned char *frame =
            static_cast<const unsigned char *> (camera_.GetFrame (TIMEOUT_SECS));
        // Convert it to planar yuv
        jsp::YUYV2YV12 (frame, y_frame_, u_frame_, v_frame_);
        // Convert it to ARGB
        jsp::YV122ARGB (y_frame_, u_frame_, v_frame_, argb_frame_);
        // Convert it to a QImage
        frame_ = QImage (
            &argb_frame_[0],
            y_frame_.cols (),
            y_frame_.rows (),
            QImage::Format_RGB32);
        // Scale it down to an icon
        icon_ = frame_.scaled (64, 64);
        // Send signals
        emit NewIcon (icon_);
        emit NewFrame (frame_);
    }

    private:
    /// @brief Resize the internal frame buffers
    void Resize (unsigned w, unsigned h)
    {
        y_frame_.resize (h, w, 0xff);
        u_frame_.resize (h / 2, w / 2, 0xff);
        v_frame_.resize (h / 2, w / 2, 0xff);
        argb_frame_.resize (h, w * 4, 0xff);
    }

    jsp::Camera camera_;
    static const unsigned WIDTH_HINT = 320;
    static const unsigned HEIGHT_HINT = 240;
    static const unsigned TIMEOUT_SECS = 3;
    jsp::raster<unsigned char> y_frame_;
    jsp::raster<unsigned char> u_frame_;
    jsp::raster<unsigned char> v_frame_;
    jsp::raster<unsigned char> argb_frame_;
    QTimer timer_;
    QImage icon_;
    QImage frame_;
};

} // namespace flying_dragon

#endif // CAMERA_CONTROLLER_H
