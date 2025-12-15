#ifndef RECTANGLE6_H
#define RECTANGLE6_H

#include "BaseShape6.h"

class Rectangle : public BaseShape
{
public:
    Rectangle(const QPoint& center = QPoint(0, 0), int width = 60, int height = 40);

    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;


    //лр7 
    // Добавляем метод move()
    void move(int dx, int dy) override;
    
    // ЛР6 методы
    void save(QTextStream& out) const override;
    void load(QTextStream& in) override;
    std::shared_ptr<BaseShape> clone() const override;
    QString getType() const override
    {
        return "Rectangle";
    }

    // Переопределяем getCenter() для прямоугольника
    QPoint getCenter() const override;
    /*
    {
        return position1 + QPoint(width / 2, height / 2);
    }
    */

    void setCenter(const QPoint& center) override;
    /*
    {
        QPoint offset = center - position1;
        move(offset.x(), offset.y());
    }
    */
private:
    int rectWidth;
    int rectHeight;

};

#endif
