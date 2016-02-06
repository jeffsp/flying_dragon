// Camera Setting Widgets
//
// Copyright (C) 2008
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 11:01:22 CDT 2008

#ifndef CAMERA_SETTING_WIDGETS_H
#define CAMERA_SETTING_WIDGETS_H

#include "camera.h"
#include "camera_control.h"
#include <QComboBox>
#include <cassert>

namespace flying_dragon
{

/// @brief Camera setting slider widget
class CameraSettingSlider : public QSlider
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param control Camera control
    /// @param camera The camera
    CameraSettingSlider (QWidget *parent,
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
        setObjectName ("CameraSettingSlider");
    }
    public slots:
    /// @brief Change the setting on the camera
    void on_CameraSettingSlider_valueChanged (int value)
    {
        camera_->SetControlValue (control_, value);
    }
    /// @brief Change the setting back to its default value
    void on_CameraSetting_setDefault ()
    {
        setValue (control_->Default ());
    }
    private:
    const jsp::CameraControl *control_;
    jsp::Camera *camera_;
};

/// @brief Camera setting combobox widget
class CameraSettingComboBox : public QComboBox
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param control Camera control
    /// @param camera The camera
    CameraSettingComboBox (QWidget *parent,
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
        setObjectName ("CameraSettingCombobox");
    }
    public slots:
    /// @brief Change the setting on the camera
    void on_CameraSettingCombobox_currentIndexChanged (int index)
    {
        camera_->SetControlValue (control_, index);
    }
    /// @brief Change the setting back to its default value
    void on_CameraSetting_setDefault ()
    {
        setCurrentIndex (control_->Default ());
    }
    private:
    const jsp::CameraControl *control_;
    jsp::Camera *camera_;
};

} // namespace flying_dragon

#endif // CAMERA_SETTING_WIDGETS_H
