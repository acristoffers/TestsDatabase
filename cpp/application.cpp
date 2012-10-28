/* (c) 2012 Álan Crístoffer */

#include "application.h"
#include "uibridge.h"

Application::Application(int argc,  char ** argv):
     QApplication(argc,argv)
{
}

bool Application::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        _UI->open_db(static_cast<QFileOpenEvent*>(event)->file());
        _UI->checkDB();
        return true;
    }

    return QApplication::event(event);
}
