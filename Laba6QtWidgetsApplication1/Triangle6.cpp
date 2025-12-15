#include "Triangle.h"
#include <QPainter>
#include <QPolygon>

Triangle::Triangle(const QPoint& center, int size)
    : BaseShape(center), size(size)
{
    width = size;
    height = size;
}

void Triangle::draw(QPainter& painter) const
{
    painter.setBrush(fillColor);
    QPen pen(Colorborder);
    pen.setWidth(2);

    if (selected)
    {
        pen.setColor(Qt::red);
        pen.setWidth(3);
    }

    painter.setPen(pen);

    QVector<QPoint> points = getPoints();
    QPolygon polygon(points);
    painter.drawPolygon(polygon);
}

QRect Triangle::getBounRect() const
{
    return QRect(position1.x() - size / 2, position1.y() - size / 2, size, size);
}

bool Triangle::contains(const QPoint& point) const
{
    QVector<QPoint> points = getPoints();
    QPolygon polygon(points);
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

void Triangle::resize(int newWidth, int newHeight)
{
    size = (newWidth + newHeight) / 2;
    size = std::max(20, size);

    width = size;
    height = size;
}

QVector<QPoint> Triangle::getPoints() const
{
    QVector<QPoint> points;

    // Вершины равностороннего треугольника
    points.append(QPoint(position1.x(), position1.y() - size / 2)); // Верхняя вершина
    points.append(QPoint(position1.x() - size / 2, position1.y() + size / 2)); // Левая нижняя
    points.append(QPoint(position1.x() + size / 2, position1.y() + size / 2)); // Правая нижняя

    return points;
}