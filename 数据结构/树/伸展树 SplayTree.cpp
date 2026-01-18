#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;

struct SplayNode {
    int val;
    SplayNode *left;
    SplayNode *right;

    SplayNode(int x) : val(x), left(nullptr), right(nullptr) {
    }
};

class SplayTree {
public:
    SplayNode *root;

    SplayTree(): root(nullptr) {
    }

    //右旋（Zag）
    SplayNode *rightRotate(SplayNode *x) {
        SplayNode *y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    //左旋（Zig）
    SplayNode *leftRotate(SplayNode *x) {
        SplayNode *y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    //核心：Splay操作
    SplayNode *splay(SplayNode *root, int val) {
        if (root == nullptr || root->val == val) {
            return root;
        }

        if (val < root->val) {
            //val在左子树
            if (root->left == nullptr) {
                return root; //没找到，把最后访问的节点旋上来
            }

            //情况1：左左（Zig-Zig）
            if (val < root->left->val) {
                root->left->left = splay(root->left->left, val);
                //核心：先转爷爷再转爸爸
                root = rightRotate(root); //第一次右旋
                if (root->left == nullptr) {
                    return root;
                }
                return rightRotate(root); //第二次右旋
            }

            //情况2：左右（Zig-Zag）
            else if (val > root->left->val) {
                root->left->right = splay(root->left->right, val);
                if (root->left->right != nullptr) {
                    root->left = leftRotate(root->left); //先左旋爸爸
                }
                return rightRotate(root); //再右旋爷爷
            }

            //单左孩子（Zig）
            else {
                return rightRotate(root);
            }
        } else {
            //val在右子树
            if (root->right == nullptr) {
                return root;
            }

            //情况3：右右（Zig-Zig）
            if (val > root->right->val) {
                root->right->right = splay(root->right->right, val);
                root = leftRotate(root);
                if (root->right == nullptr) {
                    return root;
                }
                return leftRotate(root);
            }

            //情况4：右左（Zig-Zag）
            else if (val < root->right->val) {
                root->right->left = splay(root->right->left, val);
                if (root->right->left != nullptr) {
                    root->right = rightRotate(root->right);
                }
                return leftRotate(root);
            }

            //单右孩子
            else {
                return leftRotate(root);
            }
        }
    }

    //查找并Splay
    bool search(int val) {
        root = splay(root, val);
        return root != nullptr && root->val == val;
    }

    //插入并Splay
    void insert(int val) {
        if (root == nullptr) {
            root = new SplayNode(val);
            return;
        }

        //先把接近val的节点旋上来
        root = splay(root, val);

        if (root->val == val) {
            return;
        }

        SplayNode *newNode = new SplayNode(val);

        if (val < root->val) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        } else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }
        root = newNode;
    }

    void preOrder(SplayNode *root) {
        if (root != nullptr) {
            cout << root->val << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }
};

int main() {
    SplayTree st;

    // 插入测试
    st.insert(100);
    st.insert(50);
    st.insert(200);
    st.insert(40);
    // 此时插入顺序为 100, 50, 200, 40
    // 每次插入，新节点都会变成根

    cout << "当前根节点 (应为 40): " << st.root->val << endl;

    // 查找测试 (Splay 效果)
    cout << "查找 100..." << endl;
    st.search(100);
    // 100 曾经在很下面，现在应该被旋到根部
    cout << "当前根节点 (应为 100): " << st.root->val << endl;

    cout << "先序遍历结构: ";
    st.preOrder(st.root);
    cout << endl;
}