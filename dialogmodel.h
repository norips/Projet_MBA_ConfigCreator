#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include <QDialog>

namespace Ui {
class DialogModel;
}

class DialogModel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModel(QWidget *parent = 0);
    ~DialogModel();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogModel *ui;
};

#endif // DIALOGMODEL_H
