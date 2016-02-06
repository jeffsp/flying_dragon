// Connection Manager Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 20:21:27 CDT 2008

#ifndef CONNECTION_MANAGER_WIDGET_H
#define CONNECTION_MANAGER_WIDGET_H

#include "camera_dialog.h"
#include "connection_manager.h"
#include "frame.h"
#include <QAction>
#include <QBrush>
#include <QColor>
#include <QGridLayout>
#include <QObject>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QWidget>
#include <cassert>

namespace flying_dragon
{

/// @brief A widget for displaying connections
class ConnectionManagerWidget : public QWidget
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param connection_manager Connection manager
    ConnectionManagerWidget (QWidget *parent,
        ConnectionManager *connection_manager)
        : QWidget (parent)
        , connection_manager_ (connection_manager)
        , server_connections_ (0)
        , client_connections_ (0)
        , current_streaming_connection_ (0)
        , network_camera_dialog_ ("NetworkCamera", "Network")
    {
        assert (connection_manager_);
        SetupUI ();
        QObject::connect (&network_camera_dialog_, SIGNAL(Close()),
            this, SLOT(CloseNetworkCameraDialog()));
        QObject::connect (&network_camera_dialog_, SIGNAL(NewFixation(int,int,int)),
            this, SLOT(NewFixation(int,int,int)));
    }

    private slots:
    void Added (const Connection *connection)
    {
        //qDebug() << "adding" << c->Address();
        QTreeWidgetItem *item =
            new QTreeWidgetItem (server_connections_);
        QPixmap pixmap (32, 32);
        pixmap.fill (Qt::gray);
        QIcon icon (pixmap);
        item->treeWidget ()->setColumnWidth (1, 32);
        item->treeWidget ()->setIconSize (QSize (32, 32));
        item->setData (0, Qt::UserRole, QVariant (connection->GetID ()));
        item->setText (0, QString::number (connection->GetID ()));
        item->setIcon (1, icon);
        item->setText (2, connection->GetName ());
        QObject::connect (connection, SIGNAL(StateChanged()),
            this, SLOT(StateChanged()));
        QObject::connect (connection, SIGNAL(ReceivedIcon(const QImage &)),
            this, SLOT(ReceivedIcon(const QImage &)));
        QObject::connect (connection, SIGNAL(ReceivedFrame(const Frame &)),
            this, SLOT(ReceivedFrame(const Frame &)));
        server_connections_->addTopLevelItem (item);
        UpdateRowText (connection, item);
        //qDebug() << "total" << server_connections_->count ();
    }
    void Removed (unsigned id)
    {
        const QString id_string = QString::number (id);
        QList<QTreeWidgetItem *> items = server_connections_->findItems (id_string, Qt::MatchExactly);
        assert (items.count () == 1);
        QTreeWidgetItem *item = items[0];
        //assert (index < server_connections_->topLevelItemCount ());
        //qDebug() << item;
        //qDebug() << item->text ();
        server_connections_->removeItemWidget (item, 0);
        int index = server_connections_->indexOfTopLevelItem (item);
        qDebug() << "removing" << index;
        server_connections_->takeTopLevelItem (index);
        //qDebug() << "total" << server_connections_->count ();
    }
    void StateChanged ()
    {
        const Connection *connection = qobject_cast<const Connection *> (QObject::sender ());
        UpdateRowText (connection, FindItem (connection));
    }
    void ReceivedIcon (const QImage &icon)
    {
        const Connection *connection = qobject_cast<const Connection *> (QObject::sender ());
        QTreeWidgetItem *item = FindItem (connection);
        QPixmap pixmap;
        pixmap = pixmap.fromImage (icon);
        QIcon new_icon (pixmap);
        item->setIcon (1, new_icon);
    }
    void ReceivedFrame (const Frame &frame)
    {
        QImage image = frame.Decode ();
        network_camera_dialog_.NewFrame (image);
    }
    void on_ItemWidget_itemClicked (QTreeWidgetItem *item, int column)
    {
        unsigned id = item->data (0, Qt::UserRole).toUInt ();
        Connection *connection = connection_manager_->Find (id);
        assert (connection);
        if (column == 5)
        {
            connection->SetFoveated (
                !connection->GetFoveated ());
            connection->SendFoveateCommand (
                connection->GetFoveated ());
        }
        else
        {
            if (connection == current_streaming_connection_)
                CloseCurrent ();
            else
            {
                CloseCurrent ();
                OpenNew (connection);
            }
        }
    }
    void CloseNetworkCameraDialog ()
    {
        //qDebug() << "closed";
        CloseCurrent ();
    }
    void NewFixation (int x, int y, int e2)
    {
        if (current_streaming_connection_
            && current_streaming_connection_->GetFoveated ())
            current_streaming_connection_->SendFixation (x, y, e2);
    }

