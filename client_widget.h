// Client Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 14:38:43 CDT 2008

#ifndef CLIENT_WIDGET_H
#define CLIENT_WIDGET_H

#include <QAction>
#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>
#include <stdexcept>
#include "client.h"

namespace flying_dragon
{

/// @brief Widget that controls a client
///
/// The widget allows you to connect to a server.
class ClientWidget : public QToolBar
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param client Client
    ClientWidget (QWidget *parent,
        Client *client)
        : QToolBar (parent)
        , client_ (client)
        , address_ (0)
        , port_ (0)
        , connect_action_ (0)
    {
        setObjectName (QString ("ClientWidget"));
        SetupUI ();
    }

    private slots:
    void on_ConnectAction_triggered ()
    {
        client_->Connect (address_->text (), port_->text ().toInt ());
    }

    private:
    void SetupUI ()
    {
        QIcon connect_icon;
        connect_icon = QIcon (QString::fromUtf8 (":/flying_dragon/images/new_connection.png"));
        connect_action_ = new QAction (connect_icon, "Connect", this);
        connect_action_->setObjectName(QString("ConnectAction"));
        connect_action_->setCheckable (false);
        connect_action_->setToolTip ("Connect to server");
        connect_action_->setShortcut(QKeySequence ("Ctrl+C"));
        addAction (connect_action_);

        address_ = new QLineEdit (this);
        address_->setObjectName(QString("Address"));
        address_->setText (QString ("localhost"));
        address_->setToolTip ("Server address");
        addWidget (address_);
        addSeparator ();

        //addWidget (new QLabel ("Port", this));
        port_ = new QLineEdit ("Port", this);
        port_->setText ("7480");
        port_->setValidator (new QIntValidator (this));
        port_->setToolTip ("Server port");
        port_->setVisible (false);
        //addWidget (port_);

        QMetaObject::connectSlotsByName (this);
    }
    Client *client_;
    QLineEdit *address_;
    QLineEdit *port_;
    QAction *connect_action_;
};

} // namespace flying_dragon

#endif // CLIENT_WIDGET_H
