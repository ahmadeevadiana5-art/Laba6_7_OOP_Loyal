#include "Storage6.h"
#include "Circle.h"
#include <Rectangle6.h>
#include<Triangle7.h>
#include<Square6.h>
#include<Line6.h>
#include<Ellipse6.h>
#include "group.h"
#include <algorithm>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "ShapeFactory.h"

//конструктор

Storage::Storage(int capacity) : max_capacity(capacity)
{
	shapes.reserve(capacity);

}

//Добавление фигуры
void Storage::add(std::shared_ptr<BaseShape>shape)
{
	if (shapes.size() < max_capacity)
	{
		shapes.push_back(shape); //добавляем в конец
		notifyChanges();
	}
}

//удаление объекта по индексу 
void Storage::remove(int index)
{
	if (index >= 0 && index < shapes.size())
	{
		shapes.erase(shapes.begin() + index);

	}
}

//получение объектов по индексу
std::shared_ptr<BaseShape>Storage::getObject(int index) const
{
	if (index >= 0 && index < shapes.size())
	{
		return shapes[index];
	}
	return nullptr;
}

//получение количества объектов
int Storage::getCount() const
{
	return static_cast<int>(shapes.size());
}

//удаление всех выделенных объектов
void Storage::removeSelected()
{
	std::vector<std::shared_ptr<BaseShape>>notHighlightedShapes;

	//собираем выделенные фигуры (которые мы будем удалять) 
	std::vector<std::shared_ptr<BaseShape>> selectedShapes;

	for (int i = 0; i < shapes.size(); i++)

	{
		auto currentShape = shapes[i];
		bool isShapeSelected = currentShape->isSelected();

		if (!isShapeSelected)
		{
			notHighlightedShapes.push_back(currentShape);

		}
		else
		{
			selectedShapes.push_back(currentShape);

		}

	}

	/*

	//удаляем стрелки, связанные с удаляемыми фигурами
	std::vector<std::shared_ptr<ArrowObserver>> remaningArrows;

	for (const auto& arrow : arrows)
	{
		bool keepArrow = true;

	}
	*/

	shapes = notHighlightedShapes;
	//удаляем все стрелки при удалении фигур 
	arrows.clear();
	emit treeUpdateNeeded();

}

void Storage::clear()
{
	shapes.clear(); //очищаем фигуры
	arrows.clear(); //очищаем стрелки 
	emit treeUpdateNeeded(); //TreeView
}

//4 ЛР 
void Storage::moveSelected(int dx, int dy, const QRect& bounds)
{
	for (auto& shape : shapes)
	{
		if (shape->isSelected())
		{
			shape->move(dx, dy);
			shape->adjustToBounds(bounds);

		}
	}
}

//изменение размера всех выделенных фигур 
void Storage::resizeSelected(int dw, int dh, const QRect& bounds)
{
	for (auto& shape : shapes)
	{
		if (shape->isSelected())
		{
			int newwidth = shape->getWidth() + dw;
			int newHeight = shape->getHeight() + dh;

			newwidth = std::max(10, newwidth);
			newHeight = std::max(10, newHeight);


			shape->resize(newwidth, newHeight);

			shape->adjustToBounds(bounds);

		}
	}
}

// Изменение цвета всех выделенных фигур
void Storage::changeColorSelected(const QColor& color)
{
	// Проходим по всем фигурам в хранилище
	for (auto& shape : shapes)
	{
		// Проверяем, выделена ли текущая фигура
		if (shape->isSelected())
		{
			// Устанавливаем новый цвет заливки
			shape->setfillColor(color);
			// Примечание: можно также добавить setBorderColor если нужно
		}
	}
}

// Получение списка всех выделенных фигур
const std::vector<std::shared_ptr<BaseShape>>& Storage::getSelectedShapes() const
{
	// Создаем статический вектор для возврата
	// static чтобы не создавать его каждый раз заново
	static std::vector<std::shared_ptr<BaseShape>> selectedShapes;

	// Очищаем вектор от предыдущих значений
	selectedShapes.clear();

	// Заполняем вектор только выделенными фигурами
	for (const auto& shape : shapes)
	{
		if (shape->isSelected())
		{
			selectedShapes.push_back(shape);
		}
	}

	// Возвращаем ссылку на вектор с выделенными фигурами
	return selectedShapes;
}

// Получение копии вектора всех фигур
std::vector<std::shared_ptr<BaseShape>> Storage::getShapes() const
{
	//  возвращаем копию вектора

	return shapes;
}


