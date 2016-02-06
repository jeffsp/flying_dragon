// Message Manager
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug 11 17:53:27 CDT 2008

#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include "frame.h"
#include "message.h"
#include <QObject>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

namespace flying_dragon
{

/// @brief Mediates between connections and messages
class MessageManager : public QObject
{
    Q_OBJECT

    signals:
    /// @brief An error has occurred
    /// @param error The error string
    void Error (QString error);
    /// @brief An ack has been received
    /// @param id The id of the acknowledged message
    void ReceivedAck (quint64 id);
    /// @brief A handshake has been received
    void ReceivedHandshake ();
    /// @brief A stream command has been received
    void ReceivedStreamCommand (bool state);
    /// @brief A foveate command has been received
    void ReceivedFoveateCommand (bool state);
    /// @brief A disconnect command has been received
    void ReceivedDisconnectCommand ();
    /// @brief An icon has been received
    void ReceivedIcon (const QImage &icon);
    /// @brief A frame has been received
    void ReceivedFrame (const Frame &frame);
    /// @brief A fixation has been received
    void ReceivedFixation (int x, int y, int e2);
    /// @brief Some text has been received
    void ReceivedText ();
    /// @brief A message was sent
    void Sent (const Message &msg);
    /// @brief A message was received
    void Received (const Message &msg);

    public:
    /// @brief Constructor
    /// @param tcp_socket Socket to send/receive messages
    MessageManager (QTcpSocket *tcp_socket)
        : tcp_socket_ (tcp_socket)
        , handshake_data_ ("FLYING_DRAGON")
        , current_message_id_ (0)
        , message_latency_ (0)
        , drop_icon_limit_ (1 * 1024)
        , drop_frame_limit_ (32 * 1024)
    {
        assert (tcp_socket_);
        keep_alive_timer_.setInterval (5000);
        keep_alive_timer_.start ();
        QObject::connect (tcp_socket, SIGNAL(readyRead()),
            this, SLOT(TryToRead()));
        QObject::connect (&keep_alive_timer_, SIGNAL(timeout()),
            this, SLOT(SendKeepAlive()));
    }
    /// @brief Get message latency
    /// @return The latency in ms for the last message
    int GetMessageLatency () const
    {
        return message_latency_;
    }
    /// @brief Send a handshake message
    void SendHandshake ()
    {
        //qDebug() << this << "sending handshake";
        HandshakeMessage msg (NewMessageId (), handshake_data_);
        Send (msg);
    }
    /// @brief Send a stream command message
    void SendStreamCommand (bool state)
    {
        //qDebug() << this << "sending stream command" << state;
        StreamCommandMessage msg (NewMessageId (), state);
        Send (msg);
    }
    /// @brief Send a foveate command message
    void SendFoveateCommand (bool state)
    {
        //qDebug() << this << "sending foveate command" << state;
        FoveateCommandMessage msg (NewMessageId (), state);
        Send (msg);
    }
    /// @brief Send an icon message
    void SendIcon (const QImage &icon)
    {
        // Drop icons if the buffer is too full
        if (tcp_socket_->bytesToWrite () > drop_icon_limit_)
            return;

        //qDebug() << this << "sending icon";
        IconMessage msg (NewMessageId (), icon);
        Send (msg);
    }
    /// @brief Send an frame message
    void SendFrame (const Frame &frame)
    {
        // Drop frames if the buffer is too full
        if (tcp_socket_->bytesToWrite () > drop_frame_limit_)
            return;

        //qDebug() << this << "sending frame";
        FrameMessage msg (NewMessageId (), frame);
        Send (msg);
    }
    /// @brief Send a fixation message
    void SendFixation (int x, int y, int e2)
    {
        qDebug() << this << "sending fixation";
        FixationMessage msg (NewMessageId (), x, y, e2);
        Send (msg);
    }
    /// @brief Send some text
    void SendText (const QByteArray &)
    {
        //qDebug() << this << "sending text";
        //TextMessage msg (NewMessageID (), text);
        //Send (msg);
    }

