/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *sLabel_input;
    QLabel *sLabel_result;
    QPushButton *NextFrameButton;
    QLabel *SrcLabel;
    QLabel *ResLabel;
    QTableWidget *tableWidget;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *GrowEdit;
    QLabel *label;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *SnakeIterEdit;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *SnakeStepEdit;
    QCheckBox *CLoseCheckBox;
    QPushButton *pushButton;
    QPushButton *DetailButton;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *IMGLable;
    QSpacerItem *horizontalSpacer;
    QPushButton *SelectIMG;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(898, 658);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sLabel_input = new QLabel(centralWidget);
        sLabel_input->setObjectName(QStringLiteral("sLabel_input"));
        sLabel_input->setGeometry(QRect(210, 10, 91, 17));
        sLabel_result = new QLabel(centralWidget);
        sLabel_result->setObjectName(QStringLiteral("sLabel_result"));
        sLabel_result->setGeometry(QRect(200, 320, 91, 17));
        NextFrameButton = new QPushButton(centralWidget);
        NextFrameButton->setObjectName(QStringLiteral("NextFrameButton"));
        NextFrameButton->setGeometry(QRect(650, 310, 89, 25));
        SrcLabel = new QLabel(centralWidget);
        SrcLabel->setObjectName(QStringLiteral("SrcLabel"));
        SrcLabel->setGeometry(QRect(0, 30, 512, 256));
        SrcLabel->setFrameShape(QFrame::Box);
        ResLabel = new QLabel(centralWidget);
        ResLabel->setObjectName(QStringLiteral("ResLabel"));
        ResLabel->setGeometry(QRect(0, 340, 512, 256));
        ResLabel->setFrameShape(QFrame::Box);
        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget->rowCount() < 6)
            tableWidget->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(5, __qtablewidgetitem7);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(580, 340, 211, 231));
        tableWidget->verticalHeader()->setVisible(false);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(540, 80, 301, 221));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 30, 172, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        GrowEdit = new QLineEdit(horizontalLayoutWidget);
        GrowEdit->setObjectName(QStringLiteral("GrowEdit"));

        horizontalLayout->addWidget(GrowEdit);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 80, 171, 16));
        label->setWordWrap(false);
        horizontalLayoutWidget_2 = new QWidget(groupBox);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(0, 110, 160, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setWordWrap(false);

        horizontalLayout_2->addWidget(label_3);

        SnakeIterEdit = new QLineEdit(horizontalLayoutWidget_2);
        SnakeIterEdit->setObjectName(QStringLiteral("SnakeIterEdit"));

        horizontalLayout_2->addWidget(SnakeIterEdit);

        horizontalLayoutWidget_3 = new QWidget(groupBox);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(0, 150, 160, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setWordWrap(false);

        horizontalLayout_3->addWidget(label_4);

        SnakeStepEdit = new QLineEdit(horizontalLayoutWidget_3);
        SnakeStepEdit->setObjectName(QStringLiteral("SnakeStepEdit"));

        horizontalLayout_3->addWidget(SnakeStepEdit);

        CLoseCheckBox = new QCheckBox(groupBox);
        CLoseCheckBox->setObjectName(QStringLiteral("CLoseCheckBox"));
        CLoseCheckBox->setGeometry(QRect(190, 30, 96, 22));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(120, 190, 99, 27));
        DetailButton = new QPushButton(centralWidget);
        DetailButton->setObjectName(QStringLiteral("DetailButton"));
        DetailButton->setGeometry(QRect(800, 550, 89, 25));
        horizontalLayoutWidget_4 = new QWidget(centralWidget);
        horizontalLayoutWidget_4->setObjectName(QStringLiteral("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(540, 30, 281, 51));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        IMGLable = new QLabel(horizontalLayoutWidget_4);
        IMGLable->setObjectName(QStringLiteral("IMGLable"));

        horizontalLayout_5->addWidget(IMGLable);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        horizontalLayout_4->addLayout(horizontalLayout_5);

        SelectIMG = new QPushButton(horizontalLayoutWidget_4);
        SelectIMG->setObjectName(QStringLiteral("SelectIMG"));

        horizontalLayout_4->addWidget(SelectIMG);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 898, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PillCounter", 0));
        sLabel_input->setText(QApplication::translate("MainWindow", "\345\216\237\345\233\276\345\203\217", 0));
        sLabel_result->setText(QApplication::translate("MainWindow", "\350\256\241\346\225\260\345\233\276\345\203\217", 0));
        NextFrameButton->setText(QApplication::translate("MainWindow", "\345\244\204\347\220\206", 0));
        SrcLabel->setText(QString());
        ResLabel->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\350\215\257\344\270\270\347\247\215\347\261\273", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\346\225\260\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "1", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "2", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "3", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "4", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "5", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "6", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256\345\217\202\346\225\260", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\214\272\345\237\237\347\224\237\351\225\277\351\230\210\345\200\274\357\274\232", 0));
        GrowEdit->setText(QApplication::translate("MainWindow", "0.36", 0));
        label->setText(QApplication::translate("MainWindow", "\344\270\273\345\212\250\350\275\256\345\273\223\346\250\241\345\236\213\345\217\202\346\225\260", 0));
        label_3->setText(QApplication::translate("MainWindow", "\350\277\255\344\273\243\346\254\241\346\225\260\357\274\232", 0));
        SnakeIterEdit->setText(QApplication::translate("MainWindow", "20", 0));
        label_4->setText(QApplication::translate("MainWindow", "\346\255\245\350\277\233\357\274\232", 0));
        SnakeStepEdit->setText(QApplication::translate("MainWindow", "2.0", 0));
        CLoseCheckBox->setText(QApplication::translate("MainWindow", "\345\260\201\351\227\255\350\275\256\345\273\223", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\350\256\276\345\256\232", 0));
        DetailButton->setText(QApplication::translate("MainWindow", "\350\257\246\346\203\205", 0));
        IMGLable->setText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\345\233\276\345\203\217\357\274\201", 0));
        SelectIMG->setText(QApplication::translate("MainWindow", "\351\200\211\345\217\226\345\233\276\345\203\217", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
