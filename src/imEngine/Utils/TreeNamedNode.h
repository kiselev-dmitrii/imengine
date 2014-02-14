#ifndef TREENAMEDNODE_H
#define TREENAMEDNODE_H

#include "TreeNode.h"

namespace imEngine {

/** @brief Именнованный узел дерева
 *
 * В данный момент наследуется от TreeNode, однако в случае с недостаточной
 * скоростью можно отказаться от наследования в пользу использования контейнера map;
 */
template <typename Derived>
class TreeNamedNode : public TreeNode<Derived> {
public:
        /// Конструктор
        TreeNamedNode(Derived* parent = nullptr, const String& name = "") :
                TreeNode<Derived>(parent)
        {
                if (name == "") m_name = generateName();
                else m_name = name;
        }

        /// Устанавилвает имя узла
        void setName(const String& name) {
                m_name = name;
        }

        /// Возвращает имя узла
        const String& name() const {
                return m_name;
        }

        /// Производит поиск узла по имени. В случае неудачи возвращает nullptr
        Derived* find(const String& name, bool isRecursiveSearch) {
                if (isRecursiveSearch) {
                        for (Derived* node: TreeNode<Derived>::m_children) {
                                if (node->m_name == name) {
                                        return node;
                                } else {
                                        Derived* result = node->find(name, true);
                                        if (result) return result;
                                }
                        }
                        return nullptr;

                } else {
                        for (Derived* node: TreeNode<Derived>::m_children) {
                                if (node->m_name == name) return node;
                        }
                        return nullptr;
                }
        }

        /// Показывает иерархию (дебажный метод)
        void dbgShowHierarchy(int offset = 0) {
                for (int i = 0; i < offset-1; ++i) std::cout << " | ";
                if (offset >= 1) std::cout << " |_";
                std::cout << m_name << std::endl;

                for (Derived* node: TreeNode<Derived>::m_children) node->dbgShowHierarchy(offset+1);
        }


private:
        /// Генерирует имя
        String generateName() const {
                static int counter = 0;
                ++counter;
                return "unnamed" + std::to_string(counter);
        }

protected:
        String  m_name;
};

} //namespace imEngine

#endif // TREENAMEDNODE_H
