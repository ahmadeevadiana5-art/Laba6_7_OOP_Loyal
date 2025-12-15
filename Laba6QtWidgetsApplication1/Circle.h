
#ifndef CIRCLE_H
#define CIRCLE_H
#include<QPoint>
#include <QColor>
#include<QRect>
#include<QPainter>
#include<QTextStream>
#include<memory>
#include "BaseShape6.h"

class Circle : public BaseShape
{
public:
	Circle(const QPoint& center = QPoint(0, 0), int radius = 20);

	void draw(QPainter& painter) const override;
	QRect getBounRect() const override;
	bool contains(const QPoint& point) const override;

	// Добавляем метод move()
	void move(int dx, int dy) override;
	/*
	{
		BaseShape::move(dx, dy);  // Вызываем базовый для уведомления наблюдателей

	}*/
	void resize(int newWidth, int newHeight)
		override;

	//ЛР 6
	void save(QTextStream& out) const override;
	void load(QTextStream& in) override;
	std::shared_ptr<BaseShape> clone() const override;
	int getRadius() const;
	QString getType() const override
	{
		return "Circle";
	}

	//=======ЛР7 
	// Переопределяем getCenter() для круга
	QPoint getCenter() const override;
	/*
	{
		return position1 + QPoint(radius, radius);  // Центр круга
	}
	*/

	void setCenter(const QPoint& center) override;
	/*
	{
		QPoint offset = center - getCenter();
		move(offset.x(), offset.y());
	}
	*/
private:
	int radius;

};

#endif 