#ifndef DETAILWINDOW_H
#define DETAILWINDOW_H

#include <QDialog>
#include <helper.h>

namespace Ui {
class detailWindow;
}

class detailWindow : public QDialog
{
    Q_OBJECT

public:
    explicit detailWindow(vector<MESSAGE_STRUCT>,QWidget *parent = 0);
    ~detailWindow();

private:
    Ui::detailWindow *ui;
    MESSAGE_STRUCT message;
};

#endif // DETAILWINDOW_H
