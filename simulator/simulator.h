#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QMainWindow>
#include <QtWidgets>

class Simulator : public QMainWindow {
    Q_OBJECT

public:
    explicit Simulator(QWidget *parent = 0);
    ~Simulator();

private slots:
    void memOpen();
    void memSave();
private:
    QMenu *memMenu;
    QAction *openAct;
    QAction *saveAct;

    QLabel *clkLb;
    unsigned long clk;

    QLineEdit *addLE;
    QLineEdit *valLE;
    QPushButton *loadPB;
    QPushButton *storePB;
    QTableWidget *cacheTW;
    QTableWidget *memTW;

};

#endif // SIMULATOR_H
