#include "TreeNode.h"
#include "Debug.h"

namespace imEngine {

TreeNode::TreeNode(TreeNode* parent) : m_parent(nullptr) {
        setParent(parent);
}

TreeNode::~TreeNode() {
        if (m_parent) m_parent->removeNodeFromChildrenList(this);
        deleteAllChildren();
}

TreeNode* TreeNode::parent() const {
        return m_parent;
}

void TreeNode::setParent(TreeNode *parent) {
        if (m_parent) m_parent->removeNodeFromChildrenList(this);
        if (parent) parent->addNodeToChildrenList(this);
        m_parent = parent;
}

void TreeNode::attachChild(TreeNode *node) {
        node->setParent(this);
}

void TreeNode::detachChild(TreeNode *node) {
        node->setParent(nullptr);
}

const TreeNodeList& TreeNode::children() const {
        return m_children;
}

bool TreeNode::isChildNode(TreeNode *node) const {
        for (TreeNode* item: m_children) if (item == node) return true;
        return false;
}

bool TreeNode::addNodeToChildrenList(TreeNode *node) {
        if (isChildNode(node)) {
                IM_ERROR("Node already added");
                return false;
        } else {
                m_children.push_back(node);
                onAttachChild(node);
                return true;
        }
}

bool TreeNode::removeNodeFromChildrenList(TreeNode *node) {
        for (auto it = m_children.begin(); it != m_children.end(); ++it) {
                if (*it == node) {
                        m_children.erase(it);
                        onDetachChild(node);
                        return true;
                }
        }
        return false;
}

void TreeNode::deleteAllChildren() {
        // Копируем указатели на детей, так как при удалении массив m_children может измениться из
        // дочерних объектов, а нельзя удалять и добавлять элементы в контейнер во время итерации
        auto children = m_children;
        for (TreeNode* node: children) delete node;
        m_children.clear();
}

} //namespace imEngine
