// Persistent Dialog
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// Wed Jul  2 09:22:47 CDT 2008

#ifndef PERSISTENT_DIALOG_H
#define PERSISTENT_DIALOG_H

#include <QDebug>
#include <QDialog>
#include <QMoveEvent>
#include <QSettings>
#include <QShowEvent>

namespace flying_dragon
{

/// @brief A dialog that saves its geometry settings
class PersistentDialog : public QDialog
{
    Q_OBJECT

    signals:
    void Close ();

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    PersistentDialog (QWidget *parent)
        : QDialog (parent)
        , settings_ ("jsp" , "flying_dragon")
    {
    }

    protected:
    /// @brief Dialog override
    /// @param event The event
    void closeEvent (QCloseEvent *event)
    {
        //qDebug() << "close";
        writeSettings ();
        emit Close ();
        event->accept ();
    }
    /// @brief Dialog override
    /// @param event The event
    void hideEvent (QHideEvent *event)
    {
        //qDebug() << "hide";
        writeSettings ();
        event->accept ();
    }
    /// @brief Dialog override
    /// @param event The event
    void resizeEvent (QResizeEvent *event)
    {
        //qDebug() << "resize";
        if (isVisible ())
            writeSettings ();
        event->accept ();
    }
    /// @brief Dialog override
    /// @param move The event
    void moveEvent (QMoveEvent *event)
    {
        //qDebug() << "move";
        // Move gets called by Qt before the dialog is
        // displayed.  We only are interested in user
        // initiated moves.
        if (isVisible ())
            writeSettings ();
        event->accept ();
    }
    /// @brief Dialog override
    /// @param event The event
    void showEvent (QShowEvent *event)
    {
        //qDebug() << "show";
        readSettings ();
        event->accept ();
    }

    private:
    void readSettings ()
    {
        settings_.beginGroup (objectName ());
        restoreGeometry (settings_.value ("geometry").toByteArray());
        settings_.endGroup ();
    }
    void writeSettings ()
    {
        settings_.beginGroup (objectName ());
        settings_.setValue ("geometry", saveGeometry ());
        settings_.endGroup ();
    }
    QSettings settings_;
};

} // namespace flying_dragon

#endif // PERSISTENT_DIALOG_H
