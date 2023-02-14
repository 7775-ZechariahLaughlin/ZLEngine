#pragma once

#include <iostream>;
#include <vector>;

using namespace std;

typedef unsigned int zluint;

//class/type definitions

class GraphicsEngine;
typedef shared_ptr<GraphicsEngine> GraphicsEnginePtr;

class VertexArrayObject;
typedef VertexArrayObject VAO;
typedef shared_ptr<VAO> VAOPtr;
typedef vector<VAOPtr> VAOStack;

//useful for shape data and VAOs Matrices
typedef vector<float> PositionMatrix;
typedef vector<zluint> IndicesMatrix;

//this structure will handle matrices for VAOs
struct ShapeData {
	vector<float> PositionMatrix;
	vector<zluint> IndicesMatrix;
};
