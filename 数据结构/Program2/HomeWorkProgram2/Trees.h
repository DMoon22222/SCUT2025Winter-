//
// Created by 15614 on 2025/10/14.
//

#ifndef HOMEWORKPROGRAM2_TREES_H
#define HOMEWORKPROGRAM2_TREES_H
#include <string>
#include <stack>
#include <queue>
#include <iostream>
using namespace std;

// 操作符类型枚举
enum OpType {
    CONCAT,    // 连接操作
    UNION,     // 并集操作 (|)
    KLEENE     // Kleene闭包 (*)
};

// 树节点类
class TreeNode {
public:
    char value;       // 节点值（符号：a/b/c/d/ε；操作符：|/*/-）
    OpType opType;    // 操作符类型（非叶子节点有效）
    TreeNode* left;   // 左子节点
    TreeNode* right;  // 右子节点

    // 构造函数
    TreeNode(char val, OpType type = CONCAT)
            : value(val), opType(type), left(nullptr), right(nullptr) {}

    // 析构函数（递归释放子节点）
    ~TreeNode() {
        delete left;
        delete right;
    }
};

// 正则表达式树核心类
class RegexTree {
private:
    TreeNode* root;       // 树根节点
    string originalRegex; // 原始正则表达式字符串
    const string SYMBOLS = "abcdε";  // 合法符号集

    // 检查字符是否为合法符号
    bool isSymbol(char c) const {
        return SYMBOLS.find(c) != string::npos;
    }

    // 检查括号是否匹配（返回true表示匹配）
    bool checkBracketMatch(const string& s) const {
        stack<char> bracketStack;
        for (char c : s) {
            if (c == '(') {
                bracketStack.push(c);
            } else if (c == ')') {
                if (bracketStack.empty() || bracketStack.top() != '(') {
                    return false;
                }
                bracketStack.pop();
            }
        }
        return bracketStack.empty();
    }

    // 移除最外层括号（如果存在且合法）
    string removeOuterBrackets(const string& s) const {
        if (s.size() < 2) return s;
        if (s[0] == '(' && s.back() == ')') {
            // 检查内层括号是否匹配（确保外层括号是整体包裹）
            string inner = s.substr(1, s.size() - 2);
            if (checkBracketMatch(inner)) {
                return inner;
            }
        }
        return s;
    }

    // 查找最低优先级的操作符位置（用于分割表达式）
    // 优先级：() > * > 连接 > |
    int findLowestPriorityOp(const string& s) const {
        int bracketDepth = 0;  // 括号深度（0表示不在括号内）
        int unionPos = -1;     // 并集操作符(|)位置
        int concatPos = -1;    // 连接操作符位置

        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (c == '(') {
                bracketDepth++;
            } else if (c == ')') {
                bracketDepth--;
            } else if (bracketDepth == 0) {  // 只处理括号外的操作符
                if (c == '|') {
                    unionPos = i;  // |优先级最低，最后处理
                } else if (c != '*' &&  // 排除*（优先级高于连接）
                           // 连接操作是隐式的（如ab表示a连接b），这里记录连接点
                           (i > 0 && (isSymbol(s[i-1]) || s[i-1] == '*' || s[i-1] == ')')) &&
                           (isSymbol(c) || c == '(')) {
                    concatPos = i - 1;  // 连接点记为前一个字符位置
                }
            }
        }

