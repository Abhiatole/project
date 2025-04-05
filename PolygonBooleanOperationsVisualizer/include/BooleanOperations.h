#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <vector>
#include <list>
#include <utility>
#include <string>
#include <exception>

class BooleanOperations {
public:
    BooleanOperations();
    ~BooleanOperations();

    // CGAL typedefs
    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
    typedef Kernel::Point_2 Point_2;
    typedef CGAL::Polygon_2<Kernel> Polygon_2;
    typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;
    typedef std::list<Polygon_with_holes_2> Polygon_list;

    // Create geometric shapes
    Polygon_2 createSquare(double x, double y, double size);
    Polygon_2 createTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
    
    // Boolean operations
    Polygon_list performUnion(const Polygon_2& polygon1, const Polygon_2& polygon2);
    
    // Output functions
    void printPolygonWithHoles(const Polygon_with_holes_2& poly);
    void printPolygonList(const Polygon_list& polyList);
    
    // Perform union operation between two polygons
    std::vector<std::pair<double, double>> performUnion(
        const std::vector<std::pair<double, double>>& polygonA,
        const std::vector<std::pair<double, double>>& polygonB);

    // Perform intersection operation between two polygons
    std::vector<std::pair<double, double>> performIntersection(
        const std::vector<std::pair<double, double>>& polygonA,
        const std::vector<std::pair<double, double>>& polygonB);

    // Perform difference operation between two polygons
    std::vector<std::pair<double, double>> performDifference(
        const std::vector<std::pair<double, double>>& polygonA,
        const std::vector<std::pair<double, double>>& polygonB);

    // Perform symmetric difference operation between two polygons
    std::vector<std::pair<double, double>> performSymmetricDifference(
        const std::vector<std::pair<double, double>>& polygonA,
        const std::vector<std::pair<double, double>>& polygonB);

private:
    // Helper methods to convert between CGAL and standard representations
    Polygon_2 convertToPolygon(const std::vector<std::pair<double, double>>& points);
    std::vector<std::pair<double, double>> convertFromPolygon(const Polygon_with_holes_2& polygon);
};