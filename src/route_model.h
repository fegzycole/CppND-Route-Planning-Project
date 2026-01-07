#pragma once

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model
{

public:
  class Node : public Model::Node
  {
  public:
    Node *parent = nullptr;
    float h_value = std::numeric_limits<float>::max();
    float g_value = 0.0;
    bool visited = false;
    std::vector<Node *> neighbors;

    float Distance(Node other) const
    {
      return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
    }

    void FindNeighbors();

    Node() {}
    Node(int idx, RouteModel *search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}

  private:
    int index;
    RouteModel *parent_model = nullptr;
    Node *FindNeighbor(std::vector<int> node_indices);
  };

  RouteModel(const std::vector<std::byte> &xml);
  RouteModel::Node &FindClosestNode(float x, float y);
  std::vector<Node> path;
  auto &SNodes() { return m_Nodes; }
  auto &GetNodeToRoadMap() { return node_to_road; };

private:
  std::vector<Node> m_Nodes;
  std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;

  void CreateNodeToRoadHashmap();
};