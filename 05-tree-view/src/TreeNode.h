#pragma once

#include <QString>

#include <memory>
#include <optional>
#include <vector>

enum class NodeType
{
    Root,
    Project,
    Building,
    Floor,
    Element
};

class TreeNode
{
public:
    TreeNode(QString name, NodeType type);

    [[nodiscard]] const QString &name() const { return m_name; }
    void setName(QString name) { m_name = std::move(name); }
    [[nodiscard]] NodeType type() const { return m_type; }
    [[nodiscard]] std::optional<NodeType> childType() const;

    [[nodiscard]] TreeNode *parent() const { return m_parent; }
    [[nodiscard]] TreeNode *child(int row) const;
    [[nodiscard]] int childCount() const { return static_cast<int>(m_children.size()); }
    [[nodiscard]] int row() const;
    [[nodiscard]] int elementCount() const;

    TreeNode *addChild(QString name);
    TreeNode *insertChild(int row, std::unique_ptr<TreeNode> child);
    void removeChildren(int row, int count);

private:
    QString m_name;
    NodeType m_type;
    TreeNode *m_parent = nullptr;
    std::vector<std::unique_ptr<TreeNode>> m_children;
};
