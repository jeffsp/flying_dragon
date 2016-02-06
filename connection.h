// Connection Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Jul 14 13:55:49 CDT 2008

#ifndef CONNECTION_H
#define CONNECTION_H

#include "connection_exceptions.h"
#include "frame.h"
#include "message_manager.h"
#include <QHostAddress>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QTcpSocket>
#include <QVariant>
#include <cassert>

namespace flying_dragon
{

/// @brief A peer-to-peer connection
class Connection : public QTcpSocket
{
    Q_OBJECT

    signals:
    /// @brief The state has changed
    void StateChanged ();
    /// @brief The connection received a new icon
    void ReceivedIcon (const QImage &);
    /// @brief The connection received a new frame
    void ReceivedFrame (const Frame &);

    public:
    /// @brief The state of the connection
    enum State
    {
        StateDisconnected,
        StateConnecting,
        StateHandshaking,
        StateConnected,
        StateMax,
    };
    /// @brief Constructor
    /// @param parent Parent object
    /// @param id The connection's ID
    Connection (QObject *parent, unsigned id)
        : QTcpSocket (parent)
        , message_manager_ (this)
        , id_ (id)
        , state_ (StateDisconnected)
        , is_streaming_ (false)
        , is_foveated_ (false)
        , fx_ (0)
        , fy_ (0)
    {
        QObject::connect (this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(HandleError(QAbstractSocket::SocketError)));
    }
    /// @brief Destructor
    virtual ~Connection ()
    {
        disconnectFromHost ();
    }
    /// @brief Get the current state
    State GetState () const
    {
        return state_;
    }
    /// @brief Get the name of a state
    /// @param state The state
    QString GetName (State state)
    {
        QString name;
        switch (state)
        {
            case StateDisconnected:
            name = "Disconnected";
            break;
            case StateConnecting:
            name = "Connecting";
            break;
            case StateHandshaking:
            name = "Handshaking";
            break;
            case StateConnected:
            name = "Connected";
            break;
            default:
            name = "Unknown";
        }
        return name;
    }
    /// @brief Get the connection's name
    virtual QString GetName () const = 0;
    /// @brief Get the connection's ID
    /// @return The id
    unsigned GetID () const { return id_; }
    /// @brief Get streaming state
    /// @return true if it's streaming
    bool GetStreaming () const { return is_streaming_; }
    /// @brief Set streaming state
    /// @return true if it's streaming
    void SetStreaming (bool state)
    {
        is_streaming_ = state;
        emit StateChanged ();
    }
    /// @brief Get foveated state
    /// @return true if it's foveated
    bool GetFoveated () const { return is_foveated_; }
    /// @brief Set foveated state
    /// @return true if it's foveated
    void SetFoveated (bool state)
    {
        is_foveated_ = state;
        emit StateChanged ();
    }
    /// @brief Send an icon message to the peer
    /// @param icon The icon
    void SendIcon (const QImage &icon)
    {
        message_manager_.SendIcon (icon);
    }
    /// @brief Send a frame message to the peer
    /// @param frame The frame
    void SendFrame (const QImage &frame)
    {
        // Encode the frame
        Frame f;
        if (is_foveated_)
            f.Encode (frame, fx_, fy_, e2_);
        else
            f.Encode (frame);
        message_manager_.SendFrame (f);
    }
    /// @brief Send a fixation point to the peer
    /// @param x The x coord
    /// @param y The y coord
    /// @param e2 e2
    void SendFixation (qreal x, qreal y, qreal e2)
    {
        message_manager_.SendFixation (x, y, e2);
    }
    /// @brief Get message latency
    /// @return The latency in ms
    ///
    /// When a message is created, it is timestamped.  When
    /// a peer receives a message it creates an ack message
    /// and sends it back with the original timestamp.  This
    /// function returns the difference between the current
    /// time and the last ack message's timestamp.
    int GetMessageLatency () const
    {
        return message_manager_.GetMessageLatency ();
    }

    public slots:
    void ReceivedStreamCommand (bool state)
    {
        //qDebug() << "received stream command" << state;
        is_streaming_ = state;
        emit StateChanged ();
    }
    void SendStreamCommand (bool state)
    {
        //qDebug() << "sending stream command" << state;
        message_manager_.SendStreamCommand (state);
    }
    void ReceivedFoveateCommand (bool state)
    {
        qDebug() << "received foveate command" << state;
        is_foveated_ = state;
        emit StateChanged ();
    }
    void SendFoveateCommand (bool state)
    {
        qDebug() << "sending foveate command" << state;
        message_manager_.SendFoveateCommand (state);
    }

