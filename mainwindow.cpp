#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <windows.h>
#include <tlhelp32.h>
#include <QBoxLayout>
#include <QFileIconProvider>
#include <Psapi.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->addSpacing(60);
    mainLayout->addWidget(ui->treeWidget);

    populateWidgets();
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::linesearch);
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
            processid=processEntry.th32ProcessID;
            QString filePath = QString::fromWCharArray(processEntry.szExeFile);

            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);

            // for setting text for the first column (process name)
            item->setText(0, QString::fromWCharArray(processEntry.szExeFile));


            // for setting icons
            QIcon icon = iconProvider.icon(QFileInfo(filePath));
            item->setIcon(0, icon);

            // for setting text for the second column (PID)
            item->setText(1, QString::number(processEntry.th32ProcessID));

            HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, processid);


            BOOL isWow64 = FALSE;
            IsWow64Process(processHandle, &isWow64);
            if(isWow64){
                item->setText(2,QString("x86"));
            }
            else{
                item->setText(2,QString("x64"));

            }


                DWORD exitCode = 0;
                if (GetExitCodeProcess(processHandle, &exitCode) && exitCode == STILL_ACTIVE) {
                    item->setText(3, QString("Running"));
                } else {
                    item->setText(3, QString("Unknown"));
                }



            PROCESS_MEMORY_COUNTERS_EX pmc;
            if (GetProcessMemoryInfo(processHandle, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                // Display memory usage (working set size)
                QString memoryUsage = QString::number(pmc.WorkingSetSize / (1024 * 1024)) + " MB";
                item->setText(4, memoryUsage);
            } else {
                item->setText(4, QString("Undefined"));
            }
            CloseHandle(processHandle);


        } while (Process32Next(hsnap, &processEntry));
    }

    CloseHandle(hsnap);

    // for setting column headers
    ui->treeWidget->setHeaderLabels({"Process Name", "PID","Architecture","Status","Memory"});
    ui->treeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


}
void MainWindow::linesearch(){
    QString text = ui->lineEdit->text();
    ui->treeWidget->clear();
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (Process32First(hsnap, &processEntry)) {
        do {
             if (!_wcsnicmp(processEntry.szExeFile, text.toStdWString().c_str(), text.length())){
                processid=processEntry.th32ProcessID;
                ui->treeWidget->setHeaderLabels({"Process Name", "PID","Architecture","Status"});

                QString filePath = QString::fromWCharArray(processEntry.szExeFile);

                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);


                item->setText(0, QString::fromWCharArray(processEntry.szExeFile));
                // for setting icons
                QFileIconProvider iconProvider;
                QIcon icon = iconProvider.icon(QFileInfo(filePath));
                item->setIcon(0, icon);



                // for setting text for the second column (PID)
                item->setText(1, QString::number(processEntry.th32ProcessID));

                HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, processid);

                BOOL isWow64 = FALSE;
                IsWow64Process(processHandle, &isWow64);
                if(isWow64){
                    item->setText(2,QString("x86"));
                }
                else{
                    item->setText(2,QString("x64"));

                }


                DWORD exitCode = 0;
                if (GetExitCodeProcess(processHandle, &exitCode) && exitCode == STILL_ACTIVE) {
                    item->setText(3, QString("Running"));
                } else {
                    item->setText(3, QString("Unknown"));
                }



                PROCESS_MEMORY_COUNTERS_EX pmc;
                if (GetProcessMemoryInfo(processHandle, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                    // Display memory usage (working set size)
                    QString memoryUsage = QString::number(pmc.WorkingSetSize / (1024 * 1024)) + " MB";
                    item->setText(4, memoryUsage);
                } else {
                    item->setText(4, QString("Undefined"));
                }
                CloseHandle(processHandle);


             }
        }while (Process32Next(hsnap, &processEntry));
    }

    if(text==""){
        populateWidgets();
    }
    CloseHandle(hsnap);



}
