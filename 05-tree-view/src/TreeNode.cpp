#include "TreeNode.h"

#include <algorithm>

TreeNode::TreeNode(QString name, const NodeType type) : m_name(std::move(name)), m_type(type) {}

std::optional<NodeType> TreeNode::childType() const
{
    switch (m_type) {
    case NodeType::Root:
        return NodeType::Project;
    case NodeType::Project:
        return NodeType::Building;
    case NodeType::Building:
        return NodeType::Floor;
    case NodeType::Floor:
        return NodeType::Element;
    case NodeType::Element:
        return std::nullopt;
    }
    return std::nullopt;
}

TreeNode *TreeNode::child(const int row) const { return row >= 0 && row < childCount() ? m_children[row].get() : nullptr; }

int TreeNode::row() const
{
    if (!m_parent)
        return 0;

    const auto &siblings = m_parent->m_children;
    const auto it = std::ranges::find_if(siblings, [this](const auto &sibling) { return sibling.get() == this; });
    return static_cast<int>(std::distance(siblings.begin(), it));
}

int TreeNode::elementCount() const
{
    if (m_type == NodeType::Element)
        return 1;

    int count = 0;
    for (const auto &child : m_children)
        count += child->elementCount();
    return count;
}

TreeNode *TreeNode::addChild(QString name)
{
    return insertChild(childCount(), std::make_unique<TreeNode>(std::move(name), childType().value()));
}

TreeNode *TreeNode::insertChild(const int row, std::unique_ptr<TreeNode> child)
{
    child->m_parent = this;
    return m_children.insert(m_children.begin() + row, std::move(child))->get();
}

void TreeNode::removeChildren(const int row, const int count)
{
    m_children.erase(m_children.begin() + row, m_children.begin() + row + count);
}
