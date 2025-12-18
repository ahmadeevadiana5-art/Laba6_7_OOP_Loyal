#include "Circle.h"
#include<QPainter>
#include <QTextStream>


//конструктор 
Circle::Circle(const QPoint& center, int radius1)
    : BaseShape(center), radius(radius1)
{
    width = radius * 2;
    height = radius * 2;
}

//цвет заливки
void Circle::draw(QPainter& painter) const
{
    painter.setBrush(fillColor);
    QPen pen(Colorborder);
    pen.setWidth(2);
    //если фигура выделена меняем цвет и увеливаем толщину
    if (selected)
    {
        pen.setColor(Qt::red);
        pen.setWidth(3);
    }
    painter.setPen(pen);
    QRect circleRect(position1.x() - radius, position1.y() - radius,
        radius * 2, radius * 2);
    painter.drawEllipse(circleRect);
}


QRect Circle::getBounRect() const
{
    return QRect(position1.x() - radius, position1.y() - radius,
        radius * 2, radius * 2);
}


bool Circle::contains(const QPoint& point) const
{
    int dx = point.x() - position1.x();
    int dy = point.y() - position1.y();
    return (dx * dx + dy * dy) <= (radius * radius);
}

void Circle::resize(int newWidth, int newHeight)
{
    // Для круга сохраняем пропорции - берем среднее значение
    int size = (newWidth + newHeight) / 2;

    // Ограничиваем минимальный размер
    size = std::max(20, size);

    radius = size / 2;
    width = size;
    height = size;
}


//ЛР 6 Сохранение круга в файл 
void Circle::save(QTextStream& out) const
{
    BaseShape::save(out);
    out << " " << radius << "\n";
    /*
    out << "Circle "
        << position1.x() << " "
        << position1.y() << " "
        << radius << " "
        << fillColor.name() << " "
        << Colorborder.name() << " "
        << (selected ? "1" : "0") << "\n";*/
}




//загрузка из файла

void Circle::load(QTextStream& in)
{
    BaseShape::load(in);
    in >> radius;

    /*
    QString type;
    int x, y, r;
    QString fillName, borderName;
    int selectedFlag;


    in >>type>> x >> y >> r >> fillName >> borderName >> selectedFlag;

    position1 = QPoint(x, y);
    radius = r;
    width = radius * 2;    // Вычисляем width
    height = radius * 2;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    selected = (selectedFlag == 1);
    */
    //in.readLine();

}

//создаем копию круга 
std::shared_ptr<BaseShape> Circle::clone() const
{
    auto newCircle = std::make_shared<Circle>(position1, radius);
    newCircle->setfillColor(fillColor);
    newCircle->setColorborder(Colorborder);
    newCircle->setSelected(selected);
    newCircle->width = width;
    newCircle->height = height;
    return newCircle;
}
int Circle::getRadius() const
{
    return radius;
}




//=====ЛР7 
void Circle::move(int dx, int dy) {
    BaseShape::move(dx, dy);
}

QPoint Circle::getCenter() const {
    return position1;
}

void Circle::setCenter(const QPoint& center) {
    QPoint offset = center - position1;
    move(offset.x(), offset.y());
}
 