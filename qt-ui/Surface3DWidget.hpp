#pragma once

#include "FieldGrid.hpp"

#include <QWidget>

class Q3DSurface;
class QSurfaceDataProxy;
class QSurface3DSeries;

class Surface3DWidget : public QWidget
{
public:
    explicit Surface3DWidget(QWidget *parent = nullptr);

    void setGridData(const FieldGrid& grid);
    void clear();

private:
    void setupGraph();
    void updateSurfaceData();

    FieldGrid grid_;

    Q3DSurface *graph_ = nullptr;
    QSurfaceDataProxy *dataProxy_ = nullptr;
    QSurface3DSeries *series_ = nullptr;
};