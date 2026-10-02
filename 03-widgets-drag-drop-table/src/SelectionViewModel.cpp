#include "SelectionViewModel.h"

#include "ItemTableModel.h"

namespace
{
QList<Item> sampleItems()
{
    const QList<std::pair<QString, QString>> elements = {
        {QStringLiteral("Beam 120×240"), QStringLiteral("Beam")},
        {QStringLiteral("Beam 140×280"), QStringLiteral("Beam")},
        {QStringLiteral("Beam 160×320"), QStringLiteral("Beam")},
        {QStringLiteral("Column 160×160"), QStringLiteral("Column")},
        {QStringLiteral("Column 200×200"), QStringLiteral("Column")},
        {QStringLiteral("Rafter 80×200"), QStringLiteral("Rafter")},
        {QStringLiteral("Rafter 100×220"), QStringLiteral("Rafter")},
        {QStringLiteral("Purlin 120×200"), QStringLiteral("Purlin")},
        {QStringLiteral("CLT Panel 100 mm"), QStringLiteral("Panel")},
        {QStringLiteral("CLT Panel 140 mm"), QStringLiteral("Panel")},
        {QStringLiteral("OSB Board 18 mm"), QStringLiteral("Panel")},
        {QStringLiteral("Stud 60×120"), QStringLiteral("Stud")},
        {QStringLiteral("Stud 60×160"), QStringLiteral("Stud")},
        {QStringLiteral("Sill Plate 120×120"), QStringLiteral("Plate")},
        {QStringLiteral("Top Plate 60×120"), QStringLiteral("Plate")},
        {QStringLiteral("Steel Bracket A"), QStringLiteral("Connector")},
        {QStringLiteral("Steel Bracket B"), QStringLiteral("Connector")},
        {QStringLiteral("Dowel Ø12"), QStringLiteral("Connector")},
        {QStringLiteral("Screw 8×240"), QStringLiteral("Connector")},
        {QStringLiteral("Glulam GL24h"), QStringLiteral("Beam")},
    };

    QList<Item> items;
    int id = 1;
    for (const auto &[name, category] : elements)
        items.append(Item{id++, name, category});
    return items;
}
} // namespace

SelectionViewModel::SelectionViewModel(int capacity, QObject *parent)
    : QObject(parent), m_available(new ItemTableModel(std::nullopt, this)),
      m_selected(new ItemTableModel(capacity, this))
{
    m_available->setItems(sampleItems());

    connect(m_selected, &ItemTableModel::countChanged, this, &SelectionViewModel::summaryChanged);
    connect(m_selected, &ItemTableModel::capacityChanged, this, &SelectionViewModel::capacityChanged);
    connect(m_selected, &ItemTableModel::capacityChanged, this, &SelectionViewModel::summaryChanged);
}

int SelectionViewModel::capacity() const { return m_selected->capacity().value_or(0); }

void SelectionViewModel::setCapacity(int capacity) const { m_selected->setCapacity(capacity); }

QString SelectionViewModel::summary() const
{
    return tr("%1 / %2 selected").arg(m_selected->rowCount()).arg(capacity());
}

bool SelectionViewModel::isFull() const { return m_selected->isFull(); }

QList<Item> SelectionViewModel::selection() const { return m_selected->items(); }
