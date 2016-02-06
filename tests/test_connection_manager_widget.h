// Test Connection Manager Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 20:52:04 CDT 2008

#include "connection_manager_widget.h"
#include <QDialog>
#include <QObject>
#include <QTimer>
#include <QVBoxLayout>
#include <iostream>
#include <vector>

using namespace flying_dragon;
using namespace std;

class RandomClientConnection : public ClientConnection
{
    Q_OBJECT

    public:
    RandomClientConnection (QObject *parent)
        : ClientConnection (parent, rand (), QString::number (rand ()))
    {
        QTimer::singleShot (rand () % 100, this, SLOT(SetRandomState()));
    }
    public slots:
    void SetRandomState ()
    {
        ChangeState (static_cast<Connection::State> (rand () % Connection::StateMax));
        QTimer::singleShot (rand () % 1000, this, SLOT(SetRandomState()));
        QImage image (32, 32, QImage::Format_RGB32);
        image.fill (rand ());
        emit ReceivedIcon (image);
    }
    private:
};

class Dialog : public QDialog
{
    Q_OBJECT

    public:
    Dialog (ConnectionManager *connection_manager)
        : QDialog (0)
        , connection_manager_ (connection_manager)
    {
        QVBoxLayout *layout = new QVBoxLayout (this);

        ConnectionManagerWidget *connection_manager_widget =
            new ConnectionManagerWidget (this, connection_manager);
        layout->addWidget (connection_manager_widget);
    }
    public slots:
    void Test1 ()
    {
        Connection *connection = new RandomClientConnection (this);
        connection_manager_->Add (connection);
        qDebug() << "adding " << connection->GetID ();
        ids.push_back (connection->GetID ());
        //qDebug() << connection_manager_->Total ();
    }
    void Test2 ()
    {
        assert (!ids.empty ());
        connection_manager_->Remove (ids.back ());
        ids.pop_back ();
        //qDebug() << connection_manager_->Total ();
    }
    private:
    ConnectionManager *connection_manager_;
    std::vector<unsigned> ids;
};
