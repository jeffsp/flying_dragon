// Message Manager Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug 11 20:57:02 CDT 2008

#ifndef MESSAGE_MANAGER_WIDGET_H
#define MESSAGE_MANAGER_WIDGET_H

#include "message_manager.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QWidget>
#include <cassert>

namespace flying_dragon
{

/// @brief A widget for displaying messages
class MessageManagerWidget : public QWidget
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param message_manager Message manager
    MessageManagerWidget (QWidget *parent,
        MessageManager *message_manager)
        : QWidget (parent)
        , message_manager_ (message_manager)
        , total_sent_ (0)
        , total_received_ (0)
    {
        assert (message_manager_);
        SetupUI ();
        connect (message_manager_, SIGNAL(Sent(const Message &)),
            this, SLOT(Sent(const Message &)));
        connect (message_manager_, SIGNAL(Received(const Message &)),
            this, SLOT(Received(const Message &)));
    }

    private slots:
    void Sent (const Message &msg)
    {
        text_->appendPlainText (QString ("%1 > %2, id:%3, size:%4")
                .arg (msg.GetTime ().toString ("hh:mm:ss.zzz"))
                .arg (msg.GetName (msg.GetType ()))
                .arg (msg.GetID ())
                .arg (msg.GetData ().size ()));
        ++total_sent_;
        total_sent_label_->setText (QString::number (total_sent_));
    }
    void Received (const Message &msg)
    {
        text_->appendPlainText (QString ("%1 < %2, id:%3, size:%4")
                .arg (msg.GetTime ().toString ("hh:mm:ss.zzz"))
                .arg (msg.GetName (msg.GetType ()))
                .arg (msg.GetID ())
                .arg (msg.GetData ().size ()));
        ++total_received_;
        total_received_label_->setText (QString::number (total_received_));
    }

    private:
    void SetupUI ()
    {
        QGridLayout *layout = new QGridLayout (this);
        text_ = new QPlainTextEdit (this);
        text_->setReadOnly (true);
        layout->addWidget (text_);
        QLabel *sent_label = new QLabel ("sent", this);
        QLabel *received_label = new QLabel ("received", this);
        total_sent_label_ = new QLabel ("0", this);
        total_received_label_ = new QLabel ("0", this);
        QHBoxLayout *total_messages_layout = new QHBoxLayout;
        total_messages_layout->addWidget (sent_label);
        total_messages_layout->addWidget (total_sent_label_);
        total_messages_layout->addWidget (received_label);
        total_messages_layout->addWidget (total_received_label_);
        total_messages_layout->addStretch ();
        layout->addLayout (total_messages_layout, 1, 0);
    }
    MessageManager *message_manager_;
    QPlainTextEdit *text_;
    QLabel *total_sent_label_;
    QLabel *total_received_label_;
    quint64 total_sent_;
    quint64 total_received_;
};

} // namespace flying_dragon

#endif // MESSAGE_MANAGER_WIDGET_H
