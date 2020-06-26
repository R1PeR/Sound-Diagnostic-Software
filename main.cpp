#include "mainwindow.h"
//
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//
#include <QApplication>

int main(int argc, char *argv[])
{
    //AllocConsole();
    //freopen("CONOUT$", "w", stdout);
    //freopen("CONIN$", "r", stdin);
    //std::ios::sync_with_stdio();
    //
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
