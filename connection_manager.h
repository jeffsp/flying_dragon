// Connection Manager Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Jul 10 10:24:37 CDT 2008

#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "connection.h"
#include <QIcon>
//#include <QMap>
#include <QHash>
#include <QObject>
#include <QTcpSocket>
#include <cassert>

namespace flying_dragon
{

/// @brief Manage a list of connections
class ConnectionManager : public QObject
{
    Q_OBJECT

    signals:
    /// @brief A connection has been added
    /// @param connection The added connection
    void Added (const Connection *connection);
    /// @brief A connection has been removed
    /// @param id The removed connection's id
    void Removed (unsigned id);

    public:
    /// @brief Constructor
    ConnectionManager ()
        : current_connection_id_ (0)
    {
    }
    /// @brief Get a new connection ID
    /// @return The ID
    ///
    /// A new ID is generated each time you call this
    /// function.
    unsigned GetNewID ()
    {
        return current_connection_id_++;
    }
    /// @brief Create a new connection to a server
    /// @param server Name of the server
    /// @param port The socket port number
    void ConnectToServer (const QString &server, int port)
    {
        ClientConnection *connection = new ClientConnection (this, GetNewID (), server);
        connection->ConnectToServer (server, port);
        Add (connection);
    }
    /// @brief Accept a connection from a client
    /// @param socket_descriptor Descriptor of new socket
    void AcceptConnection (int socket_descriptor)
    {
        ServerConnection *connection =
            new ServerConnection (this, GetNewID (), socket_descriptor);
        Add (connection);
    }
    /// @brief Add a connection
    /// @param connection The connection to add
    void Add (Connection *connection)
    {
        assert (connection);
        connections_[connection->GetID ()] = connection;
        emit Added (connection);
    }
    /// @brief Remove a connection
    /// @param connection The connection to add
    /// @param id The id of the connection to remove
    void Remove (unsigned id)
    {
        qDebug() << "removing " << id;
        Connection *connection = connections_[id];
        assert (connection);
        disconnect (connection, 0, 0, 0);
        connection->deleteLater ();
        connections_.remove (id);
        emit Removed (id);
    }
    /// @brief Get the total number of connections
    int Total () const
    {
        return connections_.size ();
    }
    /// @brief Find a connection given an id
    /// @param id Connection id
    Connection *Find (unsigned id)
    {
        Connection *connection = connections_[id];
        assert (connection);
        return connection;
    }

    public slots:
    /// @brief A new icon is ready to send
    void NewIcon (const QImage &icon)
    {
        Connection *c;
        foreach (c, connections_)
            if (c->GetState () == Connection::StateConnected)
                c->SendIcon (icon);
    }
    /// @brief A new frame is ready to send
    void NewFrame (const QImage &frame)
    {
        Connection *c;
        foreach (c, connections_)
            if (c->GetState () == Connection::StateConnected &&
                c->GetStreaming ())
                c->SendFrame (frame);
    }

    private:
    QHash<unsigned, Connection *> connections_;
    unsigned current_connection_id_;
};

} // namespace flying_dragon

#endif // CONNECTION_MANAGER_H