    protected slots:
    /// @brief Change to a different state
    virtual void ChangeState (State new_state)
    {
        //qDebug() << this
        //    << "changing state from"
        //    << GetName (state_) << "to" << GetName (new_state);
        state_ = new_state;
        emit StateChanged ();
    }
    /// @brief Go to Handshaking state
    void Handshaking ()
    {
        ChangeState (StateHandshaking);
        message_manager_.SendHandshake ();
        connect (&message_manager_, SIGNAL(ReceivedHandshake()),
            this, SLOT(Connected()));
    }
    /// @brief Go to Connected state
    void Connected ()
    {
        ChangeState (StateConnected);
        connect (&message_manager_, SIGNAL(ReceivedStreamCommand(bool)),
            this, SLOT(ReceivedStreamCommand(bool)));
        connect (&message_manager_, SIGNAL(ReceivedFoveateCommand(bool)),
            this, SLOT(ReceivedFoveateCommand(bool)));
        connect (&message_manager_, SIGNAL(ReceivedIcon(const QImage &)),
            this, SIGNAL(ReceivedIcon(const QImage &)));
        connect (&message_manager_, SIGNAL(ReceivedFrame(const Frame &)),
            this, SIGNAL(ReceivedFrame(const Frame &)));
        connect (&message_manager_, SIGNAL(ReceivedFixation(int,int,int)),
            this, SLOT(ReceivedFixation(int,int,int)));
    }


    private slots:
    void HandleError (QAbstractSocket::SocketError socketError)
    {
        switch (socketError)
        {
            case QAbstractSocket::RemoteHostClosedError:
                throw ConnectionGeneralException ("connection: remote host closed");
            break;
            case QAbstractSocket::HostNotFoundError:
                throw ConnectionGeneralException ("connection: host not found");
            break;
            case QAbstractSocket::ConnectionRefusedError:
                throw ConnectionGeneralException ("connection: connection refused");
            break;
            default:
                throw ConnectionGeneralException (errorString ().toStdString ().c_str ());
        }
    }
    void ReceivedFixation (int x, int y, int e2)
    {
        qDebug() << "received fixation at" << x << " " << y << " " << e2;
        fx_ = x;
        fy_ = y;
        e2_ = e2;
    }

    protected:
    /// @brief Message manager interface
    ///
    /// Each connection maintains its own message manager
    MessageManager message_manager_;

    private:
    unsigned id_;
    State state_;
    bool is_streaming_;
    bool is_foveated_;
    int fx_;
    int fy_;
    int e2_;
    static const qint64 MAX_MESSAGE_SIZE = 1024 * 1024 * 16;
};

/// @brief A connection initiated on the server
class ServerConnection : public Connection
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent object
    /// @param id The connection's ID
    /// @param socket_descriptor Descriptor of client socket
    ServerConnection (QObject *parent, unsigned id, int socket_descriptor)
        : Connection (parent, id)
    {
        ChangeState (StateConnecting);
        if (!setSocketDescriptor (socket_descriptor))
            throw ConnectionGeneralException ("server: invalid socket descriptor");
        connect (&message_manager_, SIGNAL(ReceivedDisconnectCommand()),
            this, SLOT(Disconnect()));
        // We are already connected, so go directly into the
        // Handshaking state...
        Handshaking ();
    }
    /// @brief Get the name of the connection
    QString GetName () const
    { return peerAddress ().toString (); }

    private slots:
    void Disconnect ()
    {
        // Disconnect from network
        disconnectFromHost ();
        // Disconnect signals
        this->disconnect ();
        ChangeState (StateDisconnected);
        // Allow for reconnect
        connect (this, SIGNAL(connected()),
            this, SLOT(Handshaking()));
    }
    private:
};

/// @brief A connection initiated by a client
class ClientConnection : public Connection
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent object
    /// @param id The connection's ID
    /// @param name The connection's name
    ClientConnection (QObject *parent, unsigned id, const QString &name)
        : Connection (parent, id)
        , name_ (name)
    {
    }
    /// @brief Get the name of the connection
    QString GetName () const
    { return name_; }

    public slots:
    /// @brief Connect to a server
    /// @param server Address of the server
    /// @param port Server port
    void ConnectToServer (const QString &server, quint16 port)
    {
        ChangeState (StateConnecting);
        connectToHost (server, port);
        connect (&message_manager_, SIGNAL(ReceivedDisconnectCommand()),
            this, SLOT(Disconnect()));
        connect (this, SIGNAL(connected()),
            this, SLOT(Handshaking()));
    }

    private slots:
    void Disconnect ()
    {
        // Disconnect from network
        disconnectFromHost ();
        // Disconnect signals
        this->disconnect ();
        ChangeState (StateDisconnected);
        // Allow for reconnect
        connect (this, SIGNAL(connected()),
            this, SLOT(Handshaking()));
    }

    private:
    QString name_;
};

} // namespace flying_dragon

#endif // CONNECTION_H
