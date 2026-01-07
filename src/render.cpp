#include "render.h"
#include <iostream>
#include <algorithm>

Render::Render(RouteModel &model) : m_Model(model) {
    m_Scale = static_cast<float>(std::min(m_WindowWidth, m_WindowHeight));
    m_PixelsInMeter = m_Scale / m_Model.MetricScale();
}

void Render::Display() {
    sf::RenderWindow window(sf::VideoMode({m_WindowWidth, m_WindowHeight}), "OpenStreetMap A* Route Planner");

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(238, 235, 227));

        // Draw map elements in proper layering order
        DrawLanduses(window);
        DrawLeisure(window);
        DrawWater(window);
        DrawRailways(window);
        DrawRoads(window);
        DrawBuildings(window);
        DrawPath(window);
        DrawStartPosition(window);
        DrawEndPosition(window);

        window.display();
    }
}

sf::Vector2f Render::ToScreen(float x, float y) {
    float screenX = x * m_Scale;
    float screenY = (1.0f - y) * m_Scale;
    return sf::Vector2f(screenX, screenY);
}

sf::Color Render::GetRoadColor(Model::Road::Type type) {
    switch(type) {
        case Model::Road::Motorway:     return sf::Color(226, 122, 143);
        case Model::Road::Trunk:        return sf::Color(245, 161, 136);
        case Model::Road::Primary:      return sf::Color(249, 207, 144);
        case Model::Road::Secondary:    return sf::Color(244, 251, 173);
        case Model::Road::Tertiary:     return sf::Color(244, 251, 173);
        case Model::Road::Residential:  return sf::Color(254, 254, 254);
        case Model::Road::Service:      return sf::Color(254, 254, 254);
        case Model::Road::Unclassified: return sf::Color(254, 254, 254);
        default:                        return sf::Color(200, 200, 200);
    }
}

float Render::GetRoadWidth(Model::Road::Type type) {
    switch(type) {
        case Model::Road::Motorway:     return 6.0f;
        case Model::Road::Trunk:        return 6.0f;
        case Model::Road::Primary:      return 5.0f;
        case Model::Road::Secondary:    return 4.0f;
        case Model::Road::Tertiary:     return 3.0f;
        case Model::Road::Residential:  return 2.5f;
        case Model::Road::Service:      return 1.5f;
        case Model::Road::Unclassified: return 2.0f;
        default:                        return 1.0f;
    }
}

void Render::DrawRoads(sf::RenderWindow &window) {
    const auto &ways = m_Model.Ways().data();
    const auto &nodes = m_Model.Nodes().data();

    for (const auto &road : m_Model.Roads()) {
        if (road.type == Model::Road::Footway) continue;

        const auto &way = ways[road.way];
        if (way.nodes.empty()) continue;

        sf::Color color = GetRoadColor(road.type);
        float metricWidth = GetRoadWidth(road.type);
        float width = metricWidth > 0.0f ? (metricWidth * m_PixelsInMeter) : 1.0f;

        // Draw thick roads as rectangles between nodes
        for (size_t i = 0; i < way.nodes.size() - 1; i++) {
            const auto &node1 = nodes[way.nodes[i]];
            const auto &node2 = nodes[way.nodes[i + 1]];

            sf::Vector2f p1 = ToScreen(node1.x, node1.y);
            sf::Vector2f p2 = ToScreen(node2.x, node2.y);

            sf::Vector2f direction = p2 - p1;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float angleRadians = std::atan2(direction.y, direction.x);

            sf::RectangleShape roadSegment(sf::Vector2f(length, width));
            roadSegment.setPosition(p1);
            roadSegment.setRotation(sf::radians(angleRadians));
            roadSegment.setFillColor(color);

            window.draw(roadSegment);
        }
    }
}

void Render::DrawPath(sf::RenderWindow &window) {
    if (m_Model.path.empty()) return;

    sf::VertexArray line(sf::PrimitiveType::LineStrip);

    for (const auto &node : m_Model.path) {
        sf::Vector2f pos = ToScreen(node.x, node.y);
        sf::Vertex vertex;
        vertex.position = pos;
        vertex.color = sf::Color(255, 165, 0);
        line.append(vertex);
    }

    for (size_t i = 0; i < line.getVertexCount() - 1; i++) {
        sf::Vertex p1 = line[i];
        sf::Vertex p2 = line[i + 1];

        sf::Vector2f direction = p2.position - p1.position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float angleRadians = std::atan2(direction.y, direction.x);

        sf::RectangleShape thickLine(sf::Vector2f(length, 5.0f));
        thickLine.setPosition(p1.position);
        thickLine.setRotation(sf::radians(angleRadians));
        thickLine.setFillColor(sf::Color(255, 165, 0));

        window.draw(thickLine);
    }
}

void Render::DrawStartPosition(sf::RenderWindow &window) {
    if (m_Model.path.empty()) return;

    sf::Vector2f pos = ToScreen(m_Model.path.front().x, m_Model.path.front().y);

    sf::CircleShape marker(10.0f);
    marker.setPosition(sf::Vector2f(pos.x - 10.0f, pos.y - 10.0f));
    marker.setFillColor(sf::Color::Green);
    marker.setOutlineColor(sf::Color::Black);
    marker.setOutlineThickness(2.0f);

    window.draw(marker);
}

