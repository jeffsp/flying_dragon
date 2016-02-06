// Test Camera Dialog
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Jun 12 09:59:39 CDT 2008

#include <QApplication>
#include <QObject>
#include "camera_controller.h"
#include "camera_dialog.h"
#include "camera_settings_dialog.h"

using namespace flying_dragon;
using namespace std;

int main (int argc, char **argv)
{
    try
    {
        QApplication app (argc, argv);
        QString device ("/dev/video");

        CameraController camera_controller;
        camera_controller.Open (device);

Ui_Dialog
        CameraDialog camera_dialog;
        camera_dialog.setObjectName ("TestCameraDialog");
        camera_dialog.setWindowTitle ("Test Camera");
        camera_dialog.Resize (camera_controller.GetWidth (),
            camera_controller.GetHeight ());
        camera_dialog.show ();
        camera_dialog.setWindowTitle (device);

        QObject::connect (&camera_controller, SIGNAL(NewFrame(const QImage &)),
            &camera_dialog, SLOT(NewFrame(const QImage &)));

        CameraSettingsDialog camera_settings_dialog (&camera_dialog);
        camera_settings_dialog.setObjectName ("TestCameraSettingsDialog");
        camera_settings_dialog.setWindowTitle ("Test Camera Settings");
        camera_settings_dialog.SetupUI (camera_controller.GetCamera ());
        camera_settings_dialog.show ();

        camera_controller.StartCapture ();

        return app.exec ();
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
