// New Connection Dialog
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Jul  2 16:16:51 CDT 2008

#ifndef NEW_CONNECTION_DIALOG_H
#define NEW_CONNECTION_DIALOG_H

#include "persistent_dialog.h"
#include "ui_new_connection_dialog.h"

namespace flying_dragon
{

class NewConnectionDialog : public PersistentDialog
{
    Q_OBJECT

    public:
    NewConnectionDialog (QWidget *parent)
        : PersistentDialog (parent)
    {
        ui_.setupUi(this);
    }
    QString HostAddress () const
    {
        return ui_.addressLineEdit->text ();
    }

    private:
    Ui::NewConnectionDialog ui_;
};

} // namespace flying_dragon

#endif // NEW_CONNECTION_DIALOG_H
