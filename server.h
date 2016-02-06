// Server Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Jun 12 10:00:46 CDT 2008

#ifndef SERVER_H
#define SERVER_H

#include "connection_manager.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QtDebug>
#include <cassert>

namespace flying_dragon
{

/// @brief Accept client connections
class Server : public QTcpServer
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param connection_manager The connection manager
    Server (ConnectionManager &connection_manager)
        : connection_manager_ (connection_manager)
    {
    }

    public slots:
    /// @brief Begin listening for a client
    /// @param port Listen at this port number
    void Listen (int port)
    {
        qDebug() << "server: listening on port" << port;
        bool success = QTcpServer::listen (QHostAddress::Any, (quint16) port);
        if (!success)
            throw ConnectionGeneralException ("server: listen failed");
    }
    /// @brief Stop listening
    void Close ()
    {
        qDebug() << "server: closing";
        close ();
    }


    protected:
    /// @brief QTCPServer override
    /// @param socket_descriptor Descriptor of incoming connection
    ///
    /// Gets called when a client connects
    void incomingConnection (int socket_descriptor)
    {
        qDebug() << "server: incoming connection";
        //qDebug() << "server: socketdescriptor" << socket_descriptor;
        connection_manager_.AcceptConnection (socket_descriptor);
    }

    private:
    ConnectionManager &connection_manager_;
};

} // namespace flying_dragon

#endif // SERVER_H
