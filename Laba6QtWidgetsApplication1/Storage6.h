#ifndef STORAGE6_H
#define STORAGE6_H


#include "BaseShape6.h"
#include "ArrowShape.h"
#include "group.h"
#include<QPoint>
#include <QColor>
#include<QRect>
#include<QPainter>
#include<memory>
#include<vector>
#include<QString>
#include <QObject>



class Storage : public QObject, public Observable
{
	Q_OBJECT

private:
	std::vector<std::shared_ptr<BaseShape>> shapes; //указатель на объект типа AbstractShape

	int max_capacity; //максимальное количество фигу, которые можно хранить

	//вектор для хранения стрелок 
	//std::vector<std::shared_ptr<ArrowObserver>> arrows;

	
public:

	//конструктор с начальной емкостью
	Storage(int capacity = 100);

	//Добавление фигуры
	void add(std::shared_ptr<BaseShape>shape);

	//удаление объекта по индексу 
	void remove(int index);

	//получение объектов по индексу
	std::shared_ptr<BaseShape>getObject(int index) const;


	//получение количества объектов
	int getCount() const;

	//удаление всех выделенных объектов
	void removeSelected();


	void clear();

	//4 ЛР 
	//перемещение выделенных фигур с установленным контролем границ
	void moveSelected(int dx, int dy, const QRect& bounds);

	//изменения размера выделенных фигур
	void resizeSelected(int dw, int dh, const QRect& bounds);
	//изменение цвета выделенных фигур 
	void changeColorSelected(const QColor& color);

	// Получение списка всех выделенных фигур (для отладки и расширения функциональности)
	const std::vector<std::shared_ptr<BaseShape>>& getSelectedShapes() const;

	// Получение копии вектора всех фигур (для отрисовки)
	std::vector<std::shared_ptr<BaseShape>> getShapes() const;

	//ЛР 6 
	//группировка выделенных фигур 
	std::shared_ptr<Group> groupSelected();

	//разгруппировка выделенной группы
	void ungroupSelected();
	//сохранение в файл 
	bool saveToFile(const QString& filename);

	//загрузка из файла 
	bool loadFromFile(const QString& filename);

	//получение всех выделенных фигу 
	std::vector<std::shared_ptr<BaseShape>> getAllSelectedShapes() const;


	//========ЛР7 
	//void addArrow(std::shared_ptr<ArrowObserver> arrow);
	//void removeArrow(std::shared_ptr<ArrowObserver> arrow);
	//std::shared_ptr<ArrowObserver> createArrow(std::shared_ptr<BaseShape> source, std::shared_ptr<BaseShape> target);
	//const std::vector<std::shared_ptr<ArrowObserver>>& getArrows() const;

	//методы для работы с выделением 
	std::shared_ptr<ArrowShape> createArrow(std::shared_ptr<BaseShape> source,
		std::shared_ptr<BaseShape> target);

	std::vector<std::shared_ptr<ArrowShape>> getArrows() const;

	void restoreArrowConnections();
	
	void clearSelection();
	void selectShape(std::shared_ptr<BaseShape> shape);
	void deselectShape(std::shared_ptr<BaseShape> shape);


	void notifyChanges()
	{
		emit treeUpdateNeeded();
		emit selectionChanged();
	}


signals: 
	void treeUpdateNeeded();      // Сигнал для обновления дерева
	void selectionChanged();
private:
	int findObjectIndex(std::shared_ptr<BaseShape> shape) const;
	
};


#endif