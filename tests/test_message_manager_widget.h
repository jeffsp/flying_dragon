// Test Message Manager Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 20:52:04 CDT 2008

#include "message_manager_widget.h"
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <QVBoxLayout>
#include <iostream>

using namespace flying_dragon;
using namespace std;

class Dialog : public QDialog
{
    Q_OBJECT

    public:
    Dialog ()
        : QDialog (0)
    {
        server_ =
            new QTcpServer (this);
        client_socket_ =
            new QTcpSocket (this);
        client_message_manager_ =
            new MessageManager (client_socket_);
        MessageManagerWidget *message_manager_widget =
            new MessageManagerWidget (this, client_message_manager_);
        layout_ = new QVBoxLayout (this);
        layout_->addWidget (new QLabel ("Client", this));
        layout_->addWidget (message_manager_widget);

        connect (server_, SIGNAL(newConnection()),
            this, SLOT(ServerConnected()));
        connect (client_socket_, SIGNAL(connected()),
            this, SLOT(ClientConnected()));
        connect (client_message_manager_, SIGNAL(Error(QString)),
            this, SLOT(Error(QString)));
        server_->listen ();
        client_socket_->connectToHost (server_->serverAddress (), server_->serverPort ());
        timer_.setInterval (1000);
        timer_.start ();
        QObject::connect (&timer_, SIGNAL(timeout()),
            this, SLOT(Test()));
    }
    public slots:
    void ServerConnected ()
    {
        server_socket_ = server_->nextPendingConnection ();
        server_message_manager_ =
            new MessageManager (server_socket_);
        connect (server_message_manager_, SIGNAL(Error(QString)),
            this, SLOT(Error(QString)));
        MessageManagerWidget *message_manager_widget =
            new MessageManagerWidget (this, server_message_manager_);
        layout_->addWidget (new QLabel ("Server", this));
        layout_->addWidget (message_manager_widget);
        server_message_manager_->SendHandshake ();
    }
    void ClientConnected ()
    {
        client_message_manager_->SendHandshake ();
    }
    void Test ()
    {
        QImage img (64, 64, QImage::Format_RGB32);
        const int n1 = rand () % 10;
        for (int i = 0; i < n1; ++i)
            client_message_manager_->SendIcon (img);
        const int n2 = rand () % 100;
        for (int i = 0; i < n2; ++i)
            server_message_manager_->SendIcon (img);
    }
    void Error (QString error)
    {
        QMessageBox::critical (this, "error", error);
    }
    private:
    QVBoxLayout *layout_;
    QTcpServer *server_;
    QTcpSocket *client_socket_;
    QTcpSocket *server_socket_;
    MessageManager *client_message_manager_;
    MessageManager *server_message_manager_;
    QTimer timer_;
};
