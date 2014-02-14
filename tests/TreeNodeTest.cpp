#include <imEngine/Utils/TreeNode.h>
#include <imEngine/Utils/Types.h>
#include <imEngine/Utils/Debug.h>
#include <memory>

using namespace imEngine;

class Derived : public TreeNode<Derived> {
public:
        Derived(const String& name, Derived* parent = 0) : TreeNode(parent), m_name(name) {}
        ~Derived() { IM_TRACE(m_name); }

        const String& name() const { return m_name; }

        void render(int offset = 0) const {
                for (int i = 0; i < offset; ++i) std::cout << "   ";
                std::cout << name() << std::endl;

                for (TreeNode* node: children()) {
                        ((Derived*) node)->render(offset+1);
                }
        }

private:
        String          m_name;
};

int main() {
        std::unique_ptr<Derived> root(new Derived("root"));

        auto home = new Derived("home", root.get());
        auto sol = new Derived("sol", home);
        auto usr = new Derived("usr", root.get());
        auto bin = new Derived("bin", usr);
        auto share = new Derived("share", usr);

        IM_UNUSED(sol);
        IM_UNUSED(bin);
        IM_UNUSED(share);

        root->render();
        usr->setParent(sol);
        root->render();

        return 0;
}
