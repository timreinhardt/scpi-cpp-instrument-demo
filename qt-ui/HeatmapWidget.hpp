#pragma once

#include "FieldGrid.hpp"

#include <QWidget>

class HeatmapWidget : public QWidget
{
public:
    explicit HeatmapWidget(QWidget *parent = nullptr);

    void setGridData(const FieldGrid& grid);
    void setFixedScale(double minDbm, double maxDbm);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor colourForValue(double value) const;

    FieldGrid grid_;

    double minDbm_ = -85.0;
    double maxDbm_ = -40.0;
};