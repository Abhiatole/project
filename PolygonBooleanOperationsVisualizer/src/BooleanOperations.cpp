#include "../include/BooleanOperations.h"
#include <iostream>
#include <CGAL/IO/io.h>
#include <CGAL/Boolean_set_operations_2.h>

BooleanOperations::BooleanOperations() {
    // Empty constructor
}

BooleanOperations::~BooleanOperations() {
    // Empty destructor
}

// Create a square polygon
BooleanOperations::Polygon_2 BooleanOperations::createSquare(double x, double y, double size) {
    Polygon_2 square;
    square.push_back(Point_2(x, y));
    square.push_back(Point_2(x + size, y));
    square.push_back(Point_2(x + size, y + size));
    square.push_back(Point_2(x, y + size));
    return square;
}

// Create a triangle polygon
BooleanOperations::Polygon_2 BooleanOperations::createTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    Polygon_2 triangle;
    triangle.push_back(Point_2(x1, y1));
    triangle.push_back(Point_2(x2, y2));
    triangle.push_back(Point_2(x3, y3));
    return triangle;
}

// Perform union operation
BooleanOperations::Polygon_list BooleanOperations::performUnion(const Polygon_2& polygon1, const Polygon_2& polygon2) {
    Polygon_list result;
    
    // Create a vector of polygons for the join operation
    std::vector<Polygon_2> polygons;
    polygons.push_back(polygon1);
    polygons.push_back(polygon2);
    
    // Use the correct join function with an iterator range
    CGAL::join(polygons.begin(), polygons.end(), std::back_inserter(result));
    
    return result;
}

// Convert vector of points to CGAL Polygon
BooleanOperations::Polygon_2 BooleanOperations::convertToPolygon(const std::vector<std::pair<double, double>>& points) {
    Polygon_2 polygon;
    for (const auto& point : points) {
        polygon.push_back(Point_2(point.first, point.second));
    }
    return polygon;
}

// Convert CGAL Polygon to vector of points
std::vector<std::pair<double, double>> BooleanOperations::convertFromPolygon(const Polygon_with_holes_2& poly) {
    std::vector<std::pair<double, double>> result;
    
    // Only extract the outer boundary for now (ignoring holes)
    auto outer_boundary = poly.outer_boundary();
    for (auto vertex_it = outer_boundary.vertices_begin(); vertex_it != outer_boundary.vertices_end(); ++vertex_it) {
        result.push_back(std::make_pair(
            CGAL::to_double(vertex_it->x()),
            CGAL::to_double(vertex_it->y())
        ));
    }
    
    return result;
}

// Interface methods for Qt application

std::vector<std::pair<double, double>> BooleanOperations::performUnion(
    const std::vector<std::pair<double, double>>& polygonA,
    const std::vector<std::pair<double, double>>& polygonB) {
    
    Polygon_2 poly1 = convertToPolygon(polygonA);
    Polygon_2 poly2 = convertToPolygon(polygonB);
    
    Polygon_list result = performUnion(poly1, poly2);
    
    // If result is empty, return empty vector
    if (result.empty()) {
        return std::vector<std::pair<double, double>>();
    }
    
    // Return the first polygon's boundary (simplification)
    return convertFromPolygon(result.front());
}

std::vector<std::pair<double, double>> BooleanOperations::performIntersection(
    const std::vector<std::pair<double, double>>& polygonA,
    const std::vector<std::pair<double, double>>& polygonB) {
    
    Polygon_2 poly1 = convertToPolygon(polygonA);
    Polygon_2 poly2 = convertToPolygon(polygonB);
    
    Polygon_list result;
    CGAL::intersection(poly1, poly2, std::back_inserter(result));
    
    // If result is empty, return empty vector
    if (result.empty()) {
        return std::vector<std::pair<double, double>>();
    }
    
    // Return the first polygon's boundary
    return convertFromPolygon(result.front());
}

std::vector<std::pair<double, double>> BooleanOperations::performDifference(
    const std::vector<std::pair<double, double>>& polygonA,
    const std::vector<std::pair<double, double>>& polygonB) {
    
    Polygon_2 poly1 = convertToPolygon(polygonA);
    Polygon_2 poly2 = convertToPolygon(polygonB);
    
    Polygon_list result;
    CGAL::difference(poly1, poly2, std::back_inserter(result));
    
    // If result is empty, return empty vector
    if (result.empty()) {
        return std::vector<std::pair<double, double>>();
    }
    
    // Return the first polygon's boundary
    return convertFromPolygon(result.front());
}

std::vector<std::pair<double, double>> BooleanOperations::performSymmetricDifference(
    const std::vector<std::pair<double, double>>& polygonA,
    const std::vector<std::pair<double, double>>& polygonB) {
    
    Polygon_2 poly1 = convertToPolygon(polygonA);
    Polygon_2 poly2 = convertToPolygon(polygonB);
    
    Polygon_list result;
    CGAL::symmetric_difference(poly1, poly2, std::back_inserter(result));
    
    // If result is empty, return empty vector
    if (result.empty()) {
        return std::vector<std::pair<double, double>>();
    }
    
    // Return the first polygon's boundary
    return convertFromPolygon(result.front());
}

// Print a polygon with holes
void BooleanOperations::printPolygonWithHoles(const Polygon_with_holes_2& poly) {
    std::cout << "Outer boundary:" << std::endl;
    
    auto outer_boundary = poly.outer_boundary();
    for (auto vertex_it = outer_boundary.vertices_begin(); vertex_it != outer_boundary.vertices_end(); ++vertex_it) {
        std::cout << "  (" << CGAL::to_double(vertex_it->x()) << ", " 
                  << CGAL::to_double(vertex_it->y()) << ")" << std::endl;
    }
    
    unsigned int hole_count = 0;
    for (auto hole_it = poly.holes_begin(); hole_it != poly.holes_end(); ++hole_it) {
        std::cout << "Hole " << ++hole_count << ":" << std::endl;
        for (auto vertex_it = hole_it->vertices_begin(); vertex_it != hole_it->vertices_end(); ++vertex_it) {
            std::cout << "  (" << CGAL::to_double(vertex_it->x()) << ", " 
                      << CGAL::to_double(vertex_it->y()) << ")" << std::endl;
        }
    }
}

// Print a list of polygons with holes
void BooleanOperations::printPolygonList(const Polygon_list& polyList) {
    if (polyList.empty()) {
        std::cout << "The result polygon list is empty." << std::endl;
        return;
    }
    
    std::cout << "Result contains " << polyList.size() << " polygon(s) with holes:" << std::endl;
    int count = 0;
    for (const auto& poly : polyList) {
        std::cout << "Polygon " << ++count << ":" << std::endl;
        printPolygonWithHoles(poly);
        std::cout << std::endl;
    }
}