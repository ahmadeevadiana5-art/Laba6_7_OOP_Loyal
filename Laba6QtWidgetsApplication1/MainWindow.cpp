#include "MainWindow.h"
#include "Circle.h"              // Класс круга
#include "PaintArea.h"
#include "group.h"

#include "Square6.h"
#include "Rectangle6.h"
#include "Triangle7.h"
#include "Ellipse6.h"
#include "Line6.h"


#include<QMainWindow>
#include <QToolBar>              // Панель инструментов
#include <QToolButton>           // Кнопка на панели инструментов
#include <QMenu>                 // Меню
#include <QMenuBar>              // Строка меню
#include <QColorDialog>          // Диалог выбора цвета
#include <QMessageBox>           // Диалоговые окна сообщений
#include <QAction>               // Действие меню
#include <QDebug>  
#include<QPainter>
#include <QDir>
#include<QFileDialog>
#include<QTreeWidget>
#include<QHBoxLayout>
#include<QApplication>
#include<QStatusBar>






MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),        // Вызов конструктора родительского класса
    m_storage(100),             // Инициализация хранилища на 100 фигур
    m_toolButtonGroup(nullptr), m_treeWidget(nullptr), // Инициализация указателя нулем
    m_isCreatingArrow(false), m_paintArea(nullptr),
    m_arrowSource(nullptr)
{

    // Установка заголовка окна
    setWindowTitle("Визуальный редактор - Лабораторная работа 6,7");

    // Установка положения и размера окна:
    setGeometry(100, 100, 1000, 700);


    //=============ЛР7 
    // Создаем главный layout
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Левая панель: TreeView
    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderLabel("Объекты");
    m_treeWidget->setMinimumWidth(200);

    connect(m_treeWidget, &QTreeWidget::itemClicked,
        this, &MainWindow::onTreeItemClicked);

    // Правая панель: PaintArea
    m_paintArea = new PaintArea(&m_storage, this);

    // Добавляем в layout
    mainLayout->addWidget(m_treeWidget, 1);      // Левая часть (дерево)
    mainLayout->addWidget(m_paintArea, 3);       // Правая часть (рабочая область)
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


   // m_treeObserver = new TreeViewObserver(m_treeWidget, &m_storage, this);

    //нАСТРОЙКА ИНТЕРФЕЙСА 
    

    // Создание области рисования
    // Передаем указатель на хранилище и this как родителя
    //m_paintArea = new PaintArea(&m_storage, this);

    // Установка области рисования как центрального виджета окна
    //setCentralWidget(m_paintArea);

    // Настройка пользовательского интерфейса
    setupToolBar();  // Создаем панель инструментов
    setupMenu();     // Создаем меню


    //===========ЛР6 Добавляем подключение сигналов

    connect(m_paintArea, &PaintArea::groupRequested, this, &MainWindow::onGroupClicked);
    connect(m_paintArea, &PaintArea::ungroupRequested, this, &MainWindow::onUngroupClicked);
    connect(m_paintArea, &PaintArea::selectionChanged,
        this, &MainWindow::updateTreeView);

    /*
    // НОВОЕ: Подключаем сигналы для создания стрелок
    connect(m_paintArea, &PaintArea::arrowCreationStarted,
        this, [this]() {
            m_isCreatingArrow = true;
            m_arrowSource = nullptr;
            statusBar()->showMessage("Выберите первый объект (источник) для стрелки");
        });

    connect(m_paintArea, &PaintArea::shapeSelectedForArrow,
        this, [this](std::shared_ptr<BaseShape> shape) {
            if (m_isCreatingArrow) {
                if (!m_arrowSource) {
                    m_arrowSource = shape;
                    statusBar()->showMessage("Выберите второй объект (цель) для стрелки");
                }
                else if (shape != m_arrowSource) {
                    // Создаем стрелку
                    createArrow(m_arrowSource, shape);
                    m_isCreatingArrow = false;
                    m_arrowSource = nullptr;
                    statusBar()->showMessage("Стрелка создана");
                }
            }
        });
    connect(m_paintArea, &PaintArea::arrowCreationCancelled,
        this, [this]() {
            m_isCreatingArrow = false;
            m_arrowSource = nullptr;
            statusBar()->showMessage("Создание стрелки отменено");
        });
        */
}

