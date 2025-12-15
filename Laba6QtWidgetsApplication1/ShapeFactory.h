#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H


#include <memory>
#include <QString>
#include "BaseShape6.h"

class ShapeFactory
{
public: 
	//FACTORY METHOD для создания фигур 
	static std::shared_ptr<BaseShape> createShape(const QString& shapeType);

	//определение типа фигуры
	static QString getShapeType(std::shared_ptr<BaseShape> shape);
}; 

#endif
