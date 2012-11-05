/* (c) 2012 Álan Crístoffer */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char* argv[]);

protected:
    bool event(QEvent *event);
};

#endif // APPLICATION_H
