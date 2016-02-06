// Camera Control Widgets
//
// Copyright (C) 2008
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 11:01:22 CDT 2008

#ifndef CAMERA_CONTROL_WIDGETS_H
#define CAMERA_CONTROL_WIDGETS_H

#include "camera.h"
#include "camera_control.h"
#include <QComboBox>
#include <cassert>

namespace flying_dragon
{

/// @brief A camera control slider widget
class CameraControlSlider : public QSlider
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent parent widget
    /// @param control camera control
    /// @param camera the camera
    CameraControlSlider (QWidget *parent,
        const jsp::CameraControl *control,
        jsp::Camera *camera)
        : QSlider (Qt::Horizontal, parent)
        , control_ (control)
        , camera_ (camera)
    {
        assert (control_);
        assert (camera_);
        setMinimum (control_->Min ());
        setMaximum (control_->Max ());
        setSingleStep (control_->Step ());
        setValue (camera_->GetControlValue (control_));
        setObjectName ("camera_control_slider");
    }
    public slots:
    /// @brief Change the control on the camera
    void on_camera_control_valueChanged (int value)
    {
        camera_->SetControlValue (control_, value);
    }
    /// @brief Set the control back to its default value
    void on_camera_control_setDefault ()
    {
        setValue (control_->Default ());
    }
    private:
    const jsp::CameraControl *control_;
    jsp::Camera *camera_;
};

/// @brief A camera control combobox widget
class CameraControlComboBox : public QComboBox
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent parent widget
    /// @param control camera control
    /// @param camera the camera
    CameraControlComboBox (QWidget *parent,
        const jsp::CameraControl *control,
        jsp::Camera *camera)
        : QComboBox (parent)
        , control_ (control)
        , camera_ (camera)
    {
        assert (control_);
        assert (camera_);
        const std::vector<std::string> &names = control_->GetMenuNames ();
        for (size_t j = 0; j < names.size (); ++j)
            addItem (names[j].c_str ());
        setCurrentIndex (camera_->GetControlValue (control_));
        setObjectName ("camera_control_combo_box");
    }
    public slots:
    /// @brief Change the control on the camera
    void on_camera_control_currentIndexChanged (int index)
    {
        camera_->SetControlValue (control_, index);
    }
    /// @brief Set the control back to its default value
    void on_camera_control_setDefault ()
    {
        setCurrentIndex (control_->Default ());
    }
    private:
    const jsp::CameraControl *control_;
    jsp::Camera *camera_;
};

} // namespace flying_dragon

#endif // CAMERA_CONTROL_WIDGETS_H

