#ifndef __MIPS_Simulator__MemoryView__
#define __MIPS_Simulator__MemoryView__

#include <QObject>
#include <QtWidgets>
#include <string>
#include <iostream>

class MemoryView : public QWidget {
    Q_OBJECT
public:
    MemoryView(int size) {
        _size = size;
        QGridLayout *mmLayout = new QGridLayout;
        mmsa = new QScrollArea;
        QGroupBox *tmpGroup2 = new QGroupBox;
        QGridLayout *tmpLayout2 = new QGridLayout;
        memView = new QLabel*[_size];
        for(int i = 0; i < _size; ++i) {
            std::string lb = std::to_string(i)+":";
            memView[i] = new QLabel(tr("0"));
            tmpLayout2->addWidget(new QLabel(lb.c_str()), i, 0);
            tmpLayout2->addWidget(memView[i], i, 1);
        }
        tmpGroup2->setLayout(tmpLayout2);
        tmpGroup2->setMinimumWidth(150);
        mmsa->setWidget(tmpGroup2);
        mmLayout->addWidget(new QLabel("index:"), 0, 0);
        mmLayout->addWidget(new QLabel("data"), 0, 1);
        mmLayout->addWidget(mmsa, 1, 0, 1, 2);
        setLayout(mmLayout);
    }
    ~MemoryView() {
        for (int i = 0; i < _size; ++i) {
            delete memView[i];
        }
        delete[] memView;
        delete mmsa;
    }

public slots:
    void update(uint8_t *data, uint32_t add, int len) {
        for (int i = add; i<add+len; ++i) {
            memView[i]->setText(std::to_string((int)data[i]).c_str());
        }
    }
private:
    QScrollArea *mmsa;
    QLabel **memView;
    int _size;
};

#endif
