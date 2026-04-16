#include "qnewbutton.h"
#include "minesweeper.h"
#include "ui_minesweeper.h"
#include <QGridLayout>
#include <random>
#include <QPushButton>
#include <set>
#include <utility>
#include <QSpinBox>
#include <QDialog>
#include <QLabel>
#include <QIcon>

void Minesweeper::dfs(int v, std::vector <bool> &used) {
    used[v] = true;
    if (!flag[v / PLACE_SIZE][v % PLACE_SIZE]) {
        if (!view[v / PLACE_SIZE][v % PLACE_SIZE]) {
            cnt_open++;
        }
        view[v / PLACE_SIZE][v % PLACE_SIZE] = true;
    }
    for (auto to : graph[v]) {
        if (used[to]) {
            continue;
        }
        dfs(to, used);
    }
}

int Minesweeper::Number(const int i, const int j) const {
    return i * PLACE_SIZE + j;
}
bool Minesweeper::Correct(const int i, const int j) const {
    return i >= 0 && i < PLACE_SIZE && j >= 0 && j < PLACE_SIZE;
}
void Minesweeper::generateMap(const int x, const int y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int bomb_cnt = 0;
    std::set <std::pair <int, int> > exception;
    exception.emplace(x, y);
    for (int i = 0; i < 8; i++) {
        if (Correct(x + actions[i].first, y + actions[i].second)) {
            exception.emplace(x + actions[i].first, y + actions[i].second);
        }
    }
    while (bomb_cnt < BOMBS_NUMBER) {
        int new_x = std::uniform_int_distribution <int> (0, PLACE_SIZE - 1)(gen);
        int new_y = std::uniform_int_distribution <int> (0, PLACE_SIZE - 1)(gen);
        if (exception.find({new_x, new_y}) == exception.end()) {
            bomb_cnt++;
            position[new_x][new_y] = 'B';
            exception.emplace(new_x, new_y);
        }
    }
    for (int i = 0; i < PLACE_SIZE; i++) {
        for (int j = 0; j < PLACE_SIZE; j++) {
            if (position[i][j] == 'B') {
                for (int k = 0; k < 8; k++) {
                    int new_x = i + actions[k].first;
                    int new_y = j + actions[k].second;
                    if (Correct(new_x, new_y) && position[new_x][new_y] != 'B') {
                        if (position[new_x][new_y] == '.') {
                            position[new_x][new_y] = '1';
                        } else {
                            position[new_x][new_y] = position[new_x][new_y] + 1;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < PLACE_SIZE; i++) {
        for (int j = 0; j < PLACE_SIZE; j++) {
            if (position[i][j] == '.') {
                for (int k = 0; k < 8; k++) {
                    int new_x = i + actions[k].first;
                    int new_y = j + actions[k].second;
                    if (Correct(new_x, new_y)) {
                        graph[Number(i, j)].push_back(Number(new_x, new_y));
                    }
                }
            }
        }
    }
}
void Minesweeper::startNewGame() {
    start = false;
    finish = false;
    cnt_open = 0;
    QLayoutItem* temp = verticalLayout->itemAt(0);
    QNewButton* main_button = qobject_cast<QNewButton*>(temp->widget());
    main_button->setText(">>> START <<<");
    setStyleSheet("background-color: #121212;");
    for (int i = 0; i < PLACE_SIZE; i++) {
        fill(position[i].begin(), position[i].end(), '.');
        fill(view[i].begin(), view[i].end(), false);
        fill(flag[i].begin(), flag[i].end(), false);
    }
    for (int i = 0; i < PLACE_SIZE * PLACE_SIZE; i++) {
        graph[i].clear();
    }
    for (int i = 0; i < PLACE_SIZE; i++) {
        for (int j = 0; j < PLACE_SIZE; j++) {
            QLayoutItem* temp = place->itemAtPosition(i, j);
            QNewButton* now = qobject_cast <QNewButton*>(temp->widget());
            now->setStyleSheet("background : #2A2A2A;");
            now->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
            now->setIcon(QIcon());
        }
    }
    this->show();

}
void Minesweeper::PushButton(Qt::MouseButton mouse, QNewButton* button) {
    int x = button->getPos().first, y = button->getPos().second;
    if (!start && mouse == Qt::LeftButton && !finish) {
        generateMap(x, y);
        QLayoutItem* temp = verticalLayout->itemAt(0);
        QNewButton* main_button = qobject_cast<QNewButton*>(temp->widget());
        main_button->setText(">>> GAME ACTIVE <<<");
        start = true;
    }
    if (!finish && mouse == Qt::LeftButton) {
        if (flag[x][y] == true) {
            return;
        }
        if (position[x][y] == 'B') {
            button->setStyleSheet("background : #1A1A1A;");
            button->setIcon(bomb_icon);
            finish = true;
            QLayoutItem* temp = verticalLayout->itemAt(0);
            QNewButton* main_button = qobject_cast<QNewButton*>(temp->widget());
            main_button->setText(">>> GAME OVER <<<");
            return;
        }
        std::vector <bool> used(PLACE_SIZE * PLACE_SIZE, false);
        dfs(Number(x, y), used);
        for (int i = 0; i < PLACE_SIZE; i++) {
            for (int j = 0; j < PLACE_SIZE; j++) {
                if (view[i][j]) {
                    QLayoutItem* temp = place->itemAtPosition(i, j);
                    QNewButton* now = qobject_cast <QNewButton*>(temp->widget());
                    now->setStyleSheet("background : #1A1A1A;");
                    if (position[i][j] >= '1' && position[i][j] <= '8') {
                        now->setIcon(icons[position[i][j] - '1']);
                    }
                }
            }
        }
    }
    if (!finish && mouse == Qt::RightButton && !view[x][y]) {
        if (flag[x][y] == true) {
            flag[x][y] = false;
            button->setIcon(QIcon());
        } else {
            flag[x][y] = true;
            button->setIcon(flag_icon);
        }
    }
    if (cnt_open == (PLACE_SIZE * PLACE_SIZE) - BOMBS_NUMBER && !finish) {
        finish = true;
        QLayoutItem* temp = verticalLayout->itemAt(0);
        QNewButton* main_button = qobject_cast<QNewButton*>(temp->widget());
        main_button->setText(">>> YOU WIN <<<");
    }
}
bool Minesweeper::startPage() {
    QDialog dialog(this);
    dialog.setStyleSheet(
        "QDialog {"
        "    background: #0c0c0c;"
        "    color: #00FF00;"
        "    font: 16px 'Consolas', monospace;"
        "}"
        "QLabel {"
        "    color: #00FF00;"
        "    background: transparent;"
        "    border: none;"
        "}"
        "QSpinBox {"
        "    background: #0c0c0c;"
        "    color: #00FF00;"
        "    selection-background-color: #00FF00;"
        "    font: 14px 'Consolas', monospace;"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "    width: 0px;"
        "    border: none;"
        "}"
        );
    dialog.setFixedSize(200, 130);
    QSpinBox size(&dialog);
    size.setRange(12, 35);
    size.setValue(20);
    QSpinBox bombs(&dialog);
    bombs.setRange(1, 1225);
    bombs.setValue(60);
    QPushButton ok(&dialog);
    ok.setText("[ start ]");
    ok.setStyleSheet("background: #0c0c0c; color : #00FF00; font: 'Consolas'; padding: 8px; border: none;");
    QGridLayout layout(&dialog);
    QLabel size_text("-  size");
    size_text.setStyleSheet("background: #0c0c0c; color : #00FF00; font: 'Consolas'; padding: 8px; border: none;");
    QLabel bombs_text("- mines");
    bombs_text.setStyleSheet("background: #0c0c0c; color : #00FF00; font: 'Consolas'; padding: 8px; border: none;");
    layout.addWidget(&size_text, 0, 0, 1, 1);
    layout.addWidget(&size, 0, 1, 1, 2);
    layout.addWidget(&bombs_text, 1, 0, 1, 1);
    layout.addWidget(&bombs, 1, 1, 1, 2);
    layout.addWidget(&ok, 2, 0, 1, 3);
    connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        PLACE_SIZE = size.value();
        BOMBS_NUMBER = std::min((PLACE_SIZE * PLACE_SIZE) / 2, bombs.value());
        return true;
    }
    return false;
}
Minesweeper::Minesweeper(QWidget *parent) : QWidget(parent), ui(new Ui::Minesweeper) {
    if (!startPage()) {
        this->hide();
        return;
    }
    ui->setupUi(this);
    start = false;
    finish = false;
    //121212
    setStyleSheet("background-color: #0c0c0c;");
    position.resize(PLACE_SIZE);
    flag.resize(PLACE_SIZE);
    view.resize(PLACE_SIZE);
    for (int i = 0; i < PLACE_SIZE; i++) {
        position[i].resize(PLACE_SIZE, '.');
        view[i].resize(PLACE_SIZE, false);
        flag[i].resize(PLACE_SIZE, false);
    }
    cnt_open = 0;
    graph.resize(PLACE_SIZE * PLACE_SIZE);
    this->setFixedSize(BUTTON_SIZE * PLACE_SIZE, BUTTON_SIZE * PLACE_SIZE);
    verticalLayout = new QVBoxLayout(this);
    place = new QGridLayout();
    restart = new QNewButton(this);
    restart->setText(">>> START <<<");
    restart->setStyleSheet("background: #0c0c0c; color : #00FF00; font: 14px 'Consolas'; padding: 8px; border: none;");
    restart->connect(restart, &QNewButton::newClicked, this, &Minesweeper::startNewGame);
    verticalLayout->addWidget(restart, Qt::AlignCenter);
    place->setHorizontalSpacing(1);
    place->setVerticalSpacing(1);
    for (int i = 0; i < PLACE_SIZE; i++) {
        for (int j = 0; j < PLACE_SIZE; j++) {
            QNewButton* temp = new QNewButton(this);
            temp->setStyleSheet("background : #2A2A2A;");
            temp->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
            temp->connect(temp, &QNewButton::newClicked, this, &Minesweeper::PushButton);
            place->addWidget(temp, i, j, 1, 1);
            temp->setPos(i, j);
        }
    }
    verticalLayout->addLayout(place);
    this->show();
}

Minesweeper::~Minesweeper() {
    delete ui;
}
