/* (c) 2012 Álan Crístoffer */

#include "application.h"

Application::Application(int argc,  char ** argv):
     QApplication(argc,argv)
{
}

bool Application::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        // open the file :: static_cast<QFileOpenEvent*>(event)->file()
        return true;
    }

    return QApplication::event(event);
}
