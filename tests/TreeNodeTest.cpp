#include <imEngine/Utils/TreeNamedNode.h>
#include <imEngine/Utils/Types.h>
#include <imEngine/Utils/Debug.h>
#include <memory>

using namespace imEngine;

class Derived : public TreeNamedNode<Derived> {
public:
        Derived(const String& name, Derived* parent = 0) : TreeNamedNode(name, parent) {}
        ~Derived() { IM_TRACE(m_name); }

        void render(int offset = 0) const {
                for (int i = 0; i < offset; ++i) std::cout << "   ";
                std::cout << name() << std::endl;

                for (Derived* node: children()) node->render(offset+1);
        }
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

        Derived* ptr = root->find("share", true);
        if (ptr) {
                IM_TRACE("FOUND");
                delete ptr;
        }
        root->render();

        return 0;
}
