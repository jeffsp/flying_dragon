// Client Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Jun 12 10:00:46 CDT 2008

#ifndef CLIENT_H
#define CLIENT_H

#include "connection_manager.h"
#include <QTcpSocket>
#include <QMessageBox>

namespace flying_dragon
{

/// @brief Connect a client to a server
class Client : public QObject
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param connection_manager The connection manager
    Client (ConnectionManager &connection_manager)
        : connection_manager_ (connection_manager)
    { }

    public slots:
    /// @brief Connect the client to a server
    /// @param server The server name
    /// @param port The socket port number
    void Connect (const QString &server, int port)
    {
        qDebug() << "client: connecting to" << server
            << "on port" << port;
        connection_manager_.ConnectToServer (server, port);
    }

    private:
    ConnectionManager &connection_manager_;
};

} // namespace flying_dragon

#endif // CLIENT_H
