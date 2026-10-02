#pragma once

#include <QDialog>

#include <memory>

class PersonViewModel;
class QDialogButtonBox;
class QQuickItem;
class QQuickWidget;

class PersonDialog: public QDialog
{
    Q_OBJECT

public:
    explicit PersonDialog(PersonViewModel *viewModel, QWidget *parent = nullptr);
    ~PersonDialog() override;

private:
    void loadForm();
    void updateOkButton();

    PersonViewModel *m_viewModel = nullptr;
    QQuickWidget *m_quickWidget = nullptr;
    std::unique_ptr<QQuickItem> m_form;
    QDialogButtonBox *m_buttons = nullptr;
};
