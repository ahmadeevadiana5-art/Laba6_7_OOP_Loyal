#include "Triangle7.h"
#include <QPainter>
#include <QPolygon>

Triangle::Triangle(const QPoint& center, int size)
    : BaseShape(center), size(size)
{
    width = size;
    height = size;
}
// Добавляем этот метод в реализацию
QPoint Triangle::getCenter() const {
    // У треугольника position1 уже является центром
    return position1;
}
void Triangle::move(int dx, int dy)
{
    // Вызываем метод базового класса для уведомления наблюдателей
    BaseShape::move(dx, dy);
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

void Triangle::save(QTextStream& out) const
{
    out << "Triangle "
        << position1.x() << " "
        << position1.y() << " "
        << size << " "
        << fillColor.name() << " "
        << Colorborder.name() << " "
        << (selected ? "1" : "0") << "\n";
}

void Triangle::load(QTextStream& in)
{
    QString type;
    int x, y, s;
    QString fillName, borderName;
    int selectedFlag;

    // Читаем все параметры в том же порядке, что и сохраняли
    in >> type >> x >> y >> s >> fillName >> borderName >> selectedFlag;

    position1 = QPoint(x, y);
    size = s;
    width = size;
    height = size;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    selected = (selectedFlag == 1);

    // Важно: переходим на новую строку
    //in.readLine();
}

std::shared_ptr<BaseShape> Triangle::clone() const
{
    auto newTriangle = std::make_shared<Triangle>(position1, size);
    newTriangle->setfillColor(fillColor);
    newTriangle->setColorborder(Colorborder);
    newTriangle->setSelected(selected);
    return newTriangle;
}

QString Triangle::getType() const
{
    return "Triangle";
}

void Triangle::setCenter(const QPoint& center) {
    QPoint offset = center - position1;
    move(offset.x(), offset.y());
}