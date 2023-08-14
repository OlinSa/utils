#ifndef MIXING_H
#define MIXING_H

#include <QColor>
#include <QObject>
class Mixing : public QObject {
  Q_OBJECT
  Q_ENUMS(BALL_COLOR)
  Q_PROPERTY(
      unsigned int number READ Number WRITE setNumber NOTIFY NumberChanged)
public:
  explicit Mixing(QObject *parental = nullptr);
  enum BALL_COLOR {
    BALL_COLOR_BLUE,
    BALL_COLOR_GREEN,
    BALL_COLOR_YELLOW,
  };
  unsigned int Number() const;
  void setNumber(const unsigned int &number);

signals:
  void colorChanged(const QColor &color);
  void NumberChanged();
public slots:
  void start();
  void start(BALL_COLOR ballColor);
  void stop();

private:
  unsigned int number_;
};

#endif // MIXING_H
