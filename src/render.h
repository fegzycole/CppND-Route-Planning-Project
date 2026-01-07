#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "route_model.h"

class Render {
public:
    Render(RouteModel &model);
    void Display();

private:
    void DrawLanduses(sf::RenderWindow &window);
    void DrawLeisure(sf::RenderWindow &window);
    void DrawWater(sf::RenderWindow &window);
    void DrawRailways(sf::RenderWindow &window);
    void DrawBuildings(sf::RenderWindow &window);
    void DrawRoads(sf::RenderWindow &window);
    void DrawPath(sf::RenderWindow &window);
    void DrawStartPosition(sf::RenderWindow &window);
    void DrawEndPosition(sf::RenderWindow &window);

    void DrawMultipolygon(sf::RenderWindow &window, const Model::Multipolygon &mp, sf::Color fillColor, sf::Color outlineColor = sf::Color::Transparent, float outlineThickness = 0.0f);
    void DrawWay(sf::RenderWindow &window, const Model::Way &way, sf::Color color, float width);

    sf::Vector2f ToScreen(float x, float y);
    sf::Color GetRoadColor(Model::Road::Type type);
    float GetRoadWidth(Model::Road::Type type);
    sf::Color GetLanduseColor(Model::Landuse::Type type);

    RouteModel &m_Model;
    float m_Scale;
    float m_PixelsInMeter;
    unsigned int m_WindowWidth = 800;
    unsigned int m_WindowHeight = 800;
};
