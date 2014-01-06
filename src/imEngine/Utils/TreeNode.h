#ifndef TREENODE_H
#define TREENODE_H

#include <vector>

namespace imEngine {

class TreeNode;
typedef std::vector<TreeNode*> TreeNodeList;

/** @brief Класс TreeNode представляет собой вершину дерева.
 */
class TreeNode {
public:
        /// Создает пустую вершину с родителем parent
        explicit                TreeNode(TreeNode* parent = nullptr);
        /// Деструктор. Удаляет все дочерние вершины и отвязывается от родителя
        virtual                 ~TreeNode();

        /// Возвращает указатель на родителя
        TreeNode*               parent() const;
        /// Устанавливает нового родителя. Автоматически добавляет/удаляет потомка новом/старом элементе
        void                    setParent(TreeNode* parent);

        /// Добавляет делает узел node дочерним узлом. При этом node автоматически получает родителя this
        void                    attachChild(TreeNode* node);
        /// Удаляет узел node из списка дочерних узлов. При этом node перестает иметь родителя
        void                    detachChild(TreeNode* node);
        /// Возвращает констунтную ссылку на список указателей на дочерние TreeNode
        const TreeNodeList&     children() const;

        /// Определяет, является ли node дочерним элементом
        bool                    isChildNode(TreeNode* node) const;

private:
        bool            addNodeToChildrenList(TreeNode* node);
        bool            removeNodeFromChildrenList(TreeNode* node);
        void            deleteAllChildren();

protected:
        TreeNode*       m_parent;
        TreeNodeList    m_children;
};

} //namespace imEngine

#endif // TREENODE_H
