#include "SelectionDialog.h"
#include "SelectionViewModel.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    constexpr int defaultCapacity = 10;
    SelectionViewModel viewModel(defaultCapacity);
    SelectionDialog dialog(&viewModel);

    if (dialog.exec() == QDialog::Accepted) {
        for (const auto &item : viewModel.selection())
            qInfo().noquote() << item.id << item.name;
    }
    return 0;
}
