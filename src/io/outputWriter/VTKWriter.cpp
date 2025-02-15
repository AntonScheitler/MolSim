/*
 * VTKWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "VTKWriter.h"
#include "particle/container/ParticleContainer.h"
#include "spdlogConfig.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace outputWriter {

    VTKWriter::VTKWriter(std::string baseName) : baseName(baseName){
        SPDLOG_DEBUG("Initialized VTKWriter");
    }

    void VTKWriter::plotParticles(ParticleContainer &particles, int iteration) {
        initializeOutput(particles.size());
        for (auto it = particles.begin(); *it != *(particles.end()); it->operator++()) {
            Particle& particle = **it;
            plotParticle(particle);
        }
        writeFile(baseName, iteration);
    }

    void VTKWriter::initializeOutput(int numParticles) {

        vtkFile = new VTKFile_t("UnstructuredGrid");

        // per point, we add type, position, velocity and force
        PointData pointData;
        DataArray_t mass(type::Float32, "mass", 1);
        DataArray_t velocity(type::Float32, "velocity", 3);
        DataArray_t forces(type::Float32, "force", 3);
        DataArray_t type(type::Int32, "type", 1);
        pointData.DataArray().push_back(mass);
        pointData.DataArray().push_back(velocity);
        pointData.DataArray().push_back(forces);
        pointData.DataArray().push_back(type);

        CellData cellData; // we don't have cell data => leave it empty

        // 3 coordinates
        Points points;
        DataArray_t pointCoordinates(type::Float32, "points", 3);
        points.DataArray().push_back(pointCoordinates);

        Cells cells; // we don't have cells, => leave it empty
                     // for some reasons, we have to add a dummy entry for paraview
        DataArray_t cellsData(type::Float32, "types", 0);
        cells.DataArray().push_back(cellsData);

        PieceUnstructuredGrid_t piece(pointData, cellData, points, cells,
                numParticles, 0);
        UnstructuredGrid_t unstructuredGrid(piece);
        vtkFile->UnstructuredGrid(unstructuredGrid);
    }

    void VTKWriter::writeFile(const std::string &filename, int iteration) {
        std::stringstream strstr;
        strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration
            << ".vtu";

        std::ofstream file(strstr.str().c_str());
        VTKFile(file, *vtkFile);
        delete vtkFile;
    }

    void VTKWriter::plotParticle(Particle &p) {
        if (vtkFile->UnstructuredGrid().present()) {
            SPDLOG_TRACE("UnstructuredGrid is present");
        } else {
            SPDLOG_ERROR("No UnstructuredGrid present");
        }

        PointData::DataArray_sequence &pointDataSequence =
            vtkFile->UnstructuredGrid()->Piece().PointData().DataArray();
        PointData::DataArray_iterator dataIterator = pointDataSequence.begin();

        dataIterator->push_back(p.getM());

        dataIterator++;
        dataIterator->push_back(p.getV()[0]);
        dataIterator->push_back(p.getV()[1]);
        dataIterator->push_back(p.getV()[2]);

        dataIterator++;
        dataIterator->push_back(p.getOldF()[0]);
        dataIterator->push_back(p.getOldF()[1]);
        dataIterator->push_back(p.getOldF()[2]);

        dataIterator++;
        dataIterator->push_back(p.getType());

        Points::DataArray_sequence &pointsSequence =
            vtkFile->UnstructuredGrid()->Piece().Points().DataArray();
        Points::DataArray_iterator pointsIterator = pointsSequence.begin();
        pointsIterator->push_back(p.getX()[0]);
        pointsIterator->push_back(p.getX()[1]);
        pointsIterator->push_back(p.getX()[2]);
    }

} // namespace outputWriter
