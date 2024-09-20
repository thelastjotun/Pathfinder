#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>
#include <QScopedPointer>
#include <QSettings>

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void generate();

    void showWarning(const QString &text);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void loadSettings();
    void saveSettings();

private:
    QSettings m_settings{ "MyOrg", "MyApp" };
};
