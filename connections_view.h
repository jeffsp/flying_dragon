// Connections View
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Jul 14 16:27:26 CDT 2008

#ifndef CONNECTIONS_VIEW_H
#define CONNECTIONS_VIEW_H

#include <QObject>
#include <QTreeView>
#include <QtDebug>

namespace flying_dragon
{

class ConnectionsView : public QTreeView
{
    Q_OBJECT

    public slots:
    public:
    ConnectionsView (QWidget *parent)
        : QTreeView (parent)
    {
        setRootIsDecorated (false);
        connect (this,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(on_doubleClicked(const QModelIndex &)));
    }
    protected slots:
    protected:
    private slots:
    void on_doubleClicked (const QModelIndex &index)
    {
        qDebug() << "double clicked";
    }
    private:
};

} // namespace flying_dragon

#endif // CONNECTIONS_VIEW_H
