// Test Server Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 13:02:06 CDT 2008

#include "server_widget.h"
#include "client_widget.h"
#include "connection_manager_widget.h"
#include <QApplication>
#include <QMainWindow>
#include <QObject>
#include <QGridLayout>
#include <iostream>

using namespace flying_dragon;
using namespace std;

class Dialog : public QMainWindow
{
    public:
    Dialog (Client *client,
        Server *server,
        ConnectionManager *connection_manager)
        : QMainWindow (0)
    {
        QWidget *centralwidget = new QWidget(this);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        setCentralWidget(centralwidget);
        QGridLayout *layout = new QGridLayout (centralwidget);

        ServerWidget *server_widget =
            new ServerWidget (this, server);
        addToolBar (server_widget);

        ClientWidget *client_widget =
            new ClientWidget (this, client);
        addToolBar (client_widget);

        ConnectionManagerWidget *connection_manager_widget =
            new ConnectionManagerWidget (this, connection_manager);
        layout->addWidget (connection_manager_widget);
    }
};

int main (int argc, char **argv)
{
    try
    {
        QApplication app (argc, argv);
        ConnectionManager connection_manager;

        Server server (connection_manager);
        Client client (connection_manager);
        Dialog dlg (&client, &server, &connection_manager);
        dlg.setWindowTitle (QString ("Client/Server"));
        dlg.show ();

        return app.exec ();
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
