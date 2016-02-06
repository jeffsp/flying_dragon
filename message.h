// Message Functions
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Jul 28 16:15:09 CDT 2008

#ifndef MESSAGE_H
#define MESSAGE_H

#include "frame.h"
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QImage>
#include <QTime>
#include <cassert>

namespace flying_dragon
{

/// @brief A network message
class Message
{
    public:
    /// @brief Message Types
    ///{
    enum Type
    {
        TypeAck,
        TypeKeepAlive,
        TypeHandshake,
        TypeStreamCommand,
        TypeFoveateCommand,
        TypeDisconnectCommand,
        TypeIcon,
        TypeFrame,
        TypeFixation,
        TypeText,
        TypeUnknown,
    };
    ///}
    /// @brief Constructor
    Message ()
        : type_ (TypeUnknown)
        , id_ (0)
        , time_ (QTime (0, 0))
    {
    }
    /// @brief Invariant checker
    /// @return true if invariants are satisfied
    bool IsValid () const
    {
        if (type_ == TypeUnknown)
            return false;
        if (data_.size () > MAX_DATA_SIZE)
            return false;
        return true;
    }
    /// @brief Get the message type
    /// @return The message type
    Type GetType () const
    {
        return type_;
    }
    /// @brief Get a string describing the message type
    /// @return The string
    QString GetName (Type type) const
    {
        QString name;
        switch (type)
        {
            case TypeAck:
                name = "Ack";
            break;
            case TypeKeepAlive:
                name = "KeepAlive";
            break;
            case TypeHandshake:
                name = "Handshake";
            break;
            case TypeStreamCommand:
                name = "StreamCommand";
            break;
            case TypeFoveateCommand:
                name = "FoveateCommand";
            break;
            case TypeDisconnectCommand:
                name = "DisconnectCommand";
            break;
            case TypeIcon:
                name = "Icon";
            break;
            case TypeFrame:
                name = "Frame";
            break;
            case TypeFixation:
                name = "Fixation";
            break;
            case TypeText:
                name = "Text";
            break;
            default:
            case TypeUnknown:
                name = "Unknown";
            break;
        }
        return name;
    }
    /// @brief Get the message ID number
    /// @return The ID
    quint64 GetID () const
    {
        return id_;
    }
    /// @brief Get the message timestamp
    /// @return The timestamp
    QTime GetTime () const
    {
        return time_;
    }
    /// @brief Get a serialized message header
    /// @return The header
    const QByteArray GetHeader () const
    {
        QByteArray header;
        QDataStream s (&header, QIODevice::WriteOnly);
        s << static_cast<quint32> (type_);
        s << static_cast<quint64> (id_);
        s << time_;
        s << static_cast<quint32> (data_.size ());
        //qDebug() << this << "initing";
        //qDebug() << this << "type_=" << static_cast<int> (type_);
        //qDebug() << this << "id_=" << id_;
        //qDebug() << this << "time_=" << time_.toString ();
        //qDebug() << this << "data_.size()=" << data_.size ();
        return header;
    }
    /// @brief Fill an image with an icon
    void GetIcon (QImage &icon)
    {
        QDataStream s (data_);
        qint32 width;
        qint32 height;
        s >> width;
        s >> height;
        //qDebug() << "icon" << width << height;
        icon = QImage (width, height, QImage::Format_RGB32);
        assert (icon.numBytes () == data_.size () - 8);
        memcpy (icon.bits (), data_.data () + 8, icon.numBytes ());
    }
    /// @brief Fill a frame with data
    void GetFrame (Frame &frame)
    {
        QDataStream s (data_);
        qint32 width;
        qint32 height;
        s >> width;
        s >> height;
        //qDebug() << "frame" << width << height;
        frame = Frame (width, height, QImage::Format_RGB32);
        assert (frame.numBytes () == data_.size () - 8);
        memcpy (frame.bits (), data_.data () + 8, frame.numBytes ());
    }
    /// @brief Get x and y coords
    void GetFixation (int &fx, int &fy, int &e2)
    {
        QDataStream s (data_);
        qint32 x;
        qint32 y;
        qint32 e;
        s >> x;
        s >> y;
        s >> e;
        fx = x;
        fy = y;
        e2 = e;
    }
    /// @brief Get a state
    bool GetState ()
    {
        QDataStream s (data_);
        bool state;
        s >> state;
        return state;
    }
    /// @brief Get a byte array containing the message data
    /// @return The data
    ///
    /// The data does not include the header
    const QByteArray &GetData () const
    {
        return data_;
    }
    /// @brief Try to read a message
    /// @param bytes Bytes containing a message
    /// @return true on successful read
    ///
    /// Try to read a message given a bunch of bytes.  The
    /// bytes may contain an incomplete message, in which
    /// case the function returns with no side effects.
    bool Read (const QByteArray &bytes)
    {
        // Can we get the header?
        if (bytes.size () < HEADER_SIZE)
            return false;
        QByteArray header = QByteArray (
            bytes.constData (),
            HEADER_SIZE);
        QDataStream s (header);
        quint32 type;
        quint64 id;
        QTime time;
        quint32 data_size;
        s >> type;
        s >> id;
        s >> time;
        s >> data_size;
        //qDebug() << this << "reading";
        //qDebug() << this << "type=" << static_cast<int> (type);
        //qDebug() << this << "id=" << id;
        //qDebug() << this << "time=" << time;
        //qDebug() << this << "data_size=" << data_size;
        // Can we get the data?
        if (bytes.size () < static_cast<int> (data_size + HEADER_SIZE))
            return false;
        // Commit
        type_ = static_cast<Type> (type);
        id_ = static_cast<Type> (id);
        time_ = time;
        data_ = QByteArray (
            bytes.constData () + HEADER_SIZE,
            data_size);
        assert (this->IsValid ());
        return true;
    }

