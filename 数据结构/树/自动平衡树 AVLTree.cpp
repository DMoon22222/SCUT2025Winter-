#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;

struct AvlNode {
    int val;
    AvlNode *left;
    AvlNode *right;
    int height;

    AvlNode(int x) : val(x), left(nullptr), right(nullptr), height(1) {
    }
};

class AVLTree {
public:
    AvlNode *root;

    AVLTree() : root(nullptr) {
    }

    //辅助函数：获取高度，处理空指针
    int height(AvlNode *node) {
        return (node == nullptr) ? 0 : node->height;
    }

    //辅助函数：获取平衡因子，处理空指针
    int getBalance(AvlNode *node) {
        return (node == nullptr) ? 0 : height(node->left) - height(node->right);
    }

    //辅助函数：更新高度
    void updateHeight(AvlNode *node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    //右旋--解决LL
    AvlNode *rightRotation(AvlNode *y) {
        AvlNode *x = y->left;
        AvlNode *T2 = x->right;

        //旋转
        x->right = y;
        y->left = T2;

        //更新高度
        updateHeight(y);
        updateHeight(x);

        //更新根
        return x;
    }

    //左旋--解决RR
    AvlNode *leftRotation(AvlNode *x) {
        AvlNode *y = x->right;
        AvlNode *T2 = y->left;

        //旋转
        y->left = x;
        x->right = T2;

        //更新高度
        updateHeight(x);
        updateHeight(y);

        //更新根
        return y;
    }

    AvlNode *insert(AvlNode *node, int val) {
        //插入
        if (node == nullptr) {
            node = new AvlNode(val);
        }

        if (val < node->val) {
            node->left = insert(node->left, val);
        } else if (val > node->val) {
            node->right = insert(node->right, val);
        } else {
            return node; //相等，不允许重复值
        }

        //更新高度
        updateHeight(node);

        //检查平衡因子
        int balance = getBalance(node);

        if (balance > 1 && val < node->left->val) {
            //LL
            return rightRotation(node);
        }

        if (balance < -1 && val > node->right->val) {
            //RR
            return leftRotation(node);
        }

        if (balance > 1 && val > node->left->val) {
            //LR
            //策略：先对左孩子左旋，变为LL，再整体右旋
            node->left = leftRotation(node->left);
            return rightRotation(node);
        }

        if (balance < -1 && val < node->right->val) {
            //RL
            //策略：先对右孩子右旋，变为RR，再整体左旋
            node->right = rightRotation(node->right);
            return leftRotation(node);
        }
        return node;
    }

    void insert(int val) {
        root = insert(root, val);
    }

    void preOrder(AvlNode *root) {
        if (root != nullptr) {
            cout << root->val << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }
};

int main() {
    AVLTree avl;

    // 演示: 插入 3, 2, 1
    // 在普通 BST 中这会变成直线
    // 在 AVL 中，插入 1 后会触发 LL 旋转 (右旋)
    cout << "插入 3, 2, 1..." << endl;
    avl.insert(3);
    avl.insert(2);
    avl.insert(1);

    cout << "先序遍历 (根 左 右): ";
    avl.preOrder(avl.root);
    // 预期输出: 2 1 3 (根变成了 2，平衡了)
    cout << endl;

    // 演示: 插入 4, 5
    // 树变成 2,1,3,4,5 -> 3,4,5 造成不平衡 (RR) -> 左旋
    cout << "插入 4, 5..." << endl;
    avl.insert(4);
    avl.insert(5);

    cout << "先序遍历: ";
    avl.preOrder(avl.root);
    // 预期输出: 2 1 4 3 5 (4 被提拔上来平衡右子树)
    cout << endl;
}
