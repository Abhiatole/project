#include "MainWindow.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>

// Include the BooleanOperations header
#include "../include/BooleanOperations.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDrawMode(SELECT), currentOperation(UNION), isDrawing(false)
{
    setupUI();
    setupActions();
    setupConnections();
    setWindowTitle("Polygon Boolean Operations Visualizer");
    resize(800, 600);
    statusBar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create central widget and layout
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Create graphics scene and view
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setSceneRect(-400, -300, 800, 600);
    mainLayout->addWidget(view);

    // Create control panel
    QHBoxLayout* controlLayout = new QHBoxLayout();
    mainLayout->addLayout(controlLayout);

    // Add operation selection
    QLabel* operationLabel = new QLabel("Operation:", this);
    controlLayout->addWidget(operationLabel);

    operationComboBox = new QComboBox(this);
    operationComboBox->addItem("Union");
    operationComboBox->addItem("Intersection");
    operationComboBox->addItem("Difference");
    operationComboBox->addItem("Symmetric Difference");
    controlLayout->addWidget(operationComboBox);

    // Add buttons
    performButton = new QPushButton("Perform Operation", this);
    controlLayout->addWidget(performButton);

    clearButton = new QPushButton("Clear", this);
    controlLayout->addWidget(clearButton);

    addPolygonButton = new QPushButton("Add Polygon", this);
    controlLayout->addWidget(addPolygonButton);

    // Setup status bar
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    // Setup tool bar
    toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);

    // Setup menu bar
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* fileMenu = menuBar->addMenu("&File");
    QMenu* editMenu = menuBar->addMenu("&Edit");

    // Create actions
    saveAction = new QAction("&Save Result", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip("Save the result polygon");

    loadAction = new QAction("&Load Polygons", this);
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setStatusTip("Load polygons from a file");

    exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip("Exit the application");

    drawModeAction = new QAction("&Draw Mode", this);
    drawModeAction->setCheckable(true);
    drawModeAction->setStatusTip("Toggle draw mode");

    // Add actions to menus
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu->addAction(drawModeAction);
    
    // Add drawing mode actions to toolbar
    toolBar->addAction(drawModeAction);
}

void MainWindow::setupActions()
{
    saveAction = new QAction("&Save Result", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip("Save the result polygon");

    loadAction = new QAction("&Load Polygons", this);
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setStatusTip("Load polygons from a file");

    exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip("Exit the application");

    drawModeAction = new QAction("&Draw Mode", this);
    drawModeAction->setCheckable(true);
    drawModeAction->setStatusTip("Toggle draw mode");
}

void MainWindow::setupConnections()
{
    connect(performButton, &QPushButton::clicked, this, &MainWindow::performOperation);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearScene);
    connect(addPolygonButton, &QPushButton::clicked, this, &MainWindow::addPolygon);
    connect(operationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
        [this](int index) { currentOperation = static_cast<Operation>(index); });

    connect(saveAction, &QAction::triggered, this, &MainWindow::saveResult);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadPolygons);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(drawModeAction, &QAction::triggered, this, &MainWindow::toggleDrawMode);
}

void MainWindow::clearScene()
{
    scene->clear();
    polygonA = QPolygonF();
    polygonB = QPolygonF();
    resultPolygon = QPolygonF();
    currentPoints.clear();
    isDrawing = false;
    statusBar->showMessage("Scene cleared");
}

void MainWindow::addPolygon()
{
    currentDrawMode = DRAW_POLYGON_A;
    if (!polygonA.isEmpty()) {
        currentDrawMode = DRAW_POLYGON_B;
    }
    currentPoints.clear();
    isDrawing = true;
    drawModeAction->setChecked(true);
    statusBar->showMessage("Click to add points to the polygon. Right-click to finish.");
}

void MainWindow::toggleDrawMode()
{
    if (drawModeAction->isChecked()) {
        currentDrawMode = polygonA.isEmpty() ? DRAW_POLYGON_A : DRAW_POLYGON_B;
        statusBar->showMessage("Draw mode: Click to add points to the polygon. Right-click to finish.");
    } else {
        currentDrawMode = SELECT;
        statusBar->showMessage("Select mode");
    }
}

void MainWindow::drawPolygon(const QPolygonF& polygon, const QColor& color)
{
    if (polygon.isEmpty()) {
        return;
    }
    
    QPen pen(color, 2);
    QBrush brush(color);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(color.red(), color.green(), color.blue(), 50)); // Semi-transparent
    
    scene->addPolygon(polygon, pen, brush);
    
    // Draw points
    for (const QPointF& point : polygon) {
        scene->addEllipse(point.x() - 3, point.y() - 3, 6, 6, pen, QBrush(color));
    }
}

