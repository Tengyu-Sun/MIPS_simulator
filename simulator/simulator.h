#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QMainWindow>
#include <QtWidgets>



class Simulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Simulator(char* mem, QWidget *parent = 0);
    ~Simulator();

private slots:

private:
    QMenu *test;
    QToolBar *too;
    QLineEdit *addLE;
    QLineEdit *valLE;
    QPushButton *loadPB;
    QPushButton *storePB;
    QTableWidget *memTW;
    char *memsys;
};

#endif // SIMULATOR_H
