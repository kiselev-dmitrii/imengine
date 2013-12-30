#ifndef TREENODE_H
#define TREENODE_H

#include <list>

namespace imEngine {

class TreeNode;
typedef std::list<TreeNode*> TreeNodeList;

/** @brief Класс TreeNode представляет собой вершину дерева.
 *  Данный класс будет базовым для классов Widget, SceneNode и т.д
 */
class TreeNode {
public:
        /// Конструктор.
        /// Parent - указатель на родителя, к которому автоматически приписывается данный потомок
        /// Если parent == 0, то элемент дерева считается корневым
        explicit                TreeNode(TreeNode* parent = nullptr);
        /// Деструктор. Удаляет также все дочерние элементы.
        virtual                 ~TreeNode();

        /// Возвращает указатель на родителя
        TreeNode*               parent() const;
        /// Устанавливает нового родителя. Автоматически добавляет/удаляет потомка новом/старом элементе
        void                    setParent(TreeNode* parent);

        /// Возвращает констунтную ссылку на список указателей на дочерние TreeNode
        const TreeNodeList&     children() const;

private:
        void            addChild(TreeNode* node);
        bool            removeChild(TreeNode* node);
        void            removeAllChildren();

protected:
        TreeNode*       m_parent;
        TreeNodeList    m_children;
};

} //namespace imEngine

#endif // TREENODE_H
