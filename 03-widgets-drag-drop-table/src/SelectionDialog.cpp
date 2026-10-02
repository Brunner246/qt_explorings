#include "SelectionDialog.h"

#include "ItemTableModel.h"
#include "SelectionViewModel.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpinBox>
#include <QTableView>
#include <QVBoxLayout>

SelectionDialog::SelectionDialog(SelectionViewModel *viewModel, QWidget *parent)
    : QDialog(parent), m_viewModel(viewModel)
{
    setWindowTitle(tr("03 - Widgets Drag & Drop Table"));
    resize(720, 480);

    auto *availableBox = new QGroupBox(tr("Available items"), this);
    (new QVBoxLayout(availableBox))->addWidget(createTable(m_viewModel->availableItems()));

    auto *selectedBox = new QGroupBox(tr("Selected items"), this);
    (new QVBoxLayout(selectedBox))->addWidget(createTable(m_viewModel->selectedItems()));

    m_capacitySpin = new QSpinBox(this);
    m_capacitySpin->setRange(1, 50);
    m_summaryLabel = new QLabel(this);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *tables = new QHBoxLayout;
    tables->addWidget(availableBox);
    tables->addWidget(selectedBox);

    auto *capacityRow = new QHBoxLayout;
    capacityRow->addWidget(new QLabel(tr("Maximum items:"), this));
    capacityRow->addWidget(m_capacitySpin);
    capacityRow->addStretch();
    capacityRow->addWidget(m_summaryLabel);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(tables);
    layout->addLayout(capacityRow);
    layout->addWidget(buttons);

    connect(m_capacitySpin, &QSpinBox::valueChanged, m_viewModel, &SelectionViewModel::setCapacity);
    connect(m_viewModel, &SelectionViewModel::capacityChanged, this, &SelectionDialog::updateFromViewModel);
    connect(m_viewModel, &SelectionViewModel::summaryChanged, this, &SelectionDialog::updateFromViewModel);
    updateFromViewModel();
}

QTableView *SelectionDialog::createTable(ItemTableModel *model)
{
    auto *table = new QTableView(this);
    table->setModel(model);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    table->setDragDropMode(QAbstractItemView::DragDrop);
    table->setDefaultDropAction(Qt::MoveAction);
    table->setDragDropOverwriteMode(false);
    table->setDropIndicatorShown(true);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setSectionResizeMode(ItemTableModel::NameColumn, QHeaderView::Stretch);
    return table;
}

void SelectionDialog::updateFromViewModel() const
{
    m_capacitySpin->setValue(m_viewModel->capacity());
    m_summaryLabel->setText(m_viewModel->summary());
    m_summaryLabel->setStyleSheet(m_viewModel->isFull() ? QStringLiteral("color: #c0392b; font-weight: bold;")
                                                        : QString());
}
