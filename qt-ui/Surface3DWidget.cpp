#include "Surface3DWidget.hpp"

#include <QVBoxLayout>
#include <QWidget>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QValue3DAxis>

Surface3DWidget::Surface3DWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(300);
    setupGraph();
}

void Surface3DWidget::setupGraph()
{
    graph_ = new Q3DSurface();

    QWidget *container = QWidget::createWindowContainer(graph_, this);
    container->setMinimumHeight(300);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(container);
    layout->setContentsMargins(0, 0, 0, 0);

    dataProxy_ = new QSurfaceDataProxy();
    series_ = new QSurface3DSeries(dataProxy_);

    graph_->addSeries(series_);

    graph_->axisX()->setTitle("X Position");
    graph_->axisX()->setTitleVisible(true);

    graph_->axisY()->setTitle("Power dBm");
    graph_->axisY()->setTitleVisible(true);

    graph_->axisZ()->setTitle("Y Position");
    graph_->axisZ()->setTitleVisible(true);

    graph_->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    graph_->activeTheme()->setType(Q3DTheme::ThemeQt);
}

void Surface3DWidget::setGridData(const FieldGrid& grid)
{
    grid_ = grid;
    updateSurfaceData();
}

void Surface3DWidget::clear()
{
    grid_.clear();

    if (dataProxy_)
    {
        dataProxy_->resetArray(new QSurfaceDataArray);
    }
}

void Surface3DWidget::updateSurfaceData()
{
    if (!dataProxy_)
    {
        return;
    }

    if (grid_.empty() || grid_.front().empty())
    {
        clear();
        return;
    }

    const int rows = static_cast<int>(grid_.size());
    const int cols = static_cast<int>(grid_.front().size());

    auto *dataArray = new QSurfaceDataArray;
    dataArray->reserve(rows);

    for (int y = 0; y < rows; ++y)
    {
        auto *dataRow = new QSurfaceDataRow(cols);

        for (int x = 0; x < cols; ++x)
        {
            float xPos = static_cast<float>(x);
            float zPos = static_cast<float>(y);
            float power = static_cast<float>(grid_[y][x]);

            (*dataRow)[x].setPosition(QVector3D(xPos, power, zPos));
        }

        *dataArray << dataRow;
    }

    dataProxy_->resetArray(dataArray);

    graph_->axisX()->setRange(0.0f, static_cast<float>(cols - 1));
    graph_->axisZ()->setRange(0.0f, static_cast<float>(rows - 1));
    graph_->axisY()->setRange(-85.0f, -40.0f);
}