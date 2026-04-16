#include "qnewbutton.h"
#include <QPushButton>
#include <QMouseEvent>

QNewButton::QNewButton(QWidget *parent):
    QPushButton(parent) {}

QNewButton::QNewButton(QString& text, QWidget* parent) :
    QPushButton(text, parent) {}

void QNewButton::setPos(const int i, const int j) {
    i_ = i;
    j_ = j;
}
std::pair <int, int> QNewButton::getPos() const {
    return {i_, j_};
}
void QNewButton::mousePressEvent(QMouseEvent* event) {
    emit newClicked(event->button(), this);
}
