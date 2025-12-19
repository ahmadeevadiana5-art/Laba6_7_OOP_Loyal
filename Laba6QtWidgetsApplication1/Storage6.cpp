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
#include "ArrowShape.h"

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
	//arrows.clear();
	emit treeUpdateNeeded();

}

void Storage::clear()
{
	shapes.clear(); //очищаем фигуры
	//arrows.clear(); //очищаем стрелки 
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

	//создаем новую группу COMPOSITE
	auto group = std::make_shared<Group>();

	//добавляем все выделенные фигуры в группы
	for (auto& shape : selected)
	{
		group->addShape(shape);

	}

	//удаляем фигуры из основного хранилища
	for (auto& shape : selected)
	{
		//находим и удаляем фигуру из основного контейнера 
		auto it = std::remove_if(shapes.begin(), shapes.end(),
			[shape](const std::shared_ptr<BaseShape>& s) {
				//сравниваем указатели если это та же фигура, то удаляем
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
			result.push_back(shape); //добавляем выделенную фигуру
		}
	}
	return result; //возвращаем вектор с выделенными фигурами

}



/*
//сохранение в файл 
bool Storage::saveToFile(const QString& filename)
{
	QFile file(filename); //объект файла с указанным именем

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) //только для записии | текстовый режим 
	{
		return false;
	}

	QTextStream out(&file); //создаем поток для записи текста в файл

	
	for (int i = 0; i < shapes.size(); i++)
	{

		if (shapes[i]->getType() == "Arrow")
		{
			if (auto arrow = std::dynamic_pointer_cast<ArrowShape>(shapes[i]))
			{
				// Обновляем ID объектов
				int sourceIndex = findObjectIndex(arrow->getSource());
				int targetIndex = findObjectIndex(arrow->getTarget());
				arrow->setSourceId(sourceIndex);
				arrow->setTargetId(targetIndex);
			}
		}
	}
	out << shapes.size() << "\n";

	for (const auto& shape : shapes)
	{
		if (shape->getType() == "Group")
		{
			if (auto group = std::dynamic_pointer_cast<Group>(shape))
			{
				// Сохраняем метку начала группы
				out << "GROUP_BEGIN ";
				out << group->getShapes().size() << "\n";

				// Сохраняем саму группу (ее свойства)
				group->save(out);

				// Сохраняем фигуры внутри группы
				auto children = group->getShapes();
				for (const auto& child : children)
				{
					child->save(out);
				}

				out << "GROUP_END\n";
				continue; // Пропускаем обычное сохранение
			}
		}
		shape->save(out);
	}




	file.close();
	return true;
}
*/
bool Storage::saveToFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream out(&file);

	// Обновляем ID для стрелок
	for (int i = 0; i < shapes.size(); i++)
	{
		if (shapes[i]->getType() == "Arrow")
		{
			if (auto arrow = std::dynamic_pointer_cast<ArrowShape>(shapes[i]))
			{
				int sourceIndex = findObjectIndex(arrow->getSource());
				int targetIndex = findObjectIndex(arrow->getTarget());
				arrow->setSourceId(sourceIndex);
				arrow->setTargetId(targetIndex);
			}
		}
	}

	// Сохраняем количество корневых объектов
	out << shapes.size() << "\n";

	for (const auto& shape : shapes)
	{
		// Если это группа
		if (shape->getType() == "Group")
		{
			if (auto group = std::dynamic_pointer_cast<Group>(shape))
			{
				// Сохраняем метку группы и количество детей
				auto children = group->getShapes();
				out << "GROUP " << children.size() << "\n";

				// Сохраняем детей группы (НО НЕ САМУ ГРУППУ!)
				for (const auto& child : children)
				{
					child->save(out);
				}
			}
		}
		else
		{
			// Обычная фигура (не группа)
			shape->save(out);
		}
	}

	file.close();
	return true;
}


bool Storage::loadFromFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream in(&file);
	shapes.clear();

	// Читаем количество объектов
	int totalObjects;
	in >> totalObjects;
	in.readLine(); // Переходим на следующую строку

	for (int i = 0; i < totalObjects; i++)
	{
		QString line = in.readLine();
		if (line.isNull()) break;

		line = line.trimmed();
		if (line.isEmpty()) continue;

		QStringList parts = line.split(' ', Qt::SkipEmptyParts);
		if (parts.isEmpty()) continue;

		QString type = parts[0];

		// Если это группа
		if (type == "GROUP")
		{
			// Создаем группу
			auto group = std::make_shared<Group>();
			int childCount = parts[1].toInt();

			// Читаем детей группы
			for (int j = 0; j < childCount; j++)
			{
				QString childLine = in.readLine();
				if (childLine.isNull()) break;

				childLine = childLine.trimmed();
				if (childLine.isEmpty()) continue;

				QStringList childParts = childLine.split(' ', Qt::SkipEmptyParts);
				if (childParts.isEmpty()) continue;

				QString childType = childParts[0];
				std::shared_ptr<BaseShape> child = ShapeFactory::createShape(childType);

				if (child)
				{
					QTextStream childStream(&childLine);
					child->load(childStream);
					group->addShape(child);
				}
			}

			// Добавляем группу в хранилище
			shapes.push_back(group);
		}
		else
		{
			// Обычная фигура
			std::shared_ptr<BaseShape> shape = ShapeFactory::createShape(type);

			if (shape)
			{
				QTextStream shapeStream(&line);
				shape->load(shapeStream);
				shapes.push_back(shape);
			}
		}
	}

	file.close();
	restoreArrowConnections();
	notifyChanges();
	return !shapes.empty();
}



