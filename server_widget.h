// Server Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 12:57:59 CDT 2008

#ifndef SERVER_WIDGET_H
#define SERVER_WIDGET_H

#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QToolBar>
#include <stdexcept>
#include "server.h"

namespace flying_dragon
{

/// @brief Widget that controls a server
///
/// The widget contains a pushbutton that starts the server
/// listening when pressed.
class ServerWidget : public QToolBar
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param server Server widget
    ServerWidget (QWidget *parent,
            Server *server)
        : QToolBar (parent)
        , server_ (server)
        , listen_checkbox_ (0)
        , port_ (0)
    {
        setObjectName (QString ("ServerWidget"));
        SetupUI ();
    }

    private slots:
    void on_ListenCheckBox_stateChanged (int checked)
    {
        if (checked)
        {
            server_->Listen (port_->text ().toInt ());
            port_->setEnabled (false);
        }
        else
        {
            server_->Close ();
            port_->setEnabled (true);
        }
    }

    private:
    void SetupUI ()
    {
        listen_checkbox_ = new QCheckBox ("Server Enabled", this);
        listen_checkbox_->setObjectName(QString("ListenCheckBox"));
        listen_checkbox_->setEnabled (true);
        listen_checkbox_->setToolTip ("Listen for an incoming connection");
        listen_checkbox_->setShortcut(QKeySequence ("Ctrl+L"));
        addWidget (listen_checkbox_);
        port_ = new QLineEdit ("Port", this);
        port_->setText ("7480");
        port_->setValidator (new QIntValidator (this));
        port_->setToolTip ("Server port");
        port_->setVisible (false);
        QMetaObject::connectSlotsByName (this);
    }
    Server *server_;
    QCheckBox *listen_checkbox_;
    QLineEdit *port_;
};

} // namespace flying_dragon

#endif // SERVER_WIDGET_H