void Render::DrawEndPosition(sf::RenderWindow &window) {
    if (m_Model.path.empty()) return;

    sf::Vector2f pos = ToScreen(m_Model.path.back().x, m_Model.path.back().y);

    sf::CircleShape marker(10.0f);
    marker.setPosition(sf::Vector2f(pos.x - 10.0f, pos.y - 10.0f));
    marker.setFillColor(sf::Color::Red);
    marker.setOutlineColor(sf::Color::Black);
    marker.setOutlineThickness(2.0f);

    window.draw(marker);
}

sf::Color Render::GetLanduseColor(Model::Landuse::Type type) {
    switch(type) {
        case Model::Landuse::Commercial:    return sf::Color(233, 195, 196);
        case Model::Landuse::Construction:  return sf::Color(187, 188, 165);
        case Model::Landuse::Grass:         return sf::Color(197, 236, 148);
        case Model::Landuse::Forest:        return sf::Color(158, 201, 141);
        case Model::Landuse::Industrial:    return sf::Color(223, 197, 220);
        case Model::Landuse::Railway:       return sf::Color(223, 197, 220);
        case Model::Landuse::Residential:   return sf::Color(209, 209, 209);
        default:                            return sf::Color(200, 200, 200);
    }
}

void Render::DrawMultipolygon(sf::RenderWindow &window, const Model::Multipolygon &mp, sf::Color fillColor, sf::Color outlineColor, float outlineThickness) {
    const auto &nodes = m_Model.Nodes().data();
    const auto &ways = m_Model.Ways().data();

    auto drawWayPolygon = [&](const Model::Way &way) {
        if (way.nodes.empty()) return;

        sf::ConvexShape polygon;
        polygon.setPointCount(way.nodes.size());

        for (size_t i = 0; i < way.nodes.size(); ++i) {
            const auto &node = nodes[way.nodes[i]];
            polygon.setPoint(i, ToScreen(node.x, node.y));
        }

        polygon.setFillColor(fillColor);
        if (outlineThickness > 0.0f) {
            polygon.setOutlineColor(outlineColor);
            polygon.setOutlineThickness(outlineThickness);
        }

        window.draw(polygon);
    };

    for (int way_num : mp.outer) {
        drawWayPolygon(ways[way_num]);
    }

    for (int way_num : mp.inner) {
        drawWayPolygon(ways[way_num]);
    }
}

void Render::DrawWay(sf::RenderWindow &window, const Model::Way &way, sf::Color color, float width) {
    if (way.nodes.empty()) return;

    const auto &nodes = m_Model.Nodes().data();
    sf::VertexArray line(sf::PrimitiveType::LineStrip);

    for (int node_idx : way.nodes) {
        const auto &node = nodes[node_idx];
        sf::Vertex vertex;
        vertex.position = ToScreen(node.x, node.y);
        vertex.color = color;
        line.append(vertex);
    }

    window.draw(line);
}

void Render::DrawLanduses(sf::RenderWindow &window) {
    for (const auto &landuse : m_Model.Landuses()) {
        sf::Color color = GetLanduseColor(landuse.type);
        DrawMultipolygon(window, landuse, color);
    }
}

void Render::DrawLeisure(sf::RenderWindow &window) {
    sf::Color fillColor(189, 252, 193);
    sf::Color outlineColor(160, 248, 162);

    for (const auto &leisure : m_Model.Leisures()) {
        DrawMultipolygon(window, leisure, fillColor, outlineColor, 1.0f);
    }
}

void Render::DrawWater(sf::RenderWindow &window) {
    sf::Color waterColor(155, 201, 215);

    for (const auto &water : m_Model.Waters()) {
        DrawMultipolygon(window, water, waterColor);
    }
}

void Render::DrawBuildings(sf::RenderWindow &window) {
    sf::Color fillColor(208, 197, 190);
    sf::Color outlineColor(181, 167, 154);

    for (const auto &building : m_Model.Buildings()) {
        DrawMultipolygon(window, building, fillColor, outlineColor, 1.0f);
    }
}

void Render::DrawRailways(sf::RenderWindow &window) {
    const auto &ways = m_Model.Ways().data();
    const auto &nodes = m_Model.Nodes().data();

    for (const auto &railway : m_Model.Railways()) {
        const auto &way = ways[railway.way];
        if (way.nodes.empty()) continue;

        // Draw outer railway line
        for (size_t i = 0; i < way.nodes.size() - 1; i++) {
            const auto &node1 = nodes[way.nodes[i]];
            const auto &node2 = nodes[way.nodes[i + 1]];

            sf::Vector2f p1 = ToScreen(node1.x, node1.y);
            sf::Vector2f p2 = ToScreen(node2.x, node2.y);

            sf::Vector2f direction = p2 - p1;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float angleRadians = std::atan2(direction.y, direction.x);

            // Outer line (dark grey)
            sf::RectangleShape outerLine(sf::Vector2f(length, 3.0f * m_PixelsInMeter));
            outerLine.setPosition(p1);
            outerLine.setRotation(sf::radians(angleRadians));
            outerLine.setFillColor(sf::Color(93, 93, 93));
            window.draw(outerLine);

            // Inner dashed line (white) - simplified as solid for now
            sf::RectangleShape innerLine(sf::Vector2f(length, 2.0f * m_PixelsInMeter));
            innerLine.setPosition(p1);
            innerLine.setRotation(sf::radians(angleRadians));
            innerLine.setFillColor(sf::Color::White);
            window.draw(innerLine);
        }
    }
}
