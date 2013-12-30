#include "TreeNode.h"

namespace imEngine {

TreeNode::TreeNode(TreeNode *parent) :
        m_parent(nullptr)
{
        setParent(parent);
}

TreeNode::~TreeNode() {
        removeAllChildren();
}

TreeNode* TreeNode::parent() const {
        return m_parent;
}

void TreeNode::setParent(TreeNode *parent) {
        if (m_parent) m_parent->removeChild(this);
        if (parent) parent->addChild(this);
        m_parent = parent;
}

const TreeNodeList& TreeNode::children() const {
        return m_children;
}

void TreeNode::addChild(TreeNode *node) {
        m_children.push_back(node);
}

bool TreeNode::removeChild(TreeNode *node) {
        for (auto it = m_children.begin(); it != m_children.end(); ++it) {
                if (*it == node) {
                        m_children.erase(it);
                        return true;
                }
        }
        return false;
}

void TreeNode::removeAllChildren() {
        for (TreeNode* node: m_children) delete node;
        m_children.erase(m_children.begin(), m_children.end());
}

} //namespace imEngine
