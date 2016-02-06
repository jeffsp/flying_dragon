// Test Camera Controller Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Jun 12 09:59:39 CDT 2008

#include <QApplication>
#include <QGridLayout>
#include <QObject>
#include "exception_enabled_app.h"
#include "camera_controller_widget.h"
#include "persistent_dialog.h"

using namespace flying_dragon;
using namespace std;

class Dialog : public PersistentDialog
{
    public:
    Dialog (CameraController *camera_controller)
        : PersistentDialog (0)
    {
        QGridLayout *layout = new QGridLayout (this);
        CameraControllerWidget *camera_controller_widget =
            new CameraControllerWidget (this, camera_controller);
        layout->addWidget (camera_controller_widget);
    }
};

int main (int argc, char **argv)
{
    try
    {
        ExceptionEnabledApplication app (argc, argv);
        CameraController camera_controller;
        Dialog dlg (&camera_controller);
        dlg.setWindowTitle (QString ("Camera Controller"));
        dlg.show ();

        return app.exec ();
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
