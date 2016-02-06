// The Flying Dragon
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Thu Feb 14 14:21:32 CST 2008

/// @mainpage Flying Dragon
///
/// Bunch of documentation

#include "argv.h"
#include "exception_enabled_app.h"
#include "main_window.h"
#include "ui_flying_dragon.h"
#include <iostream>

using namespace std;
using namespace jsp;
using namespace flying_dragon;

int main(int argc, char *argv[])
{
    try
    {
        // First let Qt parse args
        ExceptionEnabledApplication app (argc, argv);

        // Parse remaining args
        bool help = false;
        bool verbose = false;
        CommandLine cl;
        cl.AddSpec ("help", 'h', help, "Print help message");
        cl.AddSpec ("verbose", 'v', verbose, "Verbose output");
        // Group argv's into option groups
        cl.GroupArgs (argc, argv, 1);
        // Convert from strings to their proper type
        cl.ExtractBegin ();
        cl.Extract (help);
        cl.Extract (verbose);
        cl.ExtractEnd ();

        if (help)
        {
            cout << argv[0] << " " << cl.Usage () << endl;
            cout << cl.Help ();
            return 0;
        }
        if (!cl.GetLeftOverArgs ().empty ())
            throw runtime_error ("usage: " + string (argv[0]) + " " + cl.Usage ());

        flying_dragon::MainWindow main_window (0);
        main_window.show ();
        return app.exec ();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what () << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "uncaught exception" << std::endl;
        return -1;
    }
}
