// Main Window
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Fri Jun 27 15:16:07 CDT 2008

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "camera_controller_widget.h"
#include "client_widget.h"
#include "connection_manager_widget.h"
#include "server_widget.h"
#include "ui_flying_dragon.h"
#include <QCloseEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QSettings>

namespace flying_dragon
{

const unsigned MAJOR_VERSION = 0;
const unsigned MINOR_VERSION = 4;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    MainWindow (QMainWindow *parent)
        : QMainWindow (parent)
        , settings_ ("flying_dragon", "main")
        , client_ (connection_manager_)
        , server_ (connection_manager_)
    {
        readVersionSettings ();

        camera_controller_widget_ = new CameraControllerWidget (this, &camera_controller_);
        connection_manager_widget_ = new ConnectionManagerWidget (this, &connection_manager_);
        client_widget_ = new ClientWidget (this, &client_);
        server_widget_ = new ServerWidget (this, &server_);
        //QGridLayout *layout = new QGridLayout (this);

        ui_.setupUi(this);
        readMainWindowSettings ();
        addToolBar (Qt::TopToolBarArea, camera_controller_widget_);
        addToolBar (Qt::TopToolBarArea, server_widget_);
        addToolBar (Qt::TopToolBarArea, client_widget_);
        //readDeviceSettings ();
        //layout->addWidget (connection_manager_widget_);
        //setCentralWidget(connection_manager_widget_);
        ui_.gridLayout->addWidget (connection_manager_widget_);

        QObject::connect (&camera_controller_, SIGNAL(NewIcon (const QImage &)),
            &connection_manager_, SLOT(NewIcon (const QImage &)));
        QObject::connect (&camera_controller_, SIGNAL(NewFrame (const QImage &)),
            &connection_manager_, SLOT(NewFrame (const QImage &)));
    }

    protected:
    void closeEvent(QCloseEvent *event)
    {
        writeVersionSettings ();
        writeMainWindowSettings ();
        //writeDeviceSettings ();
        event->accept ();
    }

    public slots:
    void on_actionQuit_triggered ()
    {
        close ();
    }

    private slots:

    private:
    void writeVersionSettings ()
    {
        settings_.beginGroup ("Version");
        settings_.setValue ("major_version", MAJOR_VERSION);
        settings_.setValue ("minor_version", MINOR_VERSION);
        settings_.endGroup ();
    }
    void readVersionSettings ()
    {
        settings_.beginGroup ("Version");
        int major_version = settings_.value ("major_version", -1).toInt ();
        int minor_version = settings_.value ("minor_version", -1).toInt ();
        Q_UNUSED (major_version);
        Q_UNUSED (minor_version);
        settings_.endGroup ();
    }
    void writeMainWindowSettings ()
    {
        settings_.beginGroup ("MainWindow");
        settings_.setValue ("geometry", saveGeometry ());
        settings_.endGroup ();
    }
    void readMainWindowSettings ()
    {
        settings_.beginGroup ("MainWindow");
        restoreGeometry (settings_.value ("geometry").toByteArray());
        settings_.endGroup ();
    }
    /*
    void writeDeviceSettings ()
    {
        settings_.beginGroup ("Camera");
        settings_.setValue ("device_name_", camera_controller_->GetDeviceName ());
        settings_.endGroup ();
    }
    void readDeviceSettings ()
    {
        settings_.beginGroup ("Camera");
        device_name_ = settings_.value ("device_name_", camera_controller_->GetDeviceName ()).toString ();
        settings_.endGroup ();
    }
    */
    Ui::MainWindow ui_;
    QSettings settings_;
    CameraController camera_controller_;
    ConnectionManager connection_manager_;
    Client client_;
    Server server_;
    CameraControllerWidget *camera_controller_widget_;
    ConnectionManagerWidget *connection_manager_widget_;
    ClientWidget *client_widget_;
    ServerWidget *server_widget_;
};

} // namespace flying_dragon

#endif // MAIN_WINDOW_H
