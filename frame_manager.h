// Frame Manager Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Fri Jul 18 17:18:40 CDT 2008

#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include "camera_manager.h"
#include "connection_manager.h"
#include <QImage>
#include <QObject>
#include <QTimer>
#include <cassert>

namespace flying_dragon
{

class FrameManager : public QObject
{
    Q_OBJECT

    signals:
    void NewIcon (const QImage *icon);

    public:
    FrameManager (CameraManager *camera_manager)
        : camera_manager_ (camera_manager)
        , icon_ (QImage (32, 32, QImage::Format_RGB32))
    {
        assert (camera_manager_);
    }
    void Start (int msec = 1000)
    {
        connect (&timer_, SIGNAL(timeout()),
            this, SLOT(GetFrame()));
        timer_.start (msec);
    }

    private slots:
    void GetFrame ()
    {
        camera_manager_->GetFrame ();
        QImage frame = QImage (
            camera_manager_->ARGBFrame (),
            static_cast<int> (camera_manager_->FrameWidth ()),
            static_cast<int> (camera_manager_->FrameHeight ()),
            QImage::Format_RGB32);
        icon_ = frame.scaled (32, 32);
        emit NewIcon (&icon_);
    }

    private:
    CameraManager *camera_manager_;
    QTimer timer_;
    QImage icon_;
};

} // namespace flying_dragon

#endif // FRAME_MANAGER_H
