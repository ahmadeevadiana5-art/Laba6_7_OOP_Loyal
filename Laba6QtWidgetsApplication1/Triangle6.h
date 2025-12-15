#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BaseShape6.h"

class Triangle : public BaseShape
{
public:
    Triangle(const QPoint& center = QPoint(0, 0), int size = 40);

    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;
    void resize(int newWidth, int newHeight) override;

private:
    int size;
    QVector<QPoint> getPoints() const;
};

#endif