//Деструктор 
MainWindow::~MainWindow()
{

}

//панель инструментов 
void MainWindow::setupToolBar()
{
    QToolBar* toolBar = addToolBar("Инструменты"); //создает и возвращает указатель на панель
    toolBar->setMovable(true); //перетаскивание панели
    //группа кнопок
    m_toolButtonGroup = new QButtonGroup(this); //создание группы кнопок
    m_toolButtonGroup->setExclusive(true);

    // Кнопка для создания стрелки
    QToolButton* arrowBtn = new QToolButton();
    arrowBtn->setText("Стрелка");
    arrowBtn->setToolTip("Создать стрелку между выделенными фигурами");
    connect(arrowBtn, &QToolButton::clicked, this, [this, arrowBtn]()
        {
            // Простой способ: создать стрелку между двумя выделенными фигурами
            auto selected = m_storage.getSelectedShapes();
            if (selected.size() == 2) {
                createArrow(selected[0], selected[1]);
            }
            else {
                QMessageBox::information(this, "Информация",
                    "Для создания стрелки выделите 2 фигуры");
            }
        });
        //m_paintArea, &PaintArea::startCreatingArrow);
    toolBar->addWidget(arrowBtn);


    //список кнопок для создания фишур 
    const QList<QPair<QString, int>> shapeButtons =
    {
        {"Круг", ST_Circle},
        {"Прямоугольник", ST_Rectangle},
        {"Квадрат", ST_Square},
        {"Треугольник", ST_Triangle},
        {"Эллипс", ST_Ellipse},
        {"Линия", ST_Line}
    };


    //создание кнопок в цикле
    for (const auto& button : shapeButtons)
    {
        QToolButton* btn = new QToolButton(); //созданеи кнопки
        btn->setText(button.first);

        // Делаем кнопку  нажата/не нажата
        btn->setCheckable(true);

        // Устанавливаем подсказку при наведении
        btn->setToolTip("Создать " + button.first);

        // Добавляем кнопку в группу с соответствующим идентификатором
        m_toolButtonGroup->addButton(btn, button.second);

        // Добавляем кнопку на панель инструментов
        toolBar->addWidget(btn);
    }

    toolBar->addSeparator();

    //кнопка удалить
    QToolButton* deleteBtn = new QToolButton();
    deleteBtn->setText("Удалить");
    deleteBtn->setToolTip("Удалить выделенные фигуры.");

    connect(deleteBtn, &QToolButton::clicked,
        this, &MainWindow::onDeleteSelected);

    toolBar->addWidget(deleteBtn);


    //Группировать кнопка
    QToolButton* groupBtn = new QToolButton();
    groupBtn->setText("Группировать");
    groupBtn->setToolTip("Объединить выделенные фигуры в группу");
    connect(groupBtn, &QToolButton::clicked, this, &MainWindow::onGroupClicked);
    toolBar->addWidget(groupBtn);



    // Создаем кнопку для изменения цвета
    QToolButton* colorBtn = new QToolButton();
    colorBtn->setText("Цвет");
    colorBtn->setToolTip("Изменить цвет выделенных фигур");

    // Соединяем сигнал нажатия кнопки со слотом изменения цвета
    connect(colorBtn, &QToolButton::clicked,
        this, &MainWindow::onChangeColor);

    // Добавляем кнопку на панель
    toolBar->addWidget(colorBtn);


    connect(m_toolButtonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
        [this](QAbstractButton* button) {
            int id = m_toolButtonGroup->id(button);
            onCreateShapeClicked(id);
        });



}
void MainWindow::setupMenu()
{
    // === МЕНЮ "ФАЙЛ" ===
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    // Действие "Сохранить"
    QAction* saveAction = fileMenu->addAction("Сохранить проект...");
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setToolTip("Сохранить все фигуры в файл");
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveClicked);

    // Действие "Загрузить"
    QAction* loadAction = fileMenu->addAction("Загрузить проект...");
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setToolTip("Загрузить фигуры из файла");
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadClicked);

    fileMenu->addSeparator();

    // Действие "Очистить все"
    QAction* clearAction = fileMenu->addAction("Очистить все");
    clearAction->setToolTip("Удалить все фигуры");

    connect(clearAction, &QAction::triggered, [this]() {
        m_storage.clear();
        m_paintArea->update();
        });
    fileMenu->addSeparator();

    // Действие "Выход"
    QAction* exitAction = fileMenu->addAction("Выход");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // === МЕНЮ "ПРАВКА" ===
    QMenu* editMenu = menuBar()->addMenu("Правка");

    // Действие "Группировать"
    QAction* groupAction = editMenu->addAction("Группировать");
    groupAction->setShortcut(QKeySequence("Ctrl+G"));
    groupAction->setToolTip("Объединить выделенные фигуры в группу");

    connect(groupAction, &QAction::triggered, this, &MainWindow::onGroupClicked);
    
    // Действие "Разгруппировать"
    QAction* ungroupAction = editMenu->addAction("Разгруппировать");
    ungroupAction->setShortcut(QKeySequence("Ctrl+Shift+G"));
    ungroupAction->setToolTip("Разбить выделенную группу на отдельные фигуры");
    connect(ungroupAction, &QAction::triggered, this, &MainWindow::onUngroupClicked);

    editMenu->addSeparator();

    // Действие "Обновить дерево" (для отладки)
    QAction* refreshTreeAction = editMenu->addAction("Обновить дерево");
    refreshTreeAction->setShortcut(QKeySequence("F5"));
    connect(refreshTreeAction, &QAction::triggered,
        this, &MainWindow::updateTreeView);

    // Действие "Удалить выделенные"
    QAction* deleteAction = editMenu->addAction("Удалить выделенные");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteSelected);
}

