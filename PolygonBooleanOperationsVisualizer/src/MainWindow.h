#pragma once

#include <qMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVector>
#include <QPolygonF>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMouseEvent>  // Add this for mouse event handling
#include "MainWindow.h"
#include "../include/BooleanOperations.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void clearScene();
    void addPolygon();
    void performOperation();
    void toggleDrawMode();
    void saveResult();
    void loadPolygons();

private:
    enum DrawMode {
        SELECT,
        DRAW_POLYGON_A,
        DRAW_POLYGON_B
    };

    enum Operation {
        UNION,
        INTERSECTION,
        DIFFERENCE,
        SYMMETRIC_DIFFERENCE
    };

    void setupUI();
    void setupActions();
    void setupConnections();
    void drawPolygon(const QPolygonF& polygon, const QColor& color);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    QGraphicsScene* scene;
    QGraphicsView* view;
    QToolBar* toolBar;
    QStatusBar* statusBar;
    QComboBox* operationComboBox;
    QPushButton* performButton;
    QPushButton* clearButton;
    QPushButton* addPolygonButton;
    QAction* saveAction;
    QAction* loadAction;
    QAction* exitAction;
    QAction* drawModeAction;

    DrawMode currentDrawMode;
    Operation currentOperation;
    QPolygonF polygonA;
    QPolygonF polygonB;
    QPolygonF resultPolygon;
    QVector<QPointF> currentPoints;
    bool isDrawing;
};