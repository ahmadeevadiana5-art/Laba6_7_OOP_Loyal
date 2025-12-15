#include "Square6.h"
#include<QPainter>
#include<algorithm>

#include<QTextStream>

//создаем конструктор класса
Square::Square(const QPoint& center, int size)
    : BaseShape(center), size(size)
{
    width = size;
    height = size;

}

void Square::draw(QPainter& painter) const
{
    painter.setBrush(fillColor); //кисть для заливки фигуры
    QPen pen(Colorborder);
    pen.setWidth(2);
    //проверяем выделение фигуры
    if (selected)
    {
        pen.setColor(Qt::red);
        pen.setWidth(3);
    }

    painter.setPen(pen);
    //создаем прямоугольник для рисования
    QRect squareRect(position1.x() - size / 2, position1.y() - size / 2, size, size);
    painter.drawRect(squareRect);
}

//возвращает объект ограничивающий прямоугольник
QRect Square::getBounRect() const
{
    return QRect(position1.x() - size / 2, position1.y() - size / 2, size, size);
}

//проверяем находится ли точка внутри квадрата
bool Square::contains(const QPoint& point) const
{
    QRect bounds = getBounRect();
    return bounds.contains(point);
}

//метод для изменения размера квадрата
void Square::resize(int newWidth, int newHeight)
{
    // Для квадрата сохраняем одинаковые размеры
    size = (newWidth + newHeight) / 2;
    size = std::max(20, size);
    //обновляем
    width = size;
    height = size;
}

void Square::save(QTextStream& out) const
{
    out << "Square "
        << position1.x() << " "
        << position1.y() << " "
        << width << " "  // Для квадрата width = height = size
        << fillColor.name() << " "
        << Colorborder.name() << " "
        << (selected ? "1" : "0") << "\n";
}

void Square::load(QTextStream& in)
{
    QString type;
    int x, y, size;
    QString fillName, borderName;
    int selectedFlag;

    in >> type >> x >> y >> size >> fillName >> borderName >> selectedFlag;

    position1 = QPoint(x, y);
    width = size;
    height = size;  // Квадрат
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    selected = (selectedFlag == 1);

    //in.readLine();
}

std::shared_ptr<BaseShape> Square::clone() const
{
    auto newSquare = std::make_shared<Square>(position1, size);
    newSquare->setfillColor(fillColor);
    newSquare->setColorborder(Colorborder);
    newSquare->setSelected(selected);
    return newSquare;
}

//====ЛР7 
void Square::move(int dx, int dy) {
    BaseShape::move(dx, dy);
}

QPoint Square::getCenter() const {
    return position1;
}

void Square::setCenter(const QPoint& center) {
    QPoint offset = center - position1;
    move(offset.x(), offset.y());
}