// Слот для создания новой фигуры
void MainWindow::onCreateShapeClicked(int shapeType)
{
    std::shared_ptr<BaseShape> newShape;
    QPoint center = m_paintArea->rect().center();

    switch (shapeType) {
    case ST_Circle:
        newShape = std::make_shared<Circle>(center, 30);
        break;
    
    case ST_Square:
        newShape = std::make_shared<Square>(center, 40);
        break;
    case ST_Rectangle:
        newShape = std::make_shared<Rectangle>(center, 60, 40);
        break;
    case ST_Ellipse:
        newShape = std::make_shared<Ellipse>(center, 70, 40);
        break;
    case ST_Triangle:
        newShape = std::make_shared<Triangle>(center, 50);
        break;
    case ST_Line:
        newShape = std::make_shared<Line>(center, QPoint(center.x() + 50, center.y() + 50));
        break;
        
    default:
        newShape = std::make_shared<Circle>(center, 30);
        break;
    }
    if (newShape)
    {
        m_storage.add(newShape);
        m_paintArea->update();
        updateTreeView();

    }
    // Снимаем выделение с кнопки
    if (m_toolButtonGroup->checkedButton()) {
        m_toolButtonGroup->checkedButton()->setChecked(false);
    }
}

// Слот для удаления выделенных фигур
void MainWindow::onDeleteSelected()
{
    auto selectedShapes = m_storage.getSelectedShapes();
    if (selectedShapes.empty()) {
        QMessageBox::information(this, "Информация", "Нет выделенных фигур для удаления.");
        return;
    }

    m_storage.removeSelected();
    m_paintArea->update();
    updateTreeView();
}

// Слот для изменения цвета выделенных фигур
void MainWindow::onChangeColor()
{
    auto selectedShapes = m_storage.getSelectedShapes();
    if (selectedShapes.empty()) {
        QMessageBox::information(this, "Информация", "Сначала выделите фигуры для изменения цвета.");
        return;
    }

    QColor newColor = QColorDialog::getColor(Qt::white, this, "Выберите цвет заливки");
    if (newColor.isValid()) {
        m_storage.changeColorSelected(newColor);  // ← Убедитесь что в Storage метод называется changeColorSelected
        m_paintArea->update();
        updateTreeView();
    }
}

