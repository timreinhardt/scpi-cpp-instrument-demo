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

    // Future-proof: lets MainWindow later set this from SCPI
    // e.g. :FREQ:CENT? and :FREQ:SPAN?
    void setFrequencyRangeMHz(double startMHz, double stopMHz);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<double> traceData_;

    bool autoScale_ = false;
    bool showGrid_ = true;

    double fixedMinDbm_ = -100.0;
    double fixedMaxDbm_ = -40.0;

    double startFreqMHz_ = 2350.0;
    double stopFreqMHz_ = 2450.0;
};