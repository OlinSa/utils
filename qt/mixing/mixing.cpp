#include "mixing.h"
#include <QDebug>

Mixing::Mixing(QObject *parent) : QObject(parent) {}

void Mixing::start() {
    emit colorChanged(Qt::blue);
}

void Mixing::start(BALL_COLOR ballcolor) {
  qDebug() << "start";
  QColor color = Qt::blue;
  switch (ballcolor) {
  case BALL_COLOR_BLUE:
    color = Qt::blue;
    break;
  case BALL_COLOR_GREEN:
    color = Qt::green;
    break;
  case BALL_COLOR_YELLOW:
    color = Qt::yellow;
    break;
  }

  emit colorChanged(color);
}

unsigned int Mixing::Number() const { return number_; }
void Mixing::setNumber(const unsigned int &number) {
  if (number != number_) {
    number_ = number;
    emit NumberChanged();
  }
}

void Mixing::stop() { qDebug() << "color changed"; }