//================ ЛР 6 ============
//реализация группировки черех паттерн Composite
void MainWindow::onGroupClicked()
{
    //количество выделенных фигур 
    auto selectedShapes = m_storage.getSelectedShapes();

    if (selectedShapes.size() < 2)
    {
        QMessageBox::information(this, "Информация",
            "Для группировки необходимо выделить хотя бы 2 фигуры.");
        return;
    }
    //группировка
    auto group = m_storage.groupSelected();
    if (group) 
    {
        // Устанавливаем группе те же свойства, что и у первой выделенной фигуры
        if (!selectedShapes.empty()) 
        {
            auto firstShape = selectedShapes[0];
            group->setColorborder(Qt::red);  // Группа всегда с красной рамкой
        }

        //обновляем отображение 
        m_paintArea->update();
        updateTreeView();

        // Показываем информационное сообщение
        QMessageBox::information(this, "Группировка",
            QString("Создана группа из %1 фигур.").arg(selectedShapes.size()));
    }
}

// Разгруппировка выделенной группы
void MainWindow::onUngroupClicked()
{
    // Проверяем, есть ли выделенная группа
    bool hasSelectedGroup = false;
    auto shapes = m_storage.getShapes();

    for (const auto& shape : shapes) {
        if (shape->isSelected() && shape->isGroup()) {
            hasSelectedGroup = true;
            break;
        }
    }

    if (!hasSelectedGroup) {
        QMessageBox::information(this, "Информация",
            "Выделите группу для разгруппировки.");
        return;
    }

    // Выполняем разгруппировку
    m_storage.ungroupSelected();

    // Обновляем отображение
    m_paintArea->update();
    updateTreeView();

    QMessageBox::information(this, "Разгруппировка",
        "Группа разгруппирована.");
}

// Сохранение проекта в файл
//factory method паттерн
void MainWindow::onSaveClicked()
{
    // Проверяем, есть ли что сохранять
    if (m_storage.getCount() == 0) {
        QMessageBox::information(this, "Информация",
            "Нет фигур для сохранения.");
        return;
    }

    // Открываем диалог выбора файла
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Сохранить проект",
        QDir::homePath() + "/untitled.shape",
        "Shape Files (*.shape);;All Files (*.*)"
    );

    if (filename.isEmpty()) {
        return;  // Пользователь отменил
    }

    // Добавляем расширение, если его нет
    if (!filename.endsWith(".shape", Qt::CaseInsensitive)) {
        filename += ".shape";
    }

    // Сохраняем в файл
    if (m_storage.saveToFile(filename)) {
        QMessageBox::information(this, "Успех",
            QString("Проект сохранён в файл:\n%1").arg(filename));
    }
    else {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось сохранить проект.");
    }
}

// Загрузка проекта из файла
void MainWindow::onLoadClicked()
{
    // Спрашиваем подтверждение, если есть текущие фигуры
    if (m_storage.getCount() > 0) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Подтверждение",
            "Текущие фигуры будут удалены. Продолжить?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    // Открываем диалог выбора файла
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Загрузить проект",
        QDir::homePath(),
        "Shape Files (*.shape);;All Files (*.*)"
    );

    if (filename.isEmpty()) {
        return;  // Пользователь отменил
    }

    // Загружаем из файла
    if (m_storage.loadFromFile(filename)) {
        // Обновляем отображение
        
        m_paintArea->update();
        m_paintArea->repaint();
        updateTreeView();

        

        QMessageBox::information(this, "Успех",
            QString("Проект загружен из файла:\n%1").arg(filename));
    }
    else {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось загрузить проект. Возможно, файл повреждён.");
    }
}

