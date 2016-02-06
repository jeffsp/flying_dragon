// Test Message Manager Widget
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Mon Aug 11 20:51:55 CDT 2008

#include <QApplication>
#include <iostream>
#include "test_message_manager_widget.h"

using namespace flying_dragon;
using namespace std;

int main (int argc, char **argv)
{
    try
    {
        QApplication app (argc, argv);

        Dialog dlg;
        dlg.setWindowTitle (QString ("Message Manager"));
        dlg.show ();

        return app.exec ();
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
