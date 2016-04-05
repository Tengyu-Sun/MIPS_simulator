#ifndef MIPS_Simulator_ConfigDialog
#define MIPS_Simulator_ConfigDialog

#include <QObject>
#include <QtWidgets>

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog();
signals:
    void memsysConfig(int indexsize, int linesize, int ways, int cachecycle, int policy, int level, int memsize, int memcycle);
private slots:
    void memsysUpdate();
    void close();
private:
    QLineEdit *indexSizeLE;
    QLineEdit *lineSizeLE;
    QLineEdit *waysLE;
    QLineEdit *cacheCycleLE;
    QLineEdit *cacheLeveLE;
    QRadioButton *policyRB;
    QLineEdit *memSizeLE;
    QLineEdit *memCycleLE;
};

#endif
