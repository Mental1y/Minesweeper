#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include "qnewbutton.h"
#include <utility>
#include <QIcon>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class Minesweeper;
}
QT_END_NAMESPACE

class Minesweeper : public QWidget
{
    Q_OBJECT
private slots:
    void startNewGame();
public:
    Minesweeper(QWidget *parent = nullptr);
    ~Minesweeper();
    void PushButton(Qt::MouseButton mouse, QNewButton* button);
private:
    bool startPage();
    void generateMap(const int i, const int j);
    bool Correct(const int i, const int j) const;
    int Number(const int i, const int j) const;
    void dfs(int v, std::vector <bool> &used);
    bool finish;
    bool start;
    int BOMBS_NUMBER = 60;
    int PLACE_SIZE = 20;
    const int BUTTON_SIZE = 25;
    std::vector <QIcon> icons = {
        QIcon(":/resources/one.png"),
        QIcon(":/resources/two.png"),
        QIcon(":/resources/three.png"),
        QIcon(":/resources/four.png"),
        QIcon(":/resources/five.png"),
        QIcon(":/resources/six.png"),
        QIcon(":/resources/seven.png"),
        QIcon(":/resources/eight.png"),
        QIcon(":/resources/nine.png"),
    };
    QIcon bomb_icon = QIcon(":/resources/bomb.png");
    QIcon flag_icon = QIcon(":/resources/flag.png");
    QVBoxLayout* verticalLayout;
    QNewButton* restart;
    int cnt_open = 0;
    std::vector <std::vector <int> > graph;
    std::vector <std::vector <bool> > view;
    std::vector <std::vector <bool> > flag;
    Ui::Minesweeper *ui;
    QGridLayout* place;
    std::vector <std::pair <int, int> > actions = {
    {1, 1}, {-1, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}
    };
    std::vector <std::vector <char> > position;
};
#endif // MINESWEEPER_H
