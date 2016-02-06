// Camera Dialog
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 09:21:38 CDT 2008

#ifndef CAMERA_DIALOG_H
#define CAMERA_DIALOG_H

#include "persistent_dialog.h"
#include <QAction>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <QIcon>
#include <QImage>
#include <QTime>
#include <QTimer>
#include <QToolBar>
#include <cassert>
#include <stdexcept>

namespace flying_dragon
{

/// @brief Camera view widget
class CameraView : public QGraphicsView
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    /// @param margin Stretch margin
    CameraView (QWidget *parent, int margin)
        : QGraphicsView (parent)
        , margin_ (margin)
        , stretch_ (false)
    {
    }
    /// @brief Get stretch param
    bool getStretch () const { return stretch_; }
    /// @brief Set stretch param
    /// @param stretch New stretch param
    void setStretch (bool stretch) { stretch_ = stretch; }
    /// @brief Draw the background
    /// @param painter The painter
    /// @note QGraphicsView override
    void drawBackground (QPainter *painter, const QRectF &)
    {
        if (!background_image_.isNull ())
        {
            if (stretch_)
            {
                QRect r = geometry ();
                int w = r.width ();
                int h = r.height ();
                // Don't allow the image to shrink
                if (w < background_image_.width ())
                    w = background_image_.width ();
                if (h < background_image_.height ())
                    h = background_image_.height ();
                assert (background_image_.width () != 0);
                assert (background_image_.height () != 0);
                // Keep the image's aspect ratio
                const float aspect = background_image_.width () * 1.0 / background_image_.height ();
                if (w > h * aspect)
                    w = h * aspect;
                else if (h > w / aspect)
                    h = w / aspect;
                r.setLeft (-w / 2);
                r.setWidth (w);
                r.setTop (-h / 2);
                r.setHeight (h);
                painter->drawImage (r, background_image_);
            }
            else
            {
                painter->drawImage (QPoint (-background_image_.width () / 2,
                    -background_image_.height () / 2),
                    background_image_);
            }
        }
    }
    /// @brief Set the background image
    /// @param bg Background image
    void SetBackground (const QImage &bg)
    {
        background_image_ = bg;
        scene ()->invalidate ();
    }

    private:
    QImage background_image_;
    int margin_;
    bool stretch_;
};

/// @brief Camera scene object
class CameraScene : public QGraphicsScene
{
    Q_OBJECT

    signals:
    void NewFixation (int x, int y, int e2);

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    CameraScene (QObject *parent = 0)
        : QGraphicsScene (parent)
        , e2_ (10)
    {
    }
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event)
    {
        QPointF pos = event->scenePos ();
        //qDebug () << pos;
        emit NewFixation (pos.x (), pos.y (), 0);
    }
    void wheelEvent (QGraphicsSceneWheelEvent *event)
    {
        QPointF pos = event->scenePos ();
        int delta = event->delta ();
        e2_ += delta;
        //qDebug () << pos << delta;
        emit NewFixation (pos.x (), pos.y (), e2_);
    }

    private:
    int e2_;
};

/// @brief Dialog for displaying video
class CameraDialog : public PersistentDialog
{
    Q_OBJECT

    signals:
    /// @brief The dialog's fixation was updated
    /// @param x X coord
    /// @param y Y coord
    /// @param delta E2 delta
    void NewFixation (int x, int y, int delta);

    public slots:
    /// @brief A new frame has been generated
    /// @param frame The new frame
    ///
    /// Connect the NewFrame signal to this slot so that the
    /// dialog view will be updated whenever a new frame is
    /// generated.
    void NewFrame (const QImage &frame)
    {
        ui_.camera_view->SetBackground (frame);
    }
    /// @brief The fullscreen toggle slot
    void on_action_Fullscreen_toggled (bool checked)
    {
        if (checked)
        {
            showFullScreen ();
            //qDebug() << "fullscreen on";
        }
        else
        {
            showNormal ();
            //qDebug() << "fullscreen off";
        }
        // Fullscreen only works on top level windows, but
        // this stupid little trick will cause it to kindof
        // work...
        if (isVisible ())
        {
            hide ();
            show ();
        }
    }
    /// @brief The stretch toggle slot
    void on_action_Stretch_toggled (bool checked)
    {
        if (checked)
        {
            //qDebug() << "stretch on";
            ui_.camera_view->setStretch (true);
        }
        else
        {
            //qDebug() << "stretch off";
            ui_.camera_view->setStretch (false);
        }
        ui_.camera_scene->invalidate ();
    }