//===================ЛР6 
//группировка выделенных фигур
std::shared_ptr<Group> Storage::groupSelected()
{
	//получаем все выделенные фигуры
	auto selected = getAllSelectedShapes();

	if (selected.size() < 2)
	{
		return nullptr;

	}

	//создаем новую группу
	auto group = std::make_shared<Group>();

	//добавляем все выделенные фигуры в группы
	for (auto& shape : selected)
	{
		group->addShape(shape);

	}

	//удаляем фигуры из основного хранилища
	for (auto& shape : selected)
	{
		auto it = std::remove_if(shapes.begin(), shapes.end(),
			[shape](const std::shared_ptr<BaseShape>& s) {
				return s.get() == shape.get();
			});
		shapes.erase(it, shapes.end());
	}
	//добавляем группу в хранилище 
	shapes.push_back(group);

	return group;
}

//разгруппировка выделенной группы 
void Storage::ungroupSelected()
{
	//ищем выделеннную группу
	for (size_t i = 0; i < shapes.size(); i++)
	{
		auto shape = shapes[i];
		if (shape->isSelected() && shape->isGroup())
		{
			auto group = std::dynamic_pointer_cast<Group>(shape);

			if (group)
			{
				auto groupShapes = group->getShapes(); //получаем фигуры, которые находятся внутри группы
				//удаляем группу из хранилища
				shapes.erase(shapes.begin() + i);


				//добавляем фигуры обратно в хранилище
				for (auto& childShape : groupShapes)
				{
					shapes.push_back(childShape);
				}
				break; //разгруппировывем первую найденную группу ЛР6 
			}
		}
	}
}

//возвращает список ВСЕХ выделенных фигур 
std::vector<std::shared_ptr<BaseShape>> Storage::getAllSelectedShapes() const
{
	std::vector<std::shared_ptr<BaseShape>>result;

	//перебор каждой фигуры в хранилище
	for (const auto& shape : shapes)
	{
		//проверяем выделена ли фигура
		if (shape->isSelected())
		{
			result.push_back(shape);
		}
	}
	return result; //возвращаем вектор с выделенными фигурами

}

//сохранение в файл 
bool Storage::saveToFile(const QString& filename)
{
	QFile file(filename); //объект файла с указанным именем
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) //только для записии | текстовый режим 
		

	{
		return false;
	}
	QTextStream out(&file); //создаем поток для записи текста в файл

	//сохраняем количество фигур 
	out << shapes.size() << "\n";

	// Сохраняем каждую фигуру
	for (const auto& shape : shapes) 
	{
		//используем ShapeFactory для определения типа 
		//QString type = ShapeFactory::getShapeType(shape);
		//сохраняем тип
		//out << shape->getType() << "\n";
		//out << type << "\n";
		//вызываем виртуальный метод save()
		shape->save(out);
	}
	file.close();
	return true;
}

//загрузка из файла 
bool Storage::loadFromFile(const QString& filename)
{
	//открываем файл для чтения
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream in(&file);

	//очищаем текущие вершины
	shapes.clear();

	// Читаем файл построчно
	while (!in.atEnd()) {
		// Читаем строку
		QString line = in.readLine();

		// Убираем лишние пробелы в начале и конце
		line = line.trimmed();

		// Пропускаем пустые строки
		if (line.isEmpty()) {
			continue;
		}

		// Разбиваем строку на части
		QStringList parts = line.split(' ', Qt::SkipEmptyParts);
		if (parts.size() < 2) { // Должен быть минимум тип и один параметр
			continue;
		}

		// Первое слово - тип фигуры
		QString type = parts[0];

		// Создаем фигуру через фабрику
		std::shared_ptr<BaseShape> shape = ShapeFactory::createShape(type);
		if (!shape) {
			// Неизвестный тип фигуры
			continue;
		}

		// Для загрузки создаем поток из исходной строки
		QTextStream shapeStream(&line);

		// Загружаем данные в фигуру
		shape->load(shapeStream);

		// Добавляем фигуру в хранилище
		shapes.push_back(shape);
	}

	file.close();

	// Возвращаем true, если загрузили хотя бы одну фигуру
	return !shapes.empty();
}

//=====ЛР7 
void Storage::addArrow(std::shared_ptr<ArrowObserver> arrow)
{
	arrows.push_back(arrow);

}
void Storage::removeArrow(std::shared_ptr<ArrowObserver> arrow) 
{
	auto it = std::remove(arrows.begin(), arrows.end(), arrow);
	if (it != arrows.end()) 
	{
		arrows.erase(it, arrows.end());
	}
}

std::shared_ptr<ArrowObserver> Storage::createArrow(std::shared_ptr<BaseShape> source, std::shared_ptr<BaseShape> target)
{
	if (!source || !target || source == target)
	{
		return nullptr;

	}
	auto arrow = std::make_shared<ArrowObserver>(source, target);
	arrows.push_back(arrow);
	return arrow;
}

const std::vector<std::shared_ptr<ArrowObserver>>& Storage::getArrows() const
{
	return arrows;

}

void Storage::notifyChanges()
{
	emit treeUpdateNeeded();
	emit selectionChanged();

}