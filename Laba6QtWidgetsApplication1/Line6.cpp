#include "Line6.h"
#include <QPainter>
#include <cmath>
#include<QTextStream>

Line::Line(const QPoint& start, const QPoint& end)
    : BaseShape(start), endPoint(end)
{

    //размеры ограничивающего прямоугольника
    width = std::abs(end.x() - start.x());
    height = std::abs(end.y() - start.y());
}

void Line::draw(QPainter& painter) const
{
    QPen pen(Colorborder);
    pen.setWidth(2);

    if (selected)
    {
        pen.setColor(Qt::red);
        pen.setWidth(3);
    }

    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush); // Линия без заливки

    painter.drawLine(position1, endPoint);
}

QRect Line::getBounRect() const
{
    int left = std::min(position1.x(), endPoint.x());
    int top = std::min(position1.y(), endPoint.y());
    int right = std::max(position1.x(), endPoint.x());
    int bottom = std::max(position1.y(), endPoint.y());

    return QRect(left, top, right - left, bottom - top);
}

bool Line::contains(const QPoint& point) const
{
    return containsPoint(point, position1, endPoint, 5);
}

/*
void Line::move(int dx, int dy)
{
    /*
    position1.setX(position1.x() + dx);
    position1.setY(position1.y() + dy);
    endPoint.setX(endPoint.x() + dx);
    endPoint.setY(endPoint.y() + dy);
    */
/*
    BaseShape::move(dx, dy);
    // Также перемещаем конечную точку
    endPoint.setX(endPoint.x() + dx);
    endPoint.setY(endPoint.y() + dy);

    // Обновляем размеры (хотя они могут не меняться при перемещении)
    width = std::abs(endPoint.x() - position1.x());
    height = std::abs(endPoint.y() - position1.y());

}
*/

void Line::resize(int newWidth, int newHeight)
{
    // Вычисляем центр линии
    QPoint center((position1.x() + endPoint.x()) / 2,
        (position1.y() + endPoint.y()) / 2);

    // Вычисляем текущий вектор направления
    double dirX = endPoint.x() - position1.x();
    double dirY = endPoint.y() - position1.y();
    double currentLength = std::sqrt(dirX * dirX + dirY * dirY);

    // Если линия очень короткая, создаем горизонтальное направление
    if (currentLength < 0.1) {
        dirX = 1.0;
        dirY = 0.0;
        currentLength = 1.0;
    }

    // Нормализуем вектор
    double unitX = dirX / currentLength;
    double unitY = dirY / currentLength;

    // Определяем, какое изменение преобладает
    bool horizontalChange = std::abs(newWidth - width) > std::abs(newHeight - height);

    double newLength;
    if (horizontalChange) {
        // Горизонтальное изменение - используем изменение ширины
        newLength = currentLength + (newWidth - width);
    }
    else {
        // Вертикальное изменение - используем изменение высоты
        newLength = currentLength + (newHeight - height);
    }

    // Ограничиваем минимальную длину
    newLength = std::max(10.0, newLength);

    // Вычисляем новые конечные точки относительно центра
    double halfLength = newLength / 2.0;
    position1.setX(center.x() - unitX * halfLength);
    position1.setY(center.y() - unitY * halfLength);
    endPoint.setX(center.x() + unitX * halfLength);
    endPoint.setY(center.y() + unitY * halfLength);

    // Обновляем размеры
    width = std::abs(endPoint.x() - position1.x());
    height = std::abs(endPoint.y() - position1.y());
}

