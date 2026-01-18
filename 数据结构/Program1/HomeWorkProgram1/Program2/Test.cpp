#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <cassert>
using namespace std;

// 定义操作符类型
enum OpType {
    CONCAT,    // 连接
    UNION,     // 并集
    KLEENE     // Kleene 闭包
};

// 定义节点类型
class TreeNode {
public:
    char value;       // 节点值（符号或操作符）
    OpType opType;    // 操作符类型（仅非叶子节点有效）
    TreeNode* left;   // 左子节点
    TreeNode* right;  // 右子节点

    TreeNode(char val, OpType type = CONCAT) : value(val), opType(type), left(nullptr), right(nullptr) {}
    ~TreeNode() {
        delete left;
        delete right;
    }
};

// 正则表达式树类
class RegexTree {
private:
    TreeNode* root;
    string regex;
    const string SYMBOL_SET = "abcdε";  // 符号集

    // 检查字符是否在符号集中
    bool isSymbol(char c) {
        return SYMBOL_SET.find(c) != string::npos;
    }

    // 检查括号是否匹配
    bool isBracketMatched(const string& s) {
        stack<char> brackets;
        for (char c : s) {
            if (c == '(') {
                brackets.push(c);
            } else if (c == ')') {
                if (brackets.empty() || brackets.top() != '(') {
                    return false;
                }
                brackets.pop();
            }
        }
        return brackets.empty();
    }

    // 检查输入是否符合正则表达式规则
    bool isValidRegex(const string& s) {
        if (!isBracketMatched(s)) {
            cout << "错误：括号不匹配" << endl;
            return false;
        }
        for (char c : s) {
            if (c != '(' && c != ')' && c != '|' && c != '*' && !isSymbol(c)) {
                cout << "错误：存在非法字符 '" << c << "'" << endl;
                return false;
            }
        }
        return true;
    }

    // 去除括号
    string removeBrackets(const string& s) {
        if (s.empty()) return s;
        if (s[0] == '(' && s.back() == ')' && isBracketMatched(s.substr(1, s.length() - 2))) {
            return s.substr(1, s.length() - 2);
        }
        return s;
    }

    // 找到优先级最低的操作符位置（用于分割）
    int findLowestPrecedence(const string& s) {
        int bracketCount = 0;
        int unionPos = -1, concatPos = -1;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                bracketCount++;
            } else if (s[i] == ')') {
                bracketCount--;
            } else if (bracketCount == 0) {
                if (s[i] == '|') {
                    unionPos = i;
                } else if (s[i] != '*' && (i == 0 || s[i - 1] != '*') && (i == s.length() - 1 || s[i + 1] != '*')) {
                    concatPos = i;
                }
            }
        }
        if (unionPos != -1) return unionPos;
        return concatPos;
    }

    // 构建正则表达式树的递归函数
    TreeNode* buildTree(const string& s) {
        string str = removeBrackets(s);
        if (str.length() == 0) {
            return new TreeNode('#');
        }
        if (str.length() == 1) {
            if (isSymbol(str[0])) {
                return new TreeNode(str[0]);
            } else {
                cout << "错误：无效的单字符 '" << str[0] << "'" << endl;
                return nullptr;
            }
        }
        if (str.back() == '*') {
            TreeNode* child = buildTree(str.substr(0, str.length() - 1));
            TreeNode* node = new TreeNode('*', KLEENE);
            node->left = child;
            return node;
        }
        int pos = findLowestPrecedence(str);
        if (pos == -1) {
            cout << "错误：无法解析表达式 '" << str << "'" << endl;
            return nullptr;
        }
        if (str[pos] == '|') {
            TreeNode* left = buildTree(str.substr(0, pos));
            TreeNode* right = buildTree(str.substr(pos + 1));
            TreeNode* node = new TreeNode('|', UNION);
            node->left = left;
            node->right = right;
            return node;
        } else {
            TreeNode* left = buildTree(str.substr(0, pos + 1));
            TreeNode* right = buildTree(str.substr(pos + 1));
            TreeNode* node = new TreeNode('-', CONCAT);
            node->left = left;
            node->right = right;
            return node;
        }
    }

    // 中序遍历打印正则表达式
    void inorderPrint(TreeNode* node, string& result) {
        if (!node) return;
        if (node->opType == KLEENE) {
            // 处理Kleene闭包，需要判断子节点是否需要括号
            bool needBracket = (node->left->opType == UNION || node->left->opType == CONCAT);
            if (needBracket) result += "(";
            inorderPrint(node->left, result);
            if (needBracket) result += ")";
            result += "*";
            return;
        }
        // 处理左子树，判断是否需要括号
        bool leftNeedBracket = (node->left && (node->left->opType != node->opType) &&
                                (node->left->opType == UNION || node->left->opType == CONCAT));
        if (leftNeedBracket) result += "(";
        inorderPrint(node->left, result);
        if (leftNeedBracket) result += ")";
        // 处理当前节点
        if (node->opType != CONCAT) {
            result += node->value;
        }
        // 处理右子树，判断是否需要括号
        bool rightNeedBracket = (node->right && (node->right->opType != node->opType) &&
                                 (node->right->opType == UNION || node->right->opType == CONCAT));
        if (rightNeedBracket) result += "(";
        inorderPrint(node->right, result);
        if (rightNeedBracket) result += ")";
    }

    // 按照树形结构打印输出正则表达式树（分层遍历）
    void printTreeStructure(TreeNode* root) {
        if (!root) {
            cout << "树为空" << endl;
            return;
        }
        queue<TreeNode*> q;
        q.push(root);
        int level = 0;
        while (!q.empty()) {
            int size = q.size();
            cout << "层 " << level << ": ";
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                // 打印节点值，区分操作符和符号
                if (node->opType == CONCAT) {
                    cout << "- ";
                } else if (node->opType == UNION) {
                    cout << "| ";
                } else if (node->opType == KLEENE) {
                    cout << "* ";
                } else {
                    cout << node->value << " ";
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
    RegexTree(const string& s) : regex(s), root(nullptr) {
        if (isValidRegex(s)) {
            root = buildTree(s);
        }
    }

    ~RegexTree() {
        delete root;
    }

    // 打印正则表达式（基于树的中序遍历）
    void printRegex() {
        string result;
        inorderPrint(root, result);
        cout << "基于树打印的正则表达式: " << result << endl;
    }

    // 按照树形结构打印输出正则表达式树
    void printStructure() {
        cout << "按照树形结构打印的正则表达式树:" << endl;
        printTreeStructure(root);
    }
};

// 单元测试
void testValidRegex() {
    cout << "测试有效正则表达式：" << endl;
    string testCases[] = {"c(a|b)*", "a", "ab*c|d", "ε", "(a|b)*c"};
    for (string s : testCases) {
        cout << "输入: " << s << endl;
        RegexTree tree(s);
        tree.printRegex();
        tree.printStructure();
        cout << "------------------------" << endl;
    }
}

void testInvalidRegex() {
    cout << "测试无效正则表达式：" << endl;
    string testCases[] = {"(a|b", "a|b)", "a*b*c*d*e", "a|b|c|d|e"};
    for (string s : testCases) {
        cout << "输入: " << s << endl;
        RegexTree tree(s);
        // 无效正则表达式会在构造时输出错误信息，这里无需额外操作
        cout << "------------------------" << endl;
    }
}

int main() {
    testValidRegex();
    testInvalidRegex();

    string input;
    cout << "请输入正则表达式: ";
    cin >> input;
    RegexTree tree(input);
    tree.printRegex();
    tree.printStructure();

    return 0;
}
