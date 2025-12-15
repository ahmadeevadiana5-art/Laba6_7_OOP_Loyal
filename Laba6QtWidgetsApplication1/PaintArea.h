#ifndef PAINTAREA_H
#define PAINTAREA_H

#include<QWidget>
#include "Storage6.h"

class PaintArea : public QWidget
{
	Q_OBJECT
public:
	PaintArea(Storage* storage1, QWidget* parent = nullptr);
	void createArrowBetweenSelected();
	void startCreatingArrow();

protected:

	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	/*
	void handleMovement(int dx, int dy);
	void handleResizing(int dw, int dh);
	*/
	//лр7 
	void drawArrows(QPainter& painter);
	void handleMovement(int dx, int dy);
	void handleResizing(int dw, int dh);

	/*
	//методы для работы со стрелками 
	void startCreatingArrow();
	void cancelCreatingArrow();
	*/

	Storage* storage;
	QRect workArea;
	bool ctrlPressed;


	
	//для создания стрелки 
	std::shared_ptr<BaseShape> m_selectedForArrow;
	bool m_isCreatingArrow;
	

signals:
	void groupRequested();
	void ungroupRequested();
	void selectionChanged();
	/*
	void arrowCreationStarted();
	void arrowCreationCancelled();
	*/
};
#endif