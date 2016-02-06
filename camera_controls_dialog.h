// Camera Controls Dialog
//
// Copyright (C) 2008
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 10:57:11 CDT 2008

#ifndef CAMERA_CONTROLS_DIALOG_H
#define CAMERA_CONTROLS_DIALOG_H

#include "camera_manager.h"
#include "camera_control_widgets.h"
#include "persistent_dialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <vector>

namespace flying_dragon
{

/// @brief Dialog for video camera controls
class CameraControlsDialog : public PersistentDialog
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param camera The camera object
    CameraControlsDialog (QWidget *parent, CameraManager *camera_manager)
        : PersistentDialog (parent)
        , camera_manager_ (camera_manager)
    {
        assert (camera_manager_);
        setObjectName (QString::fromUtf8 ("CameraControlsDialog"));
        setWindowTitle (QString ("Server Camera Controls"));
    }
    void SetupUI ()
    {
        assert (camera_manager_);
        QGridLayout *gridLayout;
        gridLayout = new QGridLayout(this);

        const std::vector<const jsp::CameraControl*> &c = camera_manager_->Camera ()->GetControls ();

        QPushButton *default_button = new QPushButton ("Set Defaults", this);
        default_button->setObjectName ("camera_control_defaults");
        gridLayout->addWidget (default_button, c.size (), 0);

        for (size_t i = 0; i < c.size (); ++i)
        {
            QLabel *label = new QLabel (c[i]->Name ().c_str (), this);
            gridLayout->addWidget (label, i, 0);
            const std::vector<std::string> &names = c[i]->GetMenuNames ();
            QWidget *widget;
            if (!names.empty ())
            {
                widget = new CameraControlComboBox (this, c[i], camera_manager_->Camera ());
                QObject::connect (widget,
                    SIGNAL(currentIndexChanged(int)),
                    widget,
                    SLOT(on_camera_control_currentIndexChanged(int)));
            }
            else
            {
                widget = new CameraControlSlider (this, c[i], camera_manager_->Camera ());
                widget->setMinimumSize (200,10);

                QLabel *val = new QLabel ("0", this);
                val->setNum (camera_manager_->Camera ()->GetControlValue (c[i]));
                val->setMinimumSize (30,10);
                QObject::connect (widget,
                    SIGNAL(valueChanged (int)),
                    val,
                    SLOT(setNum (int)));
                gridLayout->addWidget (val, i, 2);
                QObject::connect (widget,
                    SIGNAL(valueChanged (int)),
                    widget,
                    SLOT(on_camera_control_valueChanged (int)));
            }
            gridLayout->addWidget (widget, i, 1);
            QObject::connect (default_button,
                SIGNAL(clicked ()),
                widget,
                SLOT(on_camera_control_setDefault ()));
        }
    }
    private:
    CameraManager *camera_manager_;
};

} // namespace flying_dragon

#endif // CAMERA_CONTROLS_DIALOG_H