void MainWindow::performOperation()
{
    if (polygonA.isEmpty() || polygonB.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Need two polygons to perform an operation");
        return;
    }
    
    // Convert QPolygonF to CGAL polygon format
    std::vector<std::pair<double, double>> pointsA;
    std::vector<std::pair<double, double>> pointsB;
    
    for (const QPointF& point : polygonA) {
        pointsA.push_back(std::make_pair(point.x(), point.y()));
    }
    
    for (const QPointF& point : polygonB) {
        pointsB.push_back(std::make_pair(point.x(), point.y()));
    }
    
    // Call the BooleanOperations class to perform the selected operation
    std::vector<std::pair<double, double>> resultPoints;
    
    try {
        BooleanOperations operations;
        
        switch (currentOperation) {
            case UNION:
                resultPoints = operations.performUnion(pointsA, pointsB);
                break;
            case INTERSECTION:
                resultPoints = operations.performIntersection(pointsA, pointsB);
                break;
            case DIFFERENCE:
                resultPoints = operations.performDifference(pointsA, pointsB);
                break;
            case SYMMETRIC_DIFFERENCE:
                resultPoints = operations.performSymmetricDifference(pointsA, pointsB);
                break;
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Error performing operation: %1").arg(e.what()));
        return;
    }
    
    // Convert result back to QPolygonF
    resultPolygon = QPolygonF();
    for (const auto& point : resultPoints) {
        resultPolygon << QPointF(point.first, point.second);
    }
    
    // Display result
    scene->clear();
    drawPolygon(polygonA, Qt::blue);
    drawPolygon(polygonB, Qt::green);
    drawPolygon(resultPolygon, Qt::red);
    
    statusBar->showMessage("Operation performed successfully");
}

void MainWindow::saveResult()
{
    if (resultPolygon.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No result to save");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, "Save Result", "", "Polygon Files (*.poly);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing");
        return;
    }
    
    QTextStream out(&file);
    out << resultPolygon.size() << "\n";
    for (const QPointF& point : resultPolygon) {
        out << point.x() << " " << point.y() << "\n";
    }
    
    statusBar->showMessage("Result saved to " + fileName);
}

void MainWindow::loadPolygons()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Polygons", "", "Polygon Files (*.poly);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file for reading");
        return;
    }
    
    clearScene();
    
    QTextStream in(&file);
    int numPoints;
    in >> numPoints;
    
    for (int i = 0; i < numPoints; i++) {
        double x, y;
        in >> x >> y;
        polygonA << QPointF(x, y);
    }
    
    if (!in.atEnd()) {
        in >> numPoints;
        for (int i = 0; i < numPoints; i++) {
            double x, y;
            in >> x >> y;
            polygonB << QPointF(x, y);
        }
    }
    
    drawPolygon(polygonA, Qt::blue);
    drawPolygon(polygonB, Qt::green);
    
    statusBar->showMessage("Polygons loaded from " + fileName);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (currentDrawMode == SELECT || !isDrawing) {
        QMainWindow::mousePressEvent(event);
        return;
    }
    
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = view->mapToScene(event->pos());
        currentPoints.append(scenePos);
        
        // Draw point
        scene->addEllipse(scenePos.x() - 3, scenePos.y() - 3, 6, 6, 
            QPen(currentDrawMode == DRAW_POLYGON_A ? Qt::blue : Qt::green),
            QBrush(currentDrawMode == DRAW_POLYGON_A ? Qt::blue : Qt::green));
        
        // Draw line if there's more than one point
        if (currentPoints.size() > 1) {
            scene->addLine(
                currentPoints[currentPoints.size() - 2].x(),
                currentPoints[currentPoints.size() - 2].y(),
                scenePos.x(), scenePos.y(),
                QPen(currentDrawMode == DRAW_POLYGON_A ? Qt::blue : Qt::green, 2)
            );
        }
    } else if (event->button() == Qt::RightButton) {
        // Finish polygon
        if (currentPoints.size() >= 3) {
            QPolygonF polygon(currentPoints);
            
            if (currentDrawMode == DRAW_POLYGON_A) {
                polygonA = polygon;
                statusBar->showMessage("Polygon A created");
            } else {
                polygonB = polygon;
                statusBar->showMessage("Polygon B created");
            }
            
            // Close the polygon by drawing a line from last to first point
            scene->addLine(
                currentPoints.last().x(), currentPoints.last().y(),
                currentPoints.first().x(), currentPoints.first().y(),
                QPen(currentDrawMode == DRAW_POLYGON_A ? Qt::blue : Qt::green, 2)
            );
            
            // Reset for next polygon
            currentPoints.clear();
            isDrawing = false;
            currentDrawMode = SELECT;
            drawModeAction->setChecked(false);
        } else {
            QMessageBox::warning(this, "Warning", "Need at least 3 points to create a polygon");
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    QMainWindow::mouseMoveEvent(event);
    
    if (isDrawing && !currentPoints.isEmpty()) {
        QPointF scenePos = view->mapToScene(event->pos());
        statusBar->showMessage(QString("Position: (%1, %2)").arg(scenePos.x()).arg(scenePos.y()));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    QMainWindow::mouseReleaseEvent(event);
}