#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <windows.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DWORD64 processid = 0;

private:
    Ui::MainWindow *ui;
    void populateWidgets();
    void linesearch();
    void showContextMenu();
    void info();
    QMenuBar *menuBar;
    void hideProtectedProcess();
};
#endif // MAINWINDOW_H
