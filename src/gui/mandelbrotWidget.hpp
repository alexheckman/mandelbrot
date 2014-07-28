#pragma once

#include <memory>

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QPoint>
#include <QRubberBand>

class MandelbrotWidget : public QWidget
{
public:
    explicit MandelbrotWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);
    /* keyboard events */
    void keyReleaseEvent(QKeyEvent *e);
    /* mouse events */
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QPoint select_origin_; //selection rectangle origin point
    QRect selection_rectangle_;
    std::unique_ptr<QRubberBand> rb_;
    bool selection_;
};