    private slots:
    /// @brief Determine if the available bytes form a message
    void TryToRead ()
    {
        while (tcp_socket_->bytesAvailable ())
        {
            //qDebug() << this << tcp_socket_->bytesAvailable () << "bytes available";

            // Try to read a message
            QByteArray bytes = tcp_socket_->peek (MAX_PEEK_SIZE);
            Message msg;
            if (!msg.Read (bytes))
                return;

            // Consume the bytes
            tcp_socket_->read (msg.GetHeader ().size ());
            tcp_socket_->read (msg.GetData ().size ());

            //qDebug() << this << tcp_socket_->bytesAvailable () << "bytes available after reading";
            // Signal
            emit Received (msg);
            //qDebug() << this << "received" << msg.GetName (msg.GetType ());

            // Don't acknowledge acks
            if (msg.GetType () != Message::TypeAck)
                SendAck (msg.GetID ());

            // Calculate latency
            if (msg.GetType () == Message::TypeAck)
            {
                QTime current_time = QTime::currentTime ();
                message_latency_ = -current_time.msecsTo (msg.GetTime ());
                //qDebug() << "message message latency: " << message_latency_;
            }

            // Interpret message
            switch (msg.GetType ())
            {
                case Message::TypeKeepAlive:
                break;

                case Message::TypeAck:
                emit ReceivedAck (msg.GetID ());
                break;

                case Message::TypeHandshake:
                {
                    QByteArray data = msg.GetData ();
                    if (data != handshake_data_)
                        emit Error ("message_manager: invalid handshake data");
                    else
                        emit ReceivedHandshake ();
                }
                break;

                case Message::TypeStreamCommand:
                {
                    emit ReceivedStreamCommand (msg.GetState ());
                }
                break;

                case Message::TypeFoveateCommand:
                {
                    emit ReceivedFoveateCommand (msg.GetState ());
                }
                break;

                case Message::TypeDisconnectCommand:
                emit ReceivedDisconnectCommand ();
                break;

                case Message::TypeIcon:
                {
                    QImage icon;
                    msg.GetIcon (icon);
                    emit ReceivedIcon (icon);
                }
                break;

                case Message::TypeFrame:
                {
                    Frame frame;
                    msg.GetFrame (frame);
                    emit ReceivedFrame (frame);
                }
                break;

                case Message::TypeFixation:
                {
                    int x, y, e2;
                    msg.GetFixation (x, y, e2);
                    emit ReceivedFixation (x, y, e2);
                }
                break;

                case Message::TypeText:
                emit ReceivedText ();
                break;

                default:
                emit Error ("message_manager: unknown message type received");
            }
        }
    }

    private slots:
    void SendKeepAlive ()
    {
        if (tcp_socket_->state () == QTcpSocket::ConnectedState)
        {
            //qDebug() << this << "sending keepalive";
            KeepAliveMessage msg (NewMessageId ());
            Send (msg);
        }
    }
    void SendAck (quint64 id)
    {
        //qDebug() << this << "sending ack";
        QByteArray ack_data;
        QDataStream s (&ack_data, QIODevice::WriteOnly);
        s << id;
        AckMessage msg (NewMessageId (), ack_data);
        Send (msg);
    }

    private:
    void Send (const Message &msg)
    {
        assert (msg.IsValid ());

        // Send the message
        tcp_socket_->write (msg.GetHeader ());
        tcp_socket_->write (msg.GetData ());
        tcp_socket_->flush ();
        //qDebug() << this << "sent message id " << msg.GetID ();
        //qDebug() << this << tcp_socket_->bytesToWrite () << "bytes queued";
        // Signal
        emit Sent (msg);
    }
    quint64 NewMessageId ()
    {
        return current_message_id_++;
    }

    QTimer keep_alive_timer_;
    QTcpSocket *tcp_socket_;
    const QByteArray handshake_data_;
    quint64 current_message_id_;
    static const qint64 MAX_PEEK_SIZE = 1024 * 1024 * 16;
    int message_latency_;
    qint64 drop_icon_limit_;
    qint64 drop_frame_limit_;
};

} // namespace flying_dragon

#endif // MESSAGE_MANAGER_H