// Очистка всех фигур
void MainWindow::onClearAllClicked()
{
    if (m_storage.getCount() == 0) {
        QMessageBox::information(this, "Информация",
            "Нет фигур для очистки.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Подтверждение",
        QString("Удалить все фигуры (%1 шт.)?").arg(m_storage.getCount()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_storage.clear();
        m_paintArea->update();
    }
}

//ЛР7 
//добавление в дерево

void MainWindow::addShapeToTree(std::shared_ptr<BaseShape> shape, QTreeWidgetItem* parent)
{
    if (!shape) return; //проверка на группу

    // Создаем элемент для фигуры
    QTreeWidgetItem* item = new QTreeWidgetItem(parent);

    QString itemText;
    if (shape->getType() == "Arrow")
    {
        item->setText(0, "Стрелка");

    }
    else
    {

        // Формируем текст: "Тип (цвет)"
        QString itemText = QString("%1 (#%2)")
            .arg(shape->getType())
            .arg(shape->getfillColor().rgb() & 0xFFFFFF, 6, 16, QChar('0'));

        //item->setText(0, shape->getType());
        item->setText(0, itemText);
    }

    // Сохраняем указатель на фигуру в данных элемента
    QVariant variant = QVariant::fromValue(static_cast<void*>(shape.get()));
    item->setData(0, Qt::UserRole, variant);

    // Если фигура выделена - отмечаем в дереве
    if (shape->isSelected()) {
        item->setBackground(0, QBrush(Qt::yellow));
        item->setSelected(true);
    }

    // Если это группа - добавляем дочерние фигуры
    if (shape->isGroup()) {
        auto group = std::dynamic_pointer_cast<Group>(shape);
        if (group) {
            auto childShapes = group->getShapes();
            for (const auto& child : childShapes) {
                addShapeToTree(child, item);
            }
        }
    }
}

//ЛР7 Observer для TreeView обновление
void MainWindow::updateTreeView()
{
    if (!m_treeWidget) return;

    // Очищаем дерево
    m_treeWidget->clear();

    // Получаем все фигуры из хранилища
    auto shapes = m_storage.getShapes();

    // Создаем корневой элемент
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_treeWidget);
    rootItem->setText(0, "Все объекты");
    rootItem->setExpanded(true);

    // Рекурсивно добавляем фигуры
    for (const auto& shape : shapes) {
        addShapeToTree(shape, rootItem);
    }
}

//обработка клика
void MainWindow::onTreeItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    if (!item) return;

    // Получаем фигуру из данных элемента
    QVariant variant = item->data(0, Qt::UserRole);
    if (!variant.isValid()) return;

    BaseShape* shape = static_cast<BaseShape*>(variant.value<void*>());
    if (!shape) return;

    // Выделяем фигуру в рабочей области
    // Сначала снимаем выделение со всех
    auto shapes = m_storage.getShapes();
    for (const auto& s : shapes) {
        s->setSelected(false);
    }

    // Выделяем выбранную фигуру, при клике выделяется фигура в рабочей области
    shape->setSelected(true);

    // Обновляем рабочую область
    m_paintArea->update();
}

void MainWindow::createArrow(std::shared_ptr<BaseShape> source, std::shared_ptr<BaseShape> target)
{
    if (!source || !target || source == target) {
        QMessageBox::warning(this, "Ошибка", "Невозможно создать стрелку");
        return;
    }
    auto arrow = std::make_shared<ArrowShape>(source, target);

    auto shapes = m_storage.getShapes();
    int sourceIndex = -1, targetIndex = -1;

    for (int i = 0; i < shapes.size(); i++) {
        if (shapes[i].get() == source.get()) sourceIndex = i;
        if (shapes[i].get() == target.get()) targetIndex = i;
    }

    arrow->setSourceId(sourceIndex);
    arrow->setTargetId(targetIndex);

    // 3. Добавляем в Storage как обычную фигуру
    m_storage.add(arrow);

    // 4. Обновляем интерфейс
    m_paintArea->update();
    updateTreeView();

    QMessageBox::information(this, "Стрелка создана",
        "Стрелка будет сохранена в файл и появится в дереве объектов");
    
}
