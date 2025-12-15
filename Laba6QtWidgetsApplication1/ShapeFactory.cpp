#include "ShapeFactory.h"
#include "Circle.h"
#include "Rectangle6.h"
#include "Square6.h"
#include "Triangle7.h"
#include "Ellipse6.h"
#include "Line6.h"
#include "Group.h"
#include<QDebug>



std::shared_ptr<BaseShape> ShapeFactory::createShape(const QString& shapeType)
{
	if (shapeType == "Circle")
	{
		return std::make_shared<Circle>();

	}
	else if (shapeType == "Rectangle")
	{
		return std::make_shared<Rectangle>();
	}
	else if (shapeType == "Square")
	{
		return std::make_shared<Square>();
	}
	else if (shapeType == "Triangle")
	{
		return std::make_shared<Triangle>();
	}
	else if (shapeType == "Ellipse")
	{
		return std::make_shared<Ellipse>();
	}
	else if (shapeType == "Line")
	{
		return std::make_shared<Line>();
	}
	else if (shapeType == "Group")
	{
		return std::make_shared<Group>();
	}
	return nullptr;

}

//метод getShapeType в классе ShapeFactory определяет тип фигур по указателю на базовый класс BaseShape
QString ShapeFactory::getShapeType(std::shared_ptr<BaseShape> shape)
{
	if (std::dynamic_pointer_cast<Circle>(shape)) {
		return "Circle";
	}
	else if (std::dynamic_pointer_cast<Rectangle>(shape)) {
		return "Rectangle";
	}
	else if (std::dynamic_pointer_cast<Square>(shape)) {
		return "Square";
	}
	else if (std::dynamic_pointer_cast<Triangle>(shape)) {
		return "Triangle";
	}
	else if (std::dynamic_pointer_cast<Ellipse>(shape)) {
		return "Ellipse";
	}
	else if (std::dynamic_pointer_cast<Line>(shape)) {
		return "Line";
	}
	else if (std::dynamic_pointer_cast<Group>(shape)) {
		return "Group";
	}

	return "Unknown";
}