        // 优先返回|的位置（优先级最低），否则返回连接操作位置
        return (unionPos != -1) ? unionPos : concatPos;
    }

    // 递归构建正则表达式树
    TreeNode* build(const string& s) const {
        string expr = removeOuterBrackets(s);
        if (expr.empty()) {
            return new TreeNode('#');  // 空表达式对应ε(#)
        }

        // 处理Kleene闭包（*优先级最高，优先处理结尾的*）
        if (expr.back() == '*') {
            string subExpr = expr.substr(0, expr.size() - 1);
            TreeNode* child = build(subExpr);
            if (!child) return nullptr;
            TreeNode* node = new TreeNode('*', KLEENE);
            node->left = child;  // *是单目操作符，只需要左子树
            return node;
        }

        // 查找最低优先级操作符，分割为左右子表达式
        int opPos = findLowestPriorityOp(expr);
        if (opPos == -1) {
            // 没有操作符，说明是单个符号
            if (expr.size() == 1 && isSymbol(expr[0])) {
                return new TreeNode(expr[0]);
            } else {
                cerr << "错误：无效的子表达式 '" << expr << "'" << endl;
                return nullptr;
            }
        }

        // 处理并集操作(|)
        if (expr[opPos] == '|') {
            string leftExpr = expr.substr(0, opPos);
            string rightExpr = expr.substr(opPos + 1);
            TreeNode* leftChild = build(leftExpr);
            TreeNode* rightChild = build(rightExpr);
            if (!leftChild || !rightChild) return nullptr;
            TreeNode* node = new TreeNode('|', UNION);
            node->left = leftChild;
            node->right = rightChild;
            return node;
        }

        // 处理连接操作（隐式，用'-'表示）
        string leftExpr = expr.substr(0, opPos + 1);
        string rightExpr = expr.substr(opPos + 1);
        TreeNode* leftChild = build(leftExpr);
        TreeNode* rightChild = build(rightExpr);
        if (!leftChild || !rightChild) return nullptr;
        TreeNode* node = new TreeNode('-', CONCAT);  // '-'表示连接
        node->left = leftChild;
        node->right = rightChild;
        return node;
    }

    // 中序遍历树，生成正则表达式字符串
    void inorderTraverse(TreeNode* node, string& result) const {
        if (!node) return;

        // 处理Kleene闭包（单目操作符）
        if (node->opType == KLEENE) {
            // 子节点需要括号包裹（如果子节点是复合表达式）
            bool needBracket = (node->left->opType == UNION || node->left->opType == CONCAT);
            if (needBracket) result += "(";
            inorderTraverse(node->left, result);
            if (needBracket) result += ")";
            result += "*";  // 追加*
            return;
        }

        // 递归处理左子树
        bool leftNeedBracket = (node->left->opType != node->opType) &&
                               (node->left->opType == UNION || node->left->opType == CONCAT);
        if (leftNeedBracket) result += "(";
        inorderTraverse(node->left, result);
        if (leftNeedBracket) result += ")";

        // 处理当前节点（连接操作不显示'-'）
        if (node->opType != CONCAT) {
            result += node->value;  // 并集操作显示'|'
        }

        // 递归处理右子树
        bool rightNeedBracket = (node->right->opType != node->opType) &&
                                (node->right->opType == UNION || node->right->opType == CONCAT);
        if (rightNeedBracket) result += "(";
        inorderTraverse(node->right, result);
        if (rightNeedBracket) result += ")";
    }

    // 分层遍历打印树结构
    void levelOrderPrint(TreeNode* root) const {
        if (!root) {
            cout << "树为空" << endl;
            return;
        }

        queue<TreeNode*> q;
        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int levelSize = q.size();  // 当前层的节点数
            cout << "第" << level << "层: ";

            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();

                // 打印节点信息（操作符或符号）
                if (node->opType == CONCAT) {
                    cout << "[连接] ";
                } else if (node->opType == UNION) {
                    cout << "[|] ";
                } else if (node->opType == KLEENE) {
                    cout << "[*] ";
                } else {
                    cout << "[" << node->value << "] ";  // 符号节点
                }

                // 子节点入队
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }

            cout << endl;
            level++;
        }
    }

public:
    // 构造函数：输入正则表达式，验证并构建树
    RegexTree(const string& regex) : originalRegex(regex), root(nullptr) {
        if (isValid()) {
            root = build(regex);
        } else {
            cerr << "正则表达式无效，无法构建树" << endl;
        }
    }

    // 析构函数
    ~RegexTree() {
        delete root;
    }

    // 验证正则表达式是否合法
    bool isValid() const {
        // 检查括号匹配
        if (!checkBracketMatch(originalRegex)) {
            cerr << "验证失败：括号不匹配" << endl;
            return false;
        }

        // 检查非法字符
        for (char c : originalRegex) {
            if (c != '(' && c != ')' && c != '|' && c != '*' && !isSymbol(c)) {
                cerr << "验证失败：存在非法字符 '" << c << "'（合法字符：a,b,c,d,ε,(,),|,*）" << endl;
                return false;
            }
        }

        return true;
    }

    // 打印正则表达式（从树还原）
    void printRegex() const {
        if (!root) return;
        string result;
        inorderTraverse(root, result);
        cout << "还原的正则表达式：" << result << endl;
    }

    // 打印树的层次结构
    void printTreeStructure() const {
        if (!root) return;
        cout << "树的层次结构：" << endl;
        levelOrderPrint(root);
    }

    // 获取根节点（仅用于测试）
    TreeNode* getRoot() const {
        return root;
    }
};
#endif //HOMEWORKPROGRAM2_TREES_H
