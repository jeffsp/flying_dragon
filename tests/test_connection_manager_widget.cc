// Test Connection Manager Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug  4 20:16:17 CDT 2008

#include "connection_manager_widget.h"
#include <QApplication>
#include <QTimer>
#include <iostream>

using namespace flying_dragon;
using namespace std;

#include "test_connection_manager_widget.h"

int main (int argc, char **argv)
{
    try
    {
        QApplication app (argc, argv);
        ConnectionManager connection_manager;

        Dialog dlg (&connection_manager);
        dlg.setWindowTitle (QString ("Connection Manager"));
        dlg.show ();
        QTimer::singleShot (0, &dlg, SLOT(Test1 ()));
        QTimer::singleShot (0, &dlg, SLOT(Test1 ()));
        QTimer::singleShot (0, &dlg, SLOT(Test1 ()));
        for (int i = 0; i < 20; ++i)
        {
            QTimer::singleShot (i * 100, &dlg, SLOT(Test1()));
            QTimer::singleShot (i * 100 + 2000, &dlg, SLOT(Test2()));
        }
        QTimer::singleShot (10000, &app, SLOT(quit()));

        return app.exec ();
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