/*
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

	//временное хранение загруженных фигур 
	//std::vector<std::shared_ptr<BaseShape>> loadedShapes;

	std::vector<std::shared_ptr<BaseShape>> loadedShapes;
	std::stack<std::shared_ptr<Group>> groupStack;

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

		if (shape)
		{
			// Для загрузки создаем поток из исходной строки
			QTextStream shapeStream(&line);

			// Загружаем данные в фигуру
			shape->load(shapeStream);

			// Добавляем фигуру в хранилище
			//shapes.push_back(shape);
			if (currentGroup)
			{
				groupChildren.push_back(shape);
			}
			else
			{
				shapes.push_back(shape);

			}
		}

	}



	file.close();

	

	restoreArrowConnections();

	notifyChanges();
	// Возвращаем true, если загрузили хотя бы одну фигуру
	return !shapes.empty();
}
*/



//=====ЛР7 
/*
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

}*/



std::shared_ptr<ArrowShape> Storage::createArrow(std::shared_ptr<BaseShape> source,
	std::shared_ptr<BaseShape> target)
{
	if (!source || !target || source == target)
	{
		return nullptr;
	}

	//создаем объект ArrowShape
	auto arrow = std::make_shared<ArrowShape>(source, target);
	// Устанавливаем ID объектов для сохранения
	int sourceIndex = findObjectIndex(source);
	int targetIndex = findObjectIndex(target);
	arrow->setSourceId(sourceIndex);
	arrow->setTargetId(targetIndex);

	//добавлем в контейнер
	shapes.push_back(arrow);
	notifyChanges();
	return arrow;
}

//Поиск индекса объекта
int Storage::findObjectIndex(std::shared_ptr<BaseShape> shape) const
{
	for (int i = 0; i < shapes.size(); i++)
	{
		if (shapes[i].get() == shape.get())
		{
			return i;
		}
	}
	return -1;
}

// Восстановление связей стрелок после загрузки
void Storage::restoreArrowConnections()
{
	// Проходим по всем фигурам
	for (auto& shape : shapes)
	{
		if (shape->getType() == "Arrow")
		{
			if (auto arrow = std::dynamic_pointer_cast<ArrowShape>(shape))
			{
				int sourceId = arrow->getSourceId();
				int targetId = arrow->getTargetId();

				// Восстанавливаем связь с источником
				if (sourceId >= 0 && sourceId < shapes.size())
				{
					arrow->setSource(shapes[sourceId]);
				}


				// Восстанавливаем связь с целью
				if (targetId >= 0 && targetId < shapes.size())
				{
					arrow->setTarget(shapes[targetId]);
				}
				if (arrow->getSource() && arrow->getTarget()) {
					// Просто вызываем метод стрелки для обновления позиции
					arrow->updateArrowPositionAndSize();
				}
			}
		}
	}
}


std::vector<std::shared_ptr<ArrowShape>> Storage::getArrows() const
{
	std::vector<std::shared_ptr<ArrowShape>> arrows;

	for (const auto& shape : shapes)
	{
		if (shape->getType() == "Arrow")
		{
			if (auto arrow = std::dynamic_pointer_cast<ArrowShape>(shape))
			{
				arrows.push_back(arrow);
			}
		}
	}

	return arrows;
}

// Очистка выделения всех фигур
void Storage::clearSelection()
{
	bool changed = false;
	for (auto& shape : shapes)
	{
		if (shape->isSelected())
		{
			shape->setSelected(false);
			changed = true;
		}
	}

	if (changed)
	{
		notifyChanges();
	}
}


// Выделение конкретной фигуры
void Storage::selectShape(std::shared_ptr<BaseShape> shape)
{
	if (!shape || shape->isSelected()) return;

	shape->setSelected(true);
	notifyChanges();
}


// Снятие выделения с конкретной фигуры
void Storage::deselectShape(std::shared_ptr<BaseShape> shape)
{
	if (!shape || !shape->isSelected()) return;

	shape->setSelected(false);
	notifyChanges();
}


