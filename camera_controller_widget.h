// Camera Controller Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Jul 31 11:53:38 CDT 2008

#ifndef CAMERA_CONTROLLER_WIDGET_H
#define CAMERA_CONTROLLER_WIDGET_H

#include <QAction>
#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QMessageBox>
#include <QMetaObject>
#include <QToolBar>
#include <QWidget>
#include <stdexcept>
#include "camera_dialog.h"
#include "camera_settings_dialog.h"
#include "camera_controller.h"

namespace flying_dragon
{

/// @brief Widget for operating a camera
class CameraControllerWidget : public QToolBar
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param camera_controller The camera controller
    CameraControllerWidget (QWidget *parent,
        CameraController *camera_controller)
        : QToolBar (parent)
        , camera_controller_ (camera_controller)
        , camera_dialog_ (0)
        , camera_settings_dialog_ (0)
        , device_name_ (0)
        , start_action_ (0)
        , view_action_ (0)
        , settings_action_ (0)
    {
        assert (camera_controller_);

        setObjectName (QString ("CameraControllerWidget"));

        camera_dialog_ =
            new CameraDialog (this);

        connect (camera_controller_, SIGNAL(NewIcon(const QImage &)),
            this, SLOT(NewIcon(const QImage &)));
        connect (camera_controller_, SIGNAL(NewFrame(const QImage &)),
            camera_dialog_, SLOT(NewFrame(const QImage &)));
        connect (camera_dialog_, SIGNAL(Close()),
            this, SLOT(CloseCameraDialog()));
        SetupUI ();
    }

    private slots:
    void on_StartAction_toggled (bool checked)
    {
        if (checked)
        {
            try
            {
            DoStart ();
            }
            catch (jsp::CameraException &e)
            {
            QMessageBox::critical (this, "Camera error", e.what ());
            start_action_->toggle ();
            }
        }
        else
        {
            DoStop ();
        }
    }
    void on_ViewAction_toggled (bool checked)
    {
        if (checked)
        {
            camera_dialog_->show ();
        }
        else
        {
            camera_dialog_->hide ();
        }
    }
    void on_SettingsAction_toggled (bool checked)
    {
        if (checked)
        {
            if (camera_settings_dialog_)
                delete camera_settings_dialog_;
            camera_settings_dialog_ =
                new CameraSettingsDialog (this);
            camera_settings_dialog_->SetupUI (camera_controller_->GetCamera ());
            camera_settings_dialog_->show ();
            connect (camera_settings_dialog_, SIGNAL(Close()),
                this, SLOT(CloseCameraSettingsDialog()));
        }
        else
        {
            camera_settings_dialog_->hide ();
        }
    }
    void NewIcon (const QImage &image)
    {
        QPixmap pixmap;
        pixmap = pixmap.fromImage (image);
        QIcon icon (pixmap);
        view_action_->setIcon (icon);
    }
    void CloseCameraDialog ()
    {
        view_action_->toggle ();
    }
    void CloseCameraSettingsDialog ()
    {
        settings_action_->toggle ();
    }

    private:
    void SetupUI ()
    {
        setFloatable (true);
        setMovable (true);

        device_name_ = new QComboBox (this);
        device_name_->setObjectName(QString("DeviceName"));
        device_name_->setEditable (false);
        device_name_->setToolTip ("Choose a camera device");
        device_name_->addItem (QString ("/dev/video"));
        device_name_->addItem (QString ("/dev/video0"));
        device_name_->addItem (QString ("/dev/video1"));
        device_name_->addItem (QString ("/dev/video2"));
        device_name_->addItem (QString ("/dev/video3"));
        addWidget (device_name_);

        QIcon start_icon;
        start_icon = QIcon (QString::fromUtf8 (":/flying_dragon/images/start_capture.png"));
        start_action_ = new QAction (start_icon, "Start", this);
        start_action_->setObjectName(QString("StartAction"));
        start_action_->setCheckable (true);
        start_action_->setToolTip ("Start/Stop the camera");
        start_action_->setShortcut(QKeySequence ("Ctrl+S"));
        addAction (start_action_);

        QIcon view_icon;
        view_icon = QIcon (QString::fromUtf8 (":/flying_dragon/images/camera_view.png"));
        view_action_ = new QAction (view_icon, "View", this);
        view_action_->setObjectName(QString("ViewAction"));
        view_action_->setCheckable (true);
        view_action_->setToolTip ("Show/Hide camera output");
        view_action_->setShortcut(QKeySequence ("Ctrl+V"));
        addAction (view_action_);

        QIcon settings_icon;
        settings_icon = QIcon (QString::fromUtf8 (":/flying_dragon/images/camera_settings.png"));
        settings_action_ = new QAction (settings_icon, "Settings", this);
        settings_action_->setObjectName(QString("SettingsAction"));
        settings_action_->setCheckable (true);
        settings_action_->setToolTip ("Show/Hide camera settings");
        settings_action_->setShortcut(QKeySequence ("Ctrl+E"));
        addAction (settings_action_);

        DoStop ();
        QMetaObject::connectSlotsByName (this);
    }
    void DoStart ()
    {
        camera_controller_->Open (device_name_->currentText ());
        camera_controller_->StartCapture (10);
        device_name_->setEnabled (false);
        start_action_->setEnabled (true);
        view_action_->setEnabled (true);
        settings_action_->setEnabled (true);
    }
    void DoStop ()
    {
        camera_controller_->StopCapture ();
        camera_controller_->Close ();
        device_name_->setEnabled (true);
        start_action_->setEnabled (true);
        view_action_->setEnabled (true);
        settings_action_->setEnabled (false);
        QIcon view_icon;
        view_icon = QIcon (QString::fromUtf8 (":/flying_dragon/images/camera_view.png"));
        if (settings_action_->isChecked ())
            settings_action_->setChecked (false);
    }

    CameraController *camera_controller_;
    CameraDialog *camera_dialog_;
    CameraSettingsDialog *camera_settings_dialog_;
    QComboBox *device_name_;
    QAction *start_action_;
    QAction *view_action_;
    QAction *settings_action_;
};

} // namespace flying_dragon

#endif // CAMERA_CONTROLLER_WIDGET_H
