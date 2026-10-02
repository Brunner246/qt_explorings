#pragma once

#include <QDialog>

class PersonViewModel;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QSpinBox;

class PersonDialog: public QDialog
{
    Q_OBJECT

public:
    explicit PersonDialog(PersonViewModel *viewModel, QWidget *parent = nullptr);

private:
    void createWidgets();
    void bindViewModel();
    void updateFromViewModel();

    PersonViewModel *m_viewModel = nullptr;
    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_emailEdit = nullptr;
    QSpinBox *m_ageSpin = nullptr;
    QComboBox *m_roleCombo = nullptr;
    QLabel *m_validationLabel = nullptr;
    QDialogButtonBox *m_buttons = nullptr;
};
