#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {
    }
};

class BST {
public:
    TreeNode *root;

    BST() : root(nullptr) {
    }

    TreeNode *insert(TreeNode *node, int val) {
        if (node == nullptr) {
            return new TreeNode(val);
        }
        if (val < node->val) {
            node->left = insert(node->left, val);
        } else if (val > node->val) {
            node->right = insert(node->right, val);
        }
        //若相等则忽略重复
        return node;
    }

    void insert(int val) {
        root = insert(root, val);
    }

    bool find(TreeNode *node, int val) {
        if (node == nullptr) {
            return false;
        }
        if (val == node->val) {
            return true;
        }
        if (val < node->val) {
            return find(node->left, val);
        }
        return find(node->right, val);
    }

    bool find(int val) {
        return find(root, val);
    }

    //辅助函数，用于删除操作中，找到子树中最小值
    TreeNode *findMin(TreeNode *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode *remove(TreeNode *node, int val) {
        if (node == nullptr) {
            return nullptr;
        }

        if (val < node->val) {
            node->left = remove(node->left, val);
        } else if (val > node->val) {
            node->right = remove(node->right, val);
        } else {
            //找到了

            //情况1：没有孩子或只有一个孩子
            if (node->left == nullptr) {
                TreeNode *temp = node->right;
                delete node;
                return temp; //若右孩子也为空则返回空
            } else if (node->right == nullptr) {
                TreeNode *temp = node->left;
                delete node;
                return temp;
            }

            //情况3：有两个孩子
            //策略：找右子树的最小值
            TreeNode *temp = findMin(node->right);
            node->val = temp->val;
            node->right = remove(node->right, node->val);
        }
        return node;
    }

    void remove(int val) {
        root = remove(root, val);
    }

    void inorder(TreeNode *node) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left);
        cout << node->val << " ";
        inorder(node->right);
    }

    void printTree() {
        inorder(root);
        cout << endl;
    }
};

int main() {
    BST bst;

    // 演示插入
    cout << "插入: 10, 5, 15, 3, 7, 12, 18" << endl;
    int inputs[] = {10, 5, 15, 3, 7, 12, 18};
    for (int x: inputs) bst.insert(x);

    cout << "当前树 (中序遍历): ";
    bst.printTree(); // 应输出: 3 5 7 10 12 15 18

    // 演示查找
    cout << "查找 7: " << (bst.find(7) ? "找到" : "未找到") << endl;
    cout << "查找 99: " << (bst.find(99) ? "找到" : "未找到") << endl;

    // 演示删除 - 情况 1 (叶子)
    cout << "删除 3 (叶子节点)..." << endl;
    bst.remove(3);
    bst.printTree();

    // 演示删除 - 情况 2 (一个孩子)
    // 先构造一个单孩子情况
    bst.insert(4); // 4 会在 5 -> 7(已删?) -> ... 逻辑中找到位置
    // 刚才删了3，5的左边空了。插入4会变成5的左孩子。

    // 演示删除 - 情况 3 (两个孩子)
    cout << "删除 10 (根节点，有两个孩子)..." << endl;
    // 10 被删除，右子树最小的 12 应该上位
    bst.remove(10);
    bst.printTree(); // 根节点应该变成了 12

    return 0;
}
