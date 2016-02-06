// Camera Settings Dialog
//
// Copyright (C) 2008
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 10:57:11 CDT 2008

#ifndef CAMERA_SETTINGS_DIALOG_H
#define CAMERA_SETTINGS_DIALOG_H

#include "camera.h"
#include "camera_setting_widgets.h"
#include "persistent_dialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <vector>

namespace flying_dragon
{

/// @brief Dialog for changing camera settings
///
/// Don't get confused about the naming.  Camera 'settings'
/// are stuff like contrast and brightness.  They are
/// changed using the camera 'controls' in the Camera
/// interface.
class CameraSettingsDialog : public PersistentDialog
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent The parent widget
    CameraSettingsDialog (QWidget *parent = 0)
        : PersistentDialog (parent)
    {
        setObjectName ("CameraSettingsDialog");
        setWindowTitle ("Camera Settings");
    }
    /// @brief Setup the user interface
    /// @param camera Pointer to an open camera object
    void SetupUI (jsp::Camera *camera)
    {
        assert (camera);
        QGridLayout *gridLayout;
        gridLayout = new QGridLayout(this);

        const std::vector<const jsp::CameraControl*> &c = camera->GetControls ();

        QPushButton *default_button = new QPushButton ("Set Defaults", this);
        default_button->setObjectName ("DefaultButton");
        gridLayout->addWidget (default_button, c.size (), 0);

        for (size_t i = 0; i < c.size (); ++i)
        {
            QLabel *label = new QLabel (c[i]->Name ().c_str (), this);
            gridLayout->addWidget (label, i, 0);
            const std::vector<std::string> &names = c[i]->GetMenuNames ();
            QWidget *widget;
            if (!names.empty ())
            {
                widget = new CameraSettingComboBox (this, c[i], camera);
                QObject::connect (widget,
                    SIGNAL(currentIndexChanged(int)),
                    widget,
                    SLOT(on_CameraSettingCombobox_currentIndexChanged(int)));
            }
            else
            {
                widget = new CameraSettingSlider (this, c[i], camera);
                widget->setMinimumSize (200,10);

                QLabel *val = new QLabel ("0", this);
                val->setNum (camera->GetControlValue (c[i]));
                val->setMinimumSize (30,10);
                QObject::connect (widget,
                    SIGNAL(valueChanged (int)),
                    val,
                    SLOT(setNum (int)));
                gridLayout->addWidget (val, i, 2);
                QObject::connect (widget,
                    SIGNAL(valueChanged (int)),
                    widget,
                    SLOT(on_CameraSettingSlider_valueChanged (int)));
            }
            gridLayout->addWidget (widget, i, 1);
            QObject::connect (default_button,
                SIGNAL(clicked ()),
                widget,
                SLOT(on_CameraSetting_setDefault ()));
        }
    }
};

} // namespace flying_dragon

#endif // CAMERA_SETTINGS_DIALOG_H
