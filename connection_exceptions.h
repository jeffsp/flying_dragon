// Connection Exceptions
//
// Copyright (C) 2008
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 16:53:16 CDT 2008

#ifndef CONNECTION_EXCEPTIONS_H
#define CONNECTION_EXCEPTIONS_H

#include <cstring>
#include <iosfwd>
#include <stdexcept>

namespace flying_dragon
{

/// @brief The base class for a Connection exception
struct ConnectionException : public std::exception
{
    /// @brief Return a description of the error
    const char *what () const throw () = 0;
};

/// @brief A general exception
struct ConnectionGeneralException : public virtual ConnectionException
{
    /// @brief Constructor
    ConnectionGeneralException (const char *str) : str (str)
    { }
    /// @brief Return a description of the error
    const char *what () const throw ()
    { return str; }
    private:
    const char *str;
};

} // namespace flying_dragon

#endif // CONNECTION_EXCEPTIONS_H

