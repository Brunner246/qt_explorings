#pragma once

#include <QDialog>

#include <memory>

class QQuickItem;
class QQuickWidget;
class SelectionViewModel;

class SelectionDialog: public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(SelectionViewModel *viewModel, QWidget *parent = nullptr);
    ~SelectionDialog() override;

private:
    void loadView();

    SelectionViewModel *m_viewModel = nullptr;
    QQuickWidget *m_quickWidget = nullptr;
    std::unique_ptr<QQuickItem> m_view;
};
