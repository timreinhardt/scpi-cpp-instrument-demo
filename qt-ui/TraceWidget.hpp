#pragma once

#include <QWidget>
#include <vector>

class TraceWidget : public QWidget
{
public:
    explicit TraceWidget(QWidget *parent = nullptr);

    void setTraceData(const std::vector<double>& values);

    void setFixedScale(double minDbm, double maxDbm);
    void setAutoScale(bool enabled);
    void setShowGrid(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<double> traceData_;

    bool autoScale_ = false;
    bool showGrid_ = true;

    double fixedMinDbm_ = -100.0;
    double fixedMaxDbm_ = -40.0;
};