/*ПОСЛЕДНИЙ ВАРИАНТ
//  текущее направление линии (вектор от начальной к конечной точке)
double dirX = endPoint.x() - position1.x();
double dirY = endPoint.y() - position1.y();

// текущая длину линии расстояние между двумя точками
double currentLength = std::sqrt(dirX * dirX + dirY * dirY);

// Если линия длины0 , создаем горизонтальную линию
if (currentLength < 0.1) {
    dirX = 1.0;
    dirY = 0.0;
    currentLength = 1.0;
}
// (делаем длину = 1)
double unitX = dirX / currentLength;
double unitY = dirY / currentLength;

// Вычисляем новую длину на основе изменения размеров
// Используем среднее значение из изменений ширины и высоты
double lengthChange = (newWidth - width + newHeight - height) / 2.0;
double newLength = currentLength + lengthChange;

// Ограничиваем минимальную длину
newLength = std::max(10.0, newLength);

// Вычисляем новую конечную точку
endPoint.setX(position1.x() + unitX * newLength);
endPoint.setY(position1.y() + unitY * newLength);

// Обновляем размеры ограничивающего прямоугольника
width = std::abs(endPoint.x() - position1.x());
height = std::abs(endPoint.y() - position1.y());

*/

/*

// Для линии изменяем конечную точку
int dx = newWidth - width;
int dy = newHeight - height;

endPoint.setX(endPoint.x() + dx);
endPoint.setY(endPoint.y() + dy);

width = newWidth;
height = newHeight;
*/

// Также нужно добавить метод detLength():
double Line::detLength() const
{
    int dx = endPoint.x() - position1.x();
    int dy = endPoint.y() - position1.y();
    return std::sqrt(dx * dx + dy * dy);
}


bool Line::containsPoint(const QPoint& point, const QPoint& lineStart, const QPoint& lineEnd, int tolerance) const
{
    // Проверка расстояния от точки до линии
    double A = point.x() - lineStart.x();
    double B = point.y() - lineStart.y();
    double C = lineEnd.x() - lineStart.x();
    double D = lineEnd.y() - lineStart.y();

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;

    if (len_sq != 0)
        param = dot / len_sq;

    double xx, yy;

    if (param < 0) {
        xx = lineStart.x();
        yy = lineStart.y();
    }
    else if (param > 1) {
        xx = lineEnd.x();
        yy = lineEnd.y();
    }
    else {
        xx = lineStart.x() + param * C;
        yy = lineStart.y() + param * D;
    }

    double dx = point.x() - xx;
    double dy = point.y() - yy;

    return std::sqrt(dx * dx + dy * dy) <= tolerance;
}


void Line::save(QTextStream& out) const
{
    out << "Line ";
    out << position1.x() << " " << position1.y() << " ";
    out << endPoint.x() << " " << endPoint.y() << " ";
    out << fillColor.name() << " ";
    out << Colorborder.name() << "\n";
}

void Line::load(QTextStream& in)
{
    int x1, y1, x2, y2;
    QString fillName, borderName;

    in >> x1 >> y1 >> x2 >> y2 >> fillName >> borderName;

    position1 = QPoint(x1, y1);
    endPoint = QPoint(x2, y2);
    width = std::abs(x2 - x1);
    height = std::abs(y2 - y1);
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
}

std::shared_ptr<BaseShape> Line::clone() const
{
    auto newLine = std::make_shared<Line>(position1, endPoint);
    newLine->setfillColor(fillColor);
    newLine->setColorborder(Colorborder);
    newLine->setSelected(selected);
    return newLine;
}

/*
// ДОБАВЛЯЕМ МЕТОДЫ ДЛЯ PATTERN OBSERVER:


QPoint Line::getCenter() const {
    // Центр линии - середина между начальной и конечной точками
    return QPoint(
        (position1.x() + endPoint.x()) / 2,
        (position1.y() + endPoint.y()) / 2
    );
}

void Line::setCenter(const QPoint& center) {
    QPoint currentCenter = getCenter();
    QPoint offset = center - currentCenter;
    move(offset.x(), offset.y());
}
*/

//====ЛР7 
void Line::move(int dx, int dy) 
{
    BaseShape::move(dx, dy);
    endPoint.rx() += dx;
    endPoint.ry() += dy;
}

QPoint Line::getCenter() const
{
    return QPoint((position1.x() + endPoint.x()) / 2,
        (position1.y() + endPoint.y()) / 2);
}

void Line::setCenter(const QPoint& center)
{
    QPoint currentCenter = getCenter();
    QPoint offset = center - currentCenter;
    move(offset.x(), offset.y());
}