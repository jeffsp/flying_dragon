// Frame Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Oct 16 15:12:06 CDT 2008

#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <cassert>

namespace flying_dragon
{

/// @brief An image frame
class Frame : public QImage
{
    public:
    Frame ()
    {
    }
    Frame (int width, int height, QImage::Format format)
        : QImage (width, height, format)
    {
    }
    void Encode (const QImage &image, int /*fx*/, int /*fy*/, int /*e2*/)
    {
        *static_cast<QImage *> (this) = image;
    }
    void Encode (const QImage &image)
    {
        *static_cast<QImage *> (this) = image;
    }
    QImage Decode () const
    {
        return *this;
    }
    protected:
    private:
};

} // namespace flying_dragon

#endif // FRAME_H
