#include "mandelbrotWidget.hpp"

#include <QPainter>
#include <QStyleOptionRubberBand>

MandelbrotWidget::MandelbrotWidget(QWidget* parent)
:
QWidget(parent),
select_origin_(),
selection_rectangle_(),
rb_(nullptr),
selection_(false)
{
}

void MandelbrotWidget::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);
}

void MandelbrotWidget::keyReleaseEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        if (rb_.get())
            rb_->hide();
        this->repaint();
    }
}

void MandelbrotWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MouseButton::LeftButton) {
        selection_ = true;
        select_origin_ = e->pos();
        rb_.reset(new QRubberBand(QRubberBand::Rectangle, this));
        rb_->setGeometry(QRect(select_origin_, QSize()).normalized());
        rb_->show();
    } else {
        if(rb_.get())
            rb_->hide();
        this->update();
    }
}

void MandelbrotWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (selection_)
        rb_->setGeometry(QRect(select_origin_, e->pos()).normalized());
}

void MandelbrotWidget::mouseReleaseEvent(QMouseEvent* e)
{
    selection_rectangle_ = QRect(select_origin_, e->pos()).normalized();
    selection_ = false;
}