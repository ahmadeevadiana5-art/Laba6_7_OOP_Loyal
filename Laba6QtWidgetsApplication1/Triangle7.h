#ifndef TRIANGLE7_H
#define TRIANGLE7_H

#include "BaseShape6.h"
#include<QTextStream>

class Triangle : public BaseShape
{
public:
    Triangle(const QPoint& center = QPoint(0, 0), int size = 40);

    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;
    void resize(int newWidth, int newHeight) override;
    // Добавляем метод move()
    void move(int dx, int dy) override;

    // Переопределяем getCenter() для треугольника
    QPoint getCenter() const override;
    void setCenter(const QPoint& center) override;


    //ЛР6 
    void save(QTextStream& out) const override;
    void load(QTextStream& in) override;
    std::shared_ptr<BaseShape> clone() const override;

    //метод для получение типа фигуры
    QString getType() const override;



private:
    int size;
    QVector<QPoint> getPoints() const;
};

#endif