#include "Ellipse6.h"
#include <QPainter>
#include<QTextStream>

#include <cmath>

Ellipse::Ellipse(const QPoint& center, int width, int height)
    : BaseShape(center)
{
    this->width = width;
    this->height = height;
}


void Ellipse::draw(QPainter& painter) const
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

    QRect ellipseRect(position1.x() - width / 2, position1.y() - height / 2, width, height);
    painter.drawEllipse(ellipseRect);
}


QRect Ellipse::getBounRect() const
{
    return QRect(position1.x() - width / 2, position1.y() - height / 2, width, height);
}

bool Ellipse::contains(const QPoint& point) const
{
    // Для эллипса используем математическую проверку
    double dx = (point.x() - position1.x()) / (double)(width / 2);
    double dy = (point.y() - position1.y()) / (double)(height / 2);

    return (dx * dx + dy * dy) <= 1.0;
}

/*
void Ellipse::save(QTextStream& out) const
{
    out << "Ellipse ";
    out << position1.x() << " " << position1.y() << " ";
    out << width << " " << height << " ";
    out << fillColor.name() << " ";
    out << Colorborder.name() << "\n";
}
*/
/*
void Ellipse::load(QTextStream& in)
{
    int x, y, w, h;
    QString fillName, borderName;

    in >> x >> y >> w >> h >> fillName >> borderName;

    position1 = QPoint(x, y);
    width = w;
    height = h;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
}
*/
std::shared_ptr<BaseShape> Ellipse::clone() const
{
    auto newEllipse = std::make_shared<Ellipse>(position1, width, height);
    newEllipse->setfillColor(fillColor);
    newEllipse->setColorborder(Colorborder);
    newEllipse->setSelected(selected);
    return newEllipse;
}

void Ellipse::resize(int newWidth, int newHeight) {
    ellipseWidth = newWidth;
    ellipseHeight = newHeight;
    width = newWidth;
    height = newHeight;
}

void Ellipse::move(int dx, int dy) {
    BaseShape::move(dx, dy);
}

QPoint Ellipse::getCenter() const {
    return position1;
}

void Ellipse::setCenter(const QPoint& center) {
    QPoint offset = center - position1;
    move(offset.x(), offset.y());
}
void Ellipse::save(QTextStream& out) const
{
    out << "Ellipse " << position1.x() << " "
        << position1.y() << " "
        << width << " "
        << height << " "
        << fillColor.name() << " "
        << Colorborder.name() << " "
        << (selected ? "1" : "0") << "\n";
}
void Ellipse::load(QTextStream& in)
{
    int x, y, w, h;
    QString fillName, borderName;
    int selectedFlag;

    in >> x >> y >> w >> h >> fillName >> borderName >> selectedFlag;
    in.readLine(); // Перейти на новую строку

    position1 = QPoint(x, y);
    width = w;
    height = h;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    selected = (selectedFlag == 1);
}

