// Camera Manager
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 09:20:09 CDT 2008

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "camera.h"
#include "colorspace.h"
#include "raster.h"
#include <string>
#include <QTime>

namespace flying_dragon
{

class CameraManager
{
    public:
    void Open (const std::string &device_name)
    {
        //camera_.SetLog (std::clog);
        camera_.Open (device_name.c_str ());
        camera_.InitBuffers (WIDTH_HINT, HEIGHT_HINT);
        unsigned w, h;
        jsp::Camera::PixelFormat p;
        jsp::Camera::Colorspace c;
        camera_.GetCaptureFormat (w, h, p, c);
        if (p != jsp::Camera::PixelFormatYUYV)
            throw std::runtime_error ("Unsupported pixel format");
        if (c != jsp::Camera::ColorspaceSRGB)
            throw std::runtime_error ("Unsupported colorspace");
        Resize (w, h);
        camera_.StartCapture ();
        // Sometimes you get junk in the first few
        // frames...
        QTime t;
        t.start ();
        while (t.elapsed () < 250)
            camera_.GetFrame (TIMEOUT);
    }
    jsp::Camera *Camera ()
    { return &camera_; }
    size_t FrameWidth () const
    { return y_frame_.cols (); }
    size_t FrameHeight () const
    { return y_frame_.rows (); }
    void GetFrame ()
    {
        const unsigned char *frame =
            static_cast<const unsigned char *> (camera_.GetFrame (TIMEOUT));
        // Convert it to planar yuv
        jsp::YUYV2YV12 (frame, y_frame_, u_frame_, v_frame_);
        // Convert it to ARGB
        jsp::YV122ARGB (y_frame_, u_frame_, v_frame_, argb_frame_);
    }
    const unsigned char *YFrame () const
    { return &y_frame_[0]; }
    const unsigned char *UFrame () const
    { return &u_frame_[0]; }
    const unsigned char *VFrame () const
    { return &v_frame_[0]; }
    const unsigned char *ARGBFrame () const
    { return &argb_frame_[0]; }
    private:
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
    static const unsigned TIMEOUT = 3;
    jsp::raster<unsigned char> y_frame_;
    jsp::raster<unsigned char> u_frame_;
    jsp::raster<unsigned char> v_frame_;
    jsp::raster<unsigned char> argb_frame_;
};

} // namespace flying_dragon

#endif // CAMERA_MANAGER_H
