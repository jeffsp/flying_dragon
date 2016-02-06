// Exception enabled qt app
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Tue May  6 08:17:31 CDT 2008

#ifndef EXCEPTION_ENABLED_APP_H
#define EXCEPTION_ENABLED_APP_H

#include <QApplication>
#include <iostream>
#include <stdexcept>

namespace flying_dragon
{

/// @brief A Qt application that can throw
class ExceptionEnabledApplication : public QApplication
{
    Q_OBJECT

    public:
    /// @brief Constructor
    /// @param argc Number of args
    /// @param argv The args
    ExceptionEnabledApplication (int &argc, char *argv[])
        : QApplication (argc, argv)
    {
    }
    /// @brief QApplication override
    bool notify (QObject *o, QEvent *e)
    {
        try
        {
            return QApplication::notify (o, e);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what () << std::endl;
            QApplication::quit ();
            // You can't rethrow after you quit()
        }
        catch (...)
        {
            std::cerr << "unknown exception" << std::endl;
            QApplication::quit ();
            // You can't rethrow after you quit()
        }
        return true;
    }
    private:
};

} // namespace flying_dragon

#endif // EXCEPTION_ENABLED_APP_H