    public:
    /// @brief Constructor
    /// @param parent Parent widget
    CameraDialog (QWidget *parent = 0)
        : PersistentDialog (parent)
    {
        setObjectName ("CameraDialog");
        setWindowTitle ("Camera");
        SetupUI ();
    }
    /// @brief Constructor
    /// @param parent Parent widget
    CameraDialog (const QString &name
        , const QString &title
        , QWidget *parent = 0)
        : PersistentDialog (parent)
    {
        setObjectName (name);
        setWindowTitle (title);
        SetupUI ();
    }
    /// @brief Resize the dialog and view area
    /// @param w The scene width
    /// @param h The scene height
    void Resize (unsigned w, unsigned h)
    {
        resize (w + MARGIN, h + MARGIN + ui_.tool_bar->height ());
    }
    /// @brief Camera dialog user interface objects
    struct CameraDialogUI
    {
        //@{
        CameraView *camera_view;
        CameraScene *camera_scene;
        QAction *action_Fullscreen;
        QAction *action_Stretch;
        QToolBar *tool_bar;
        //@}
    } ui_;

    private:
    void SetupUI ()
    {
        QGridLayout *gridLayout = new QGridLayout (this);
        ui_.camera_view = new CameraView (this, MARGIN);
        ui_.camera_scene = new CameraScene (this);
        ui_.camera_view->setScene (ui_.camera_scene);
        QImage background_image (DEFAULT_WIDTH, DEFAULT_HEIGHT, QImage::Format_RGB32);
        background_image.fill (0);
        ui_.camera_view->SetBackground (background_image);
        ui_.action_Fullscreen = new QAction (this);
        ui_.action_Fullscreen->setObjectName (QString::fromUtf8 ("action_Fullscreen"));
        ui_.action_Fullscreen->setEnabled (true);
        ui_.action_Fullscreen->setCheckable (true);
        ui_.action_Fullscreen->setChecked (false);
        ui_.action_Fullscreen->setShortcut(QString ("Ctrl+F"));
        QIcon fullscreen_icon;
        fullscreen_icon.addPixmap(QPixmap(QString::fromUtf8(":/flying_dragon/images/window_fullscreen.png")), QIcon::Normal, QIcon::Off);
        fullscreen_icon.addPixmap(QPixmap(QString::fromUtf8(":/flying_dragon/images/window_nofullscreen.png")), QIcon::Normal, QIcon::On);
        ui_.action_Fullscreen->setIcon (fullscreen_icon);
        ui_.action_Stretch = new QAction (this);
        ui_.action_Stretch->setObjectName (QString::fromUtf8 ("action_Stretch"));
        ui_.action_Stretch->setEnabled (true);
        ui_.action_Stretch->setCheckable (true);
        ui_.action_Stretch->setChecked (false);
        ui_.action_Stretch->setShortcut(QString ("Ctrl+S"));
        QIcon stretch_icon;
        stretch_icon.addPixmap(QPixmap(QString::fromUtf8(":/flying_dragon/images/stretch.png")), QIcon::Normal, QIcon::On);
        ui_.action_Stretch->setIcon (stretch_icon);
        ui_.tool_bar = new QToolBar (this);
        ui_.tool_bar->addAction (ui_.action_Fullscreen);
        ui_.tool_bar->addAction (ui_.action_Stretch);
        gridLayout->addWidget (ui_.tool_bar, 0, 0);
        gridLayout->addWidget (ui_.camera_view, 1, 0);
        gridLayout->setContentsMargins (0, 0, 0, 0);
        QObject::connect (ui_.camera_scene, SIGNAL(NewFixation(int,int,int)),
            this, SIGNAL(NewFixation(int,int,int)));
        resize (DEFAULT_WIDTH + MARGIN, DEFAULT_HEIGHT + MARGIN + ui_.tool_bar->height ());
        QMetaObject::connectSlotsByName (this);
    }
    static const unsigned MARGIN = 30;
    static const unsigned DEFAULT_WIDTH = 320;
    static const unsigned DEFAULT_HEIGHT = 240;
};

} // namespace flying_dragon

#endif // CAMERA_DIALOG_H
