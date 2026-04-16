#ifndef QNEWBUTTON_H
#define QNEWBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QMouseEvent>

class QNewButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QNewButton(QString& text, QWidget* parent = nullptr);
    explicit QNewButton(QWidget* parent = nullptr);
    void setPos(int i, int j);
    std::pair <int, int> getPos() const;

signals:
    void newClicked(Qt::MouseButton mouse, QNewButton* button);

protected:
    void mousePressEvent(QMouseEvent* event);
private:
    int i_, j_;
};

#endif // QNEWBUTTON_H
