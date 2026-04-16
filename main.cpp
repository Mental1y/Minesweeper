#include "minesweeper.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("D:/User/projects/Qt_projects/Minesweeper/resources/bomb.png"));
    Minesweeper w;
    if (w.isHidden()) {
        return 0;
    }
    w.show();
    return a.exec();
}
