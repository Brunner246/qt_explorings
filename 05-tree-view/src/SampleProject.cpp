#include "SampleProject.h"

#include <QStringList>

namespace
{
void addFloor(TreeNode *building, const QString &name, const QStringList &elements)
{
    TreeNode *floor = building->addChild(name);
    for (const auto &element : elements)
        floor->addChild(element);
}
} // namespace

std::unique_ptr<TreeNode> createSampleProjects()
{
    auto root = std::make_unique<TreeNode>(QString(), NodeType::Root);

    TreeNode *residential = root->addChild(QStringLiteral("Residential Lakeside"));
    TreeNode *houseA = residential->addChild(QStringLiteral("House A"));
    addFloor(houseA,
             QStringLiteral("Ground Floor"),
             {"Sill Plate", "Exterior Wall North", "Exterior Wall South", "Column C1"});
    addFloor(houseA, QStringLiteral("First Floor"), {"Floor Beam B1", "Floor Beam B2", "CLT Ceiling Panel"});
    addFloor(houseA, QStringLiteral("Roof"), {"Rafter R1", "Rafter R2", "Rafter R3", "Ridge Purlin"});

    TreeNode *houseB = residential->addChild(QStringLiteral("House B"));
    addFloor(houseB, QStringLiteral("Ground Floor"), {"Sill Plate", "Exterior Wall East", "Interior Wall"});
    addFloor(houseB, QStringLiteral("Roof"), {"Rafter R1", "Rafter R2", "Ridge Purlin"});

    TreeNode *office = root->addChild(QStringLiteral("Office Timber Tower"));
    TreeNode *tower = office->addChild(QStringLiteral("Tower"));
    for (int level = 1; level <= 3; ++level)
        addFloor(tower,
                 QStringLiteral("Level %1").arg(level),
                 {"Glulam Column G1", "Glulam Column G2", "Glulam Beam", "CLT Slab"});

    return root;
}