    private:
    void SetupUI ()
    {
        QGridLayout *layout = new QGridLayout (this);

        server_connections_ = new QTreeWidget (this);
        server_connections_->setObjectName ("ServerConnections");
        layout->addWidget (server_connections_, 0, 0);
        QStringList server_labels;
        server_labels << "ID";
        server_labels << "Icon";
        server_labels << "Name";
        server_labels << "State";
        server_labels << "Streaming";
        server_labels << "Foveated";
        server_connections_->setHeaderLabels (server_labels);
        server_connections_->setRootIsDecorated (false);
        server_connections_->hideColumn (0);

        client_connections_ = new QTreeWidget (this);
        client_connections_->setObjectName ("ClientConnections");
        layout->addWidget (client_connections_, 0, 1);
        QStringList client_labels;
        client_labels << "ID";
        client_labels << "Icon";
        client_labels << "Name";
        client_labels << "State";
        client_labels << "Streaming";
        client_labels << "Foveated";
        client_connections_->setHeaderLabels (client_labels);
        client_connections_->setRootIsDecorated (false);
        client_connections_->hideColumn (0);

        network_camera_dialog_.Resize (320, 240);

        connect (connection_manager_, SIGNAL(Added(const Connection *)),
            this, SLOT(Added(const Connection *)));
        connect (connection_manager_, SIGNAL(Removed(unsigned)),
            this, SLOT(Removed(unsigned)));
        QMetaObject::connectSlotsByName (this);
    }
    QTreeWidgetItem *FindItem (const Connection *connection)
    {
        const QString &id = QString::number (connection->GetID ());
        // Search servers
        QList<QTreeWidgetItem *> items = server_connections_->findItems (id, Qt::MatchExactly);
        if (items.count () != 0)
        {
            assert (items.count () == 1);
            //qDebug() << "found" << items.count () << "items";
            return items[0];
        }
        // Search clients
        items = client_connections_->findItems (id, Qt::MatchExactly);
        assert (items.count () == 1);
        return items[0];
    }
    void UpdateRowText (const Connection *connection, QTreeWidgetItem *item)
    {
        item->setText (2, connection->GetName ());
        Connection::State state = connection->GetState ();
        switch (state)
        {
            case Connection::StateDisconnected:
            item->setForeground (2, QBrush (QColor (128, 128, 128)));
            item->setText (3, "Disconnected");
            break;
            case Connection::StateConnecting:
            item->setForeground (2, QBrush (QColor (128, 128, 0)));
            item->setText (3, "Connecting");
            break;
            case Connection::StateHandshaking:
            item->setForeground (2, QBrush (QColor (0, 128, 0)));
            item->setText (3, "Handshaking");
            break;
            case Connection::StateConnected:
            item->setForeground (2, QBrush (QColor (0, 0, 0)));
            item->setText (3, "Connected");
            break;
            case Connection::StateMax:
            default:
            throw ConnectionGeneralException ("invalid state");
        }
        if (connection->GetStreaming ())
        {
            item->setText (4, "ON");
            item->setCheckState (4, Qt::Checked);
        }
        else
        {
            item->setText (4, "OFF");
            item->setCheckState (4, Qt::Unchecked);
        }
        if (connection->GetFoveated ())
        {
            item->setText (5, "ON");
            item->setCheckState (5, Qt::Checked);
        }
        else
        {
            item->setText (5, "OFF");
            item->setCheckState (5, Qt::Unchecked);
        }
    }
    void CloseCurrent ()
    {
        if (current_streaming_connection_)
        {
            current_streaming_connection_->SetStreaming (false);
            current_streaming_connection_->SendStreamCommand (false);
            current_streaming_connection_ = 0;
            network_camera_dialog_.hide ();
        }
    }
    void OpenNew (Connection *connection)
    {
        current_streaming_connection_ = connection;
        current_streaming_connection_->SetStreaming (true);
        current_streaming_connection_->SendStreamCommand (true);
        network_camera_dialog_.setWindowTitle (current_streaming_connection_->GetName ());
        network_camera_dialog_.setObjectName (current_streaming_connection_->GetName ());
        network_camera_dialog_.show ();
    }
    ConnectionManager *connection_manager_;
    QTreeWidget *server_connections_;
    QTreeWidget *client_connections_;
    Connection *current_streaming_connection_;
    CameraDialog network_camera_dialog_;
};

} // namespace flying_dragon

#endif // CONNECTION_MANAGER_WIDGET_H
