//
//  IlluminationModel.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#include "IlluminationModel.hpp"

using namespace Facehack;

IlluminationModel::IlluminationModel()
{}

IlluminationModel::~IlluminationModel()
{}

bool    IlluminationModel::Initialize(const float* const coeffsR,
                                      const float* const coeffsG,
                                      const float* const coeffsB)
{
    return true;
}

bool    IlluminationModel::Initialize()
{
    return true;
}

void    IlluminationModel::Finalize()
{}

