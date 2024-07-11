#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <windows.h>
#include <tlhelp32.h>
#include <QBoxLayout>
#include <QFileIconProvider>
#include <Psapi.h>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <ShlObj.h>
#include "dialog.h"
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
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(ui->pushButton,&QPushButton::pressed,this,&MainWindow::linesearch);
    connect(ui->pushButton_2,&QPushButton::pressed,this,&MainWindow::info);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateWidgets() {
    ui->treeWidget->clear();
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

            HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processid);


            BOOL isWow64 = FALSE;
            IsWow64Process(processHandle, &isWow64);
            if(isWow64){
                item->setText(2,QString("x86"));
            }
            else{
                item->setText(2,QString("x64"));

            }


            DWORD exitCode = 0;
            if (GetExitCodeProcess(processHandle, &exitCode)&& exitCode == STILL_ACTIVE) {
                    item->setText(3, QString("Running"));
            }
                else {
                    item->setText(3, QString("-"));
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
                if (GetExitCodeProcess(processHandle, &exitCode)&& exitCode == STILL_ACTIVE) {
                    item->setText(3, QString("Running"));
                }
                else {
                    item->setText(3, QString("-"));
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

    if(text.isEmpty()){
        populateWidgets();
    }
    CloseHandle(hsnap);



}
void MainWindow::showContextMenu() {
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    QMenu contextMenu(this);
    QAction *terminateAction = contextMenu.addAction("Terminate Process");
    QAction *infoAction = contextMenu.addAction("Process Information");
    QAction *locationAction = contextMenu.addAction("Open file location");
    contextMenu.setStyleSheet(
        "QMenu {"
        "    background-color: rgb(51, 51, 51);"
        "    color: rgb(255, 255, 255);"
        "    border: 1px solid rgb(85, 85, 85);"
        "}"
        "QMenu::item {"
        "    background-color: transparent;"
        "}"
        "QMenu::item:selected {"
        "    background-color: rgb(85, 85, 85);"
        "}"
        );

    connect(terminateAction,&QAction::triggered,this,[this,item](){
        DWORD64 processId = item->text(1).toULong();
        HANDLE processh = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
        if(processh){
            if(TerminateProcess(processh,0)){
                QMessageBox::information(this, "Success", "Process terminated successfully.");
                    linesearch(); // Execute your function or method

            }
            else{
                QMessageBox::information(this, "Failed", "Failed to terminate process.");

            }

        }
        else {
            QMessageBox::warning(this, "Error", "Unable to open process for termination.");
        }
    });
    connect(infoAction,&QAction::triggered,this,[this,item](){
         DWORD64 processId = item->text(1).toULong();
        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
         if (processHandle) {
            QString processInfo = QString("Process Name: %1\nPID: %2\nArchitecture: %3\nStatus: %4\nMemory: %5")
                                      .arg(item->text(0), item->text(1), item->text(2), item->text(3), item->text(4));

              QMessageBox::information(this, "Process Information", processInfo);
            CloseHandle(processHandle);

             }

        else {
             QMessageBox::warning(this, "Error", "Unable to open process for information.");
         }


    });
    connect(locationAction,&QAction::triggered,this,[this,item](){

        DWORD processId = item->text(1).toULong();
        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        if (processHandle) {
            WCHAR processPath[MAX_PATH];
            DWORD bufferSize = sizeof(processPath);

            if (QueryFullProcessImageNameW(processHandle, 0, processPath, &bufferSize)) {
                QString filePath = QString::fromWCharArray(processPath);
                QFileInfo fileInfo(filePath);
                QString directoryPath = fileInfo.path();

                ShellExecuteW(nullptr, L"open", reinterpret_cast<LPCWSTR>(directoryPath.utf16()), nullptr, nullptr, SW_SHOWNORMAL);
            } else {
                QMessageBox::warning(this, "Error", "Failed to retrieve process path.");
            }

            CloseHandle(processHandle);
        } else {
            QMessageBox::warning(this, "Error", "Unable to open process for location.");
        }

    });

    contextMenu.exec(QCursor::pos());
}

void MainWindow::info(){
    Dialog dia;
    dia.exec();
}
