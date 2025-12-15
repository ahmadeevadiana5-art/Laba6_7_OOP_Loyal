#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QTreeWidget>
#include "Storage6.h"
#include<QStatusBar>


class PaintArea;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // Конструктор главного окна
    MainWindow(QWidget* parent = nullptr);

    // Деструктор
    ~MainWindow();
    //QTreeWidget* m_treeWidget;  // ЛР7 дерево объектов

private slots:
    void onCreateShapeClicked(int shapeType);
    // Удаление выделенных фигур
    void onDeleteSelected();

    // Изменение цвета выделенных фигур
    void onChangeColor();


    //=====================ЛР 6 
    void onGroupClicked(); //группировка
    void onUngroupClicked(); //разгруппировка
    void onSaveClicked(); //сохранение в файл 
    void onLoadClicked(); //загрузка из файла
    void onClearAllClicked(); //очистить все

    //===============ЛР7
    void onTreeItemClicked(QTreeWidgetItem* item, int column);
    void updateTreeView();  // Обновить дерево

private:
    // Создание и настройка панели инструментов
    void setupToolBar();

    // Создание и настройка меню
    void setupMenu();

    //=======ЛР7 Метод для добавления фигуры в дерево
    void addShapeToTree(std::shared_ptr<BaseShape> shape, QTreeWidgetItem* parent);


    //метод для создания стрелки 
    void createArrow(std::shared_ptr<BaseShape> source,
        std::shared_ptr<BaseShape> target);
/*
    // Метод для обновления дерева объектов
    void rebuildObjectTree();
    */
    // Указатель на область рисования (View)
    PaintArea* m_paintArea;

    // Хранилище фигур (Model)
    Storage m_storage;

    // Группа кнопок для инструментов (чтобы они были взаимоисключающими)
    QButtonGroup* m_toolButtonGroup;

    //ЛР7 для отображения иерархии объектов
    QTreeWidget* m_treeWidget;

    bool m_isCreatingArrow;
    std::shared_ptr<BaseShape> m_arrowSource;

    // Перечисление для идентификации типов фигур
    enum ShapeType {
        ST_Circle,      // Круг
        ST_Rectangle,   // Прямоугольник  
        ST_Square,      // Квадрат
        ST_Triangle,    // Треугольник
        ST_Ellipse,     // Эллипс
        ST_Line         // Линия


    };
};

#endif