#ifndef LINE6_H
#define LINE6_H

#include "BaseShape6.h"

class Line : public BaseShape
{
public:
    Line(const QPoint& start = QPoint(0, 0), const QPoint& end = QPoint(40, 40));

    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;
    void move(int dx, int dy) override;
    void resize(int newWidth, int newHeight) override;

    double detLength() const;

    // ЛР6 методы
    void save(QTextStream& out) const override;
    void load(QTextStream& in) override;
    std::shared_ptr<BaseShape> clone() const override;
    QString getType() const override
    {
        return "Line";

    }



    //======= ЛР7: Методы для паттерна Observer
    QPoint getCenter() const override;
    /*
    {
        // Центр линии - середина между начальной и конечной точками
        return QPoint(
            (position1.x() + endPoint.x()) / 2,
            (position1.y() + endPoint.y()) / 2
        );
    }
    */

    void setCenter(const QPoint& center) override;
    /*
    {
        QPoint currentCenter = getCenter();
        QPoint offset = center - currentCenter;
        move(offset.x(), offset.y());
    }*/


    // Геттеры для точек
    QPoint getStartPoint() const { return position1; }
    QPoint getEndPoint() const { return endPoint; }
    void setEndPoint(const QPoint& end) { endPoint = end; }

private:
    QPoint endPoint;
    bool containsPoint(const QPoint& point, const QPoint& lineStart, const QPoint& lineEnd, int tolerance = 5) const;
};

#endif