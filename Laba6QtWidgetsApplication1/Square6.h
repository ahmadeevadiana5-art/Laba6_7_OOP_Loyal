#ifndef SQUARE6_H
#define SQUARE6_H

#include"BaseShape6.h"

class Square : public BaseShape
{
public:
    Square(const QPoint& center = QPoint(0, 0), int size = 40);

    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;

    void move(int dx, int dy) override;
    /*
    {
        BaseShape::move(dx, dy);
    }*/

    void resize(int newWidth, int newHeight) override;


    // ЛР6 методы
    void save(QTextStream& out) const override;
    void load(QTextStream& in) override;
    std::shared_ptr<BaseShape> clone() const override;

    QString getType() const override {
        return "Square";
    }

    //ЛР7 
    QPoint getCenter() const override;
    /*
    {
        return position1;  // position1 уже центр
    }
    */
    void setCenter(const QPoint& center) override;
    /*
    {
        QPoint offset = center - position1;
        move(offset.x(), offset.y());
    }*/

private:
    int size;
};

#endif