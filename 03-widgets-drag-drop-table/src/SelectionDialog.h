#pragma once

#include <QDialog>

class ItemTableModel;
class QLabel;
class QSpinBox;
class QTableView;
class SelectionViewModel;

class SelectionDialog: public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(SelectionViewModel *viewModel, QWidget *parent = nullptr);

private:
    [[nodiscard]] QTableView *createTable(ItemTableModel *model);
    void updateFromViewModel() const;

    SelectionViewModel *m_viewModel = nullptr;
    QSpinBox *m_capacitySpin = nullptr;
    QLabel *m_summaryLabel = nullptr;
};
