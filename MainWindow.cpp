#include "MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    connect(pbGenerate, &QPushButton::clicked, this, &MainWindow::generate);
    connect(pbAutoBFS, &QPushButton::toggled, gvPathView, &PathView::setAutoBFS);

    connect(gvPathView, &PathView::errorChanged, this, &MainWindow::showWarning);

    loadSettings();
}

void MainWindow::generate()
{
    gvPathView->generateField(sbWidth->value(), sbHeight->value());
}

void MainWindow::showWarning(const QString &text)
{
    QMessageBox::warning(this, tr("Pathfinder"), text, QMessageBox::Apply);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
}

void MainWindow::loadSettings()
{
    qDebug() << m_settings.value("geometry");
    restoreGeometry(m_settings.value("geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    m_settings.setValue("geometry", this->saveGeometry());

    qDebug() << m_settings.value("geometry");
}
