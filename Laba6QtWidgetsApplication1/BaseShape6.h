#ifndef BASESHAPE6_H
#define BASESHAPE6_H

#include<QPoint>
#include <QColor>
#include<QRect>
#include<QPainter>
#include <QTextStream> //save/load
#include <memory>
#include <vector>
#include"Observer.h"

class BaseShape : public Observable //наследуем от Observable
{
public:
	BaseShape(const QPoint& position = QPoint(0, 0));
	virtual ~BaseShape() = default;  //позволяет конкретно удалять объекты через указатеь на базовый класс


	virtual void draw(QPainter& painter)const = 0;

	//получение ограничивающего прямоугольника
	virtual QRect getBounRect()          const = 0;

	virtual bool contains(const QPoint& point)
		const = 0;


	//===========ЛР7 
	// Добавляем методы для работы с центром фигуры
	virtual QPoint getCenter() const;
	/* {
		// Базовая реализация - возвращает позицию фигуры
		// Конкретные фигуры могут переопределить этот метод
		return position1 + QPoint(width / 2, height / 2);
	}*/

	virtual void setCenter(const QPoint& center);
	/*
	{
		QPoint oldCenter = getCenter();
		QPoint offset = center - oldCenter;

		// Перемещаем фигуру на вычисленное смещение
		move(offset.x(), offset.y());
	}*/


	//Проверка попадания точки в фигуру для выделение мышью 7
	virtual void move(int dx, int dy);
		/*
	{
		QPoint oldCenter = getCenter();
		position1.rx() += dx;
		position1.ry() += dy;
		if (dx != 0 || dy != 0) {
			notifyObservers();
		}
	}
	*/
	//изменение размера
	virtual void resize(int newwidht, int newheight);

	virtual void adjustToBounds(const QRect& allowedArea);


	//для позиции
	void setPosition(const QPoint& newPosit);
	/*
	{
		int dx = newPosit.x() - position1.x();
		int dy = newPosit.y() - position1.y();
		move(dx, dy);
	}*/
	QPoint getPosition() const;
	/*
	{
		return position1;

	}*/

	//работа с цветами для заливки и границы
	virtual void setfillColor(const QColor& color);
	void setColorborder(const QColor& color);
	QColor getfillColor() const;
	QColor getColorborder() const;

	//для выделения
	void setSelected(bool selected);
	bool isSelected() const;

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	//методы контроля границ 
	bool isOutOfBounds(const QRect& allowedArea) const;


	//ЛР 6 Методы для загрузки и сохранения
	virtual void save(QTextStream& out) const = 0;
	virtual void load(QTextStream& in) = 0;
	virtual QString getType() const =0; //ЛР6 каждая фигура реализует метод
	virtual QString toString() const { return getType(); } //для представления в читаемом формате ЛР6

	//для composite Pattern (группировка)
	virtual bool isGroup()
		const
	{
		return false;
	}
	virtual std::shared_ptr<BaseShape> clone()
		const = 0;

	//для работы с Group
	virtual void addShape(std::shared_ptr<BaseShape> shape)
	{
		Q_UNUSED(shape);
	}

	virtual void removeShape(std::shared_ptr<BaseShape> shape)
	{
		Q_UNUSED(shape);
	}
	virtual std::vector<std::shared_ptr<BaseShape>> getShapes() const
	{
		return { };
	}

	

protected:
	QPoint position1; //позиция фигуры
	int width; //размеры
	int height;
	QColor fillColor; //цвет заливки
	QColor Colorborder; //цвет границы
	bool selected; //флаг выделения


};


#endif
