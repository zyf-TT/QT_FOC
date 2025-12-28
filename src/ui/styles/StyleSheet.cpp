#include "StyleSheet.h"

QString StyleSheet::getApplicationStyleSheet()
{
    return R"(
QMainWindow {
    background-color: #f5f5f7;
}
QGroupBox {
    font-weight: 600;
    font-size: 14px;
    border: none;
    border-radius: 12px;
    background-color: #ffffff;
    padding: 20px;
    margin-top: 10px;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 15px;
    padding: 0 8px;
    color: #1d1d1f;
}
QLabel {
    color: #1d1d1f;
    font-size: 13px;
}
QComboBox {
    border: 1px solid #d2d2d7;
    border-radius: 8px;
    padding: 8px 12px;
    background-color: #ffffff;
    font-size: 13px;
    color: #1d1d1f;
}
QComboBox:focus {
    border: 1px solid #007aff;
    outline: none;
}
QPushButton {
    background-color: #007aff;
    color: #ffffff;
    border: none;
    border-radius: 8px;
    padding: 10px 20px;
    font-size: 14px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #0051d5;
}
QPushButton:pressed {
    background-color: #0040a8;
}
QPushButton:disabled {
    background-color: #d2d2d7;
    color: #8e8e93;
}
QTextEdit {
    border: 1px solid #d2d2d7;
    border-radius: 8px;
    background-color: #ffffff;
    padding: 10px;
    font-family: monospace;
    font-size: 12px;
    color: #1d1d1f;
}
QDoubleSpinBox {
    border: 1px solid #d2d2d7;
    border-radius: 8px;
    padding: 8px 12px;
    background-color: #ffffff;
    font-size: 13px;
    color: #1d1d1f;
}
QDoubleSpinBox:focus {
    border: 1px solid #007aff;
}
QRadioButton {
    font-size: 13px;
    color: #1d1d1f;
    spacing: 8px;
}
QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border-radius: 9px;
    border: 2px solid #d2d2d7;
    background-color: #ffffff;
}
QRadioButton::indicator:hover {
    border: 2px solid #007aff;
}
QRadioButton::indicator:checked {
    border: 2px solid #007aff;
    background: qradialgradient(cx: 0.5, cy: 0.5, radius: 0.4, stop: 0 #007aff, stop: 0.4 #007aff, stop: 0.5 #ffffff, stop: 1 #ffffff);
}
QFrame[card="true"] {
    background-color: #ffffff;
    border-radius: 12px;
    border: none;
}
QProgressBar {
    border: 1px solid #d2d2d7;
    border-radius: 4px;
    background-color: #f5f5f7;
    text-align: center;
    height: 20px;
}
QProgressBar::chunk {
    background-color: #007aff;
    border-radius: 3px;
}
)";
}

QString StyleSheet::getCardStyle()
{
    return R"(
QFrame {
    background-color: #ffffff;
    border-radius: 12px;
    border: none;
}
)";
}

QString StyleSheet::getNavBarButtonStyle()
{
    return R"(
QPushButton {
    background-color: transparent;
    border: none;
    padding: 10px 15px;
    font-size: 14px;
    color: #8e8e93;
    text-align: left;
}
QPushButton:hover {
    color: #1d1d1f;
    background-color: #f5f5f7;
}
QPushButton[active="true"] {
    color: #007aff;
    font-weight: 600;
    border-bottom: 2px solid #007aff;
}
)";
}

QString StyleSheet::getStatusBadgeStyle()
{
    return R"(
QLabel {
    border-radius: 12px;
    padding: 4px 12px;
    font-size: 12px;
    font-weight: 600;
    color: #ffffff;
}
)";
}

QString StyleSheet::getProgressBarStyle()
{
    return R"(
QProgressBar {
    border: 1px solid #d2d2d7;
    border-radius: 4px;
    background-color: #f5f5f7;
    text-align: center;
    height: 20px;
}
QProgressBar::chunk {
    background-color: #007aff;
    border-radius: 3px;
}
)";
}