    private:
    Type type_;
    quint64 id_;
    QTime time_;
    static const int HEADER_SIZE =
        sizeof (quint32) + // type
        sizeof (quint64) + // id
        sizeof (QTime) + // time
        sizeof (quint32); // data size
    static const int MAX_DATA_SIZE = 1024 * 1024 * 16;

    protected:
    /// @brief Constructor
    /// @param type Message type
    /// @param id Message ID
    /// @param data Optional message data
    ///
    /// This constructor is used to construct derived
    /// message classes.
    Message (Type type, quint64 id, const QByteArray &data = QByteArray ())
        : type_ (type)
        , id_ (id)
        , time_ (QTime::currentTime ())
        , data_ (data)
    {
    }
    /// @brief Serialized message data
    ///
    /// Derived classes have access to the message data.
    /// This will avoid copying messages that are
    /// potentially very large.
    QByteArray data_;
};

/// @brief An acknowledgement of a message
class AckMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id Message ID
    /// @param data Ack data
    AckMessage (quint64 id, const QByteArray &data)
        : Message (TypeAck, id, data)
    { }

    private:
};

/// @brief An empty message sent to keep the connection alive
class KeepAliveMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id Message ID
    KeepAliveMessage (quint64 id)
        : Message (TypeKeepAlive, id)
    { }

    private:
};

/// @brief A message that initiates a connection
class HandshakeMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id The message ID
    /// @param data The handshake string
    HandshakeMessage (quint64 id, const QByteArray &data)
        : Message (TypeHandshake, id, data)
    { }

    private:
};

/// @brief A message that indicates stream state
class StreamCommandMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id The message ID
    /// @param state The stream state
    StreamCommandMessage (quint64 id, bool state)
        : Message (TypeStreamCommand, id)
    {
        QDataStream s (&data_, QIODevice::WriteOnly);
        s << state;
    }

    private:
};

/// @brief A message that indicates foveation state
class FoveateCommandMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id The message ID
    /// @param state The foveation state
    FoveateCommandMessage (quint64 id, bool state)
        : Message (TypeFoveateCommand, id)
    {
        QDataStream s (&data_, QIODevice::WriteOnly);
        s << state;
    }

    private:
};

/// @brief A message containing an icon image
class IconMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id The message ID
    /// @param icon Icon image
    IconMessage (quint64 id, const QImage &icon)
        : Message (TypeIcon, id)
    {
        QDataStream s (&data_, QIODevice::WriteOnly);
        s << static_cast<qint32> (icon.width ());
        s << static_cast<qint32> (icon.height ());
        data_ += QByteArray (reinterpret_cast<const char *> (icon.bits ()), icon.numBytes ());
    }

    private:
};

/// @brief A message containing a frame image
class FrameMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id The message ID
    /// @param frame Frame image
    FrameMessage (quint64 id, const Frame &frame)
        : Message (TypeFrame, id)
    {
        QDataStream s (&data_, QIODevice::WriteOnly);
        s << static_cast<qint32> (frame.width ());
        s << static_cast<qint32> (frame.height ());
        data_ += QByteArray (reinterpret_cast<const char *> (frame.bits ()), frame.numBytes ());
    }

    private:
};

/// @brief A message containing a fixation image
class FixationMessage : public Message
{
    public:
    /// @brief Constructor
    /// @param id The message ID
    /// @param x X coord
    /// @param y Y coord
    /// @param e2_delta e2 delta
    FixationMessage (quint64 id, int x, int y, int e2_delta)
        : Message (TypeFixation, id)
    {
        QDataStream s (&data_, QIODevice::WriteOnly);
        s << static_cast<qint32> (x);
        s << static_cast<qint32> (y);
        s << static_cast<qint32> (e2_delta);
    }

    private:
};

} // namespace flying_dragon

#endif // MESSAGE_H
