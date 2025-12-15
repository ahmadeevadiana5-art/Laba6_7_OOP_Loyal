#include "Rectangle6.h"
#include <QPainter>
#include<QTextStream>

Rectangle::Rectangle(const QPoint& center, int width, int height)
    : BaseShape(center), rectWidth(width), rectHeight(height)
{
    this->width = width;
    this->height = height;
}

void Rectangle::draw(QPainter& painter) const
{
    painter.setBrush(fillColor);
    QPen pen(Colorborder);
    pen.setWidth(2);

    //обрабатываем выделенное состояние
    if (selected)
    {
        pen.setColor(Qt::red);
        pen.setWidth(3);
    }

    painter.setPen(pen);

    QRect rect(position1.x() - width / 2, position1.y() - height / 2, width, height);
    painter.drawRect(rect);
    // Х - координата левого верхнего угла 
}

//реализация метода ограничивающий прямоугольника
QRect Rectangle::getBounRect() const
{
    //создаем и возвращаем тот же прямоугольник который используется для рисования
    return QRect(position1.x() - width / 2, position1.y() - height / 2, width, height);
}
//метод для попадания точки в фигуру
bool Rectangle::contains(const QPoint& point) const
{
    QRect bounds = getBounRect();
    return bounds.contains(point);
}

//сохраняет в файл
void Rectangle::save(QTextStream& out) const
{
    out << "Rectangle "
        << position1.x() << " "
        << position1.y() << " "
        << width << " "
        << height << " "
        << fillColor.name() << " "
        << Colorborder.name() << " "
        << (selected ? "1" : "0") << "\n";

}


//загружает из файла
void Rectangle::load(QTextStream& in)
{
    QString type;
    int x, y, w, h;
    QString fillName, borderName;
    int selectedFlag;

    in >> type >> x >> y >> w >> h >> fillName >> borderName >> selectedFlag;

    position1 = QPoint(x, y);
    width = w;
    height = h;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    selected = (selectedFlag == 1);

    //in.readLine();
}

//создает копию для Composite
std::shared_ptr<BaseShape> Rectangle::clone() const
{
    auto newRect = std::make_shared<Rectangle>(position1, width, height);
    newRect->setfillColor(fillColor);
    newRect->setColorborder(Colorborder);
    newRect->setSelected(selected);
    return newRect;
}

void Rectangle::move(int dx, int dy)
{
    BaseShape::move(dx, dy);
}

QPoint Rectangle::getCenter() const
{
    return position1;

}

void Rectangle::setCenter(const QPoint& center)
{
    QPoint offset = center - position1;
    move(offset.x(), offset.y());

}