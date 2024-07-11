#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <windows.h>
#include <tlhelp32.h>
#include <QBoxLayout>
#include <QFileIconProvider>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->addSpacing(60);
    mainLayout->addWidget(ui->treeWidget);

    populateWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateWidgets() {
    QFileIconProvider iconProvider;

    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (Process32First(hsnap, &processEntry)) {
        do {
            QString filePath = QString::fromWCharArray(processEntry.szExeFile);

            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);

            // for setting text for the first column (process name)
            item->setText(0, QString::fromWCharArray(processEntry.szExeFile));


            // for setting icons
            QIcon icon = iconProvider.icon(QFileInfo(filePath));
            item->setIcon(0, icon);

            // for setting text for the second column (PID)
            item->setText(1, QString::number(processEntry.th32ProcessID));



        } while (Process32Next(hsnap, &processEntry));
    }

    CloseHandle(hsnap);

    // for setting column headers
    ui->treeWidget->setHeaderLabels({"Process Name", "PID","Cpu"});
    ui->treeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


}
