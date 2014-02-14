#ifndef TREENODE_H
#define TREENODE_H

#include "Types.h"
#include "Debug.h"
#include <vector>

namespace imEngine {


/** @brief Класс TreeNode представляет собой вершину дерева.
 */
template <typename Derived>
class TreeNode {
public:
        /// Создает узел с родителем parent
        explicit TreeNode(Derived* parent = nullptr) : m_parent(nullptr) {
                setParent(parent);
        }

        /// Деструктор. Удаляет все дочерние вершины и отвязывается от родителя
        virtual ~TreeNode() {
                if (m_parent) m_parent->removeNodeFromChildrenList(static_cast<Derived*>(this));
                deleteAllChildren();
        }

        /// Возвращает указатель на родителя
        Derived* parent() const {
                return m_parent;
        }

        /// Устанавливает нового родителя. Автоматически добавляет/удаляет потомка новом/старом элементе
        void setParent(Derived* parent) {
                if (m_parent) m_parent->removeNodeFromChildrenList(static_cast<Derived*>(this));
                if (parent) parent->addNodeToChildrenList(static_cast<Derived*>(this));
                m_parent = parent;
        }

         /// Добавляет делает узел node дочерним узлом. При этом node автоматически получает родителя this
        void attachChild(Derived* node) {
                node->setParent(static_cast<Derived*>(this));
        }

        /// Удаляет узел node из списка дочерних узлов. При этом node перестает иметь родителя
        void detachChild(Derived* node) {
                node->setParent(nullptr);
        }

        /// Возвращает констунтную ссылку на список указателей на дочерние TreeNode
        const std::vector<Derived*>& children() const {
                return m_children;
        }

        /// Определяет, является ли node дочерним элементом
        bool isChildNode(Derived* node) const {
                for (Derived* item: m_children) if (item == node) return true;
                return false;
        }

protected:
        /// Выполняется при присоединении данной ноды
        virtual void onAttach() { }
        /// Выполняется при отсоединении данной ноды
        virtual void onDetach() { }
        /// Выполняется при присоединении дочерней ноды
        virtual void onAttachChild(Derived* node) { }
        /// Выполняется при отсоединении дочерней ноды
        virtual void onDetachChild(Derived* node) { }

private:
        bool addNodeToChildrenList(Derived* node) {
                if (isChildNode(node)) {
                        IM_ERROR("Node already added");
                        return false;
                } else {
                        m_children.push_back(node);
                        onAttachChild(node);
                        node->onAttach();
                        return true;
                }
        }

        bool removeNodeFromChildrenList(Derived* node) {
                for (auto it = m_children.begin(); it != m_children.end(); ++it) {
                        if (*it == node) {
                                m_children.erase(it);
                                onDetachChild(node);
                                node->onDetach();
                                return true;
                        }
                }
                return false;
        }

        void deleteAllChildren() {
                // Копируем указатели на детей, так как при удалении массив m_children может измениться из
                // дочерних объектов, а нельзя удалять и добавлять элементы в контейнер во время итерации
                auto children = m_children;
                for (Derived* node: children) delete node;
                m_children.clear();

        }

protected:
        Derived*                m_parent;
        std::vector<Derived*>   m_children;
};


} //namespace imEngine

#endif // TREENODE_H
