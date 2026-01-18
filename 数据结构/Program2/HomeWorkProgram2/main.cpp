#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <stdexcept>
#include <cctype>
#include <memory>
using namespace std;
// 符号集定义
const string SYMBOLS = "abcd"; // ∑ = {a, b, c, d}
const char EPSILON = 'E'; // 表示空串

// 节点类型枚举
enum NodeType {
    SYMBOL,     // 符号节点（叶子）-> 0
    CONCAT,     // 连接操作（-）-> 1
    UNION,      // 并集操作（|）-> 2
    KLEENE      // Kleene闭包（*）-> 3
};

// 正则表达式树节点类
class RegexNode {
public:
    NodeType type;
    char value; // 对于符号节点，存储字符；对于操作符节点，忽略或用于调试
    unique_ptr<RegexNode> left;
    unique_ptr<RegexNode> right;

    // 构造函数
    RegexNode(NodeType t, char v = '\0') : type(t), value(v) {}

    // 不需要显式析构函数，unique_ptr会自动管理内存
};

// 正则表达式树类
class RegexTree {
private:
    unique_ptr<RegexNode> root;

    // 辅助函数：验证输入字符串是否有效
    bool isValidRegex(const string& regex) {
        stack<char> parenStack;
        for (size_t i = 0; i < regex.length(); i++) {
            char c = regex[i];
            if (c == '(') {
                parenStack.push(c);
            } else if (c == ')') {
                if (parenStack.empty()) {
                    return false; // 不匹配的右括号
                }
                parenStack.pop();
            } else if (c != '|' && c != '*' && c != EPSILON) {
                if (SYMBOLS.find(c) == string::npos) {
                    return false; // 非法字符
                }
            }
        }
        return parenStack.empty(); // 括号必须匹配
    }

    // 辅助函数：将正则表达式字符串转换为后缀表达式（使用调度场算法）
    string toPostfix(const string& regex) {
        string output;
        stack<char> opStack;
        // 优先级映射：数字越高优先级越高
        auto precedence = [](char op) {
            if (op == '|') return 1;
            if (op == '-') return 2;
            if (op == '*') return 3;
            return 0;
        };

        // 遍历输入，处理隐式连接
        string processedRegex;
        for (size_t i = 0; i < regex.length(); i++) {
            char c = regex[i];
            processedRegex += c;
            // 检查是否需要插入连接符：当前字符是符号、*或)，且下一个字符是符号或(
            if (i + 1 < regex.length()) {
                char next = regex[i + 1];
                if ((c != '|' && c != '(' && next != '|' && next != '*' && next != ')')) {
                    processedRegex += '-'; // 插入隐式连接符
                }
            }
        }

        // 调度场算法处理 processedRegex
        for (char c : processedRegex) {
            if (c == ' ' || c == '\t') continue; // 跳过空格
            if (SYMBOLS.find(c) != std::string::npos || c == EPSILON) {
                output += c;
            } else if (c == '(') {
                opStack.push(c);
            } else if (c == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    output += opStack.top();
                    opStack.pop();
                }
                if (!opStack.empty() && opStack.top() == '(') {
                    opStack.pop();
                } else {
                    throw invalid_argument("Mismatched parentheses");
                }
            } else { // 操作符：|, -, *
                while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) {
                    output += opStack.top();
                    opStack.pop();
                }
                opStack.push(c);
            }
        }

        while (!opStack.empty()) {
            if (opStack.top() == '(') {
                throw invalid_argument("Mismatched parentheses");
            }
            output += opStack.top();
            opStack.pop();
        }

        return output;
    }

    // 从后缀表达式构建树
    unique_ptr<RegexNode> buildTreeFromPostfix(const string& postfix) {
        std::stack<std::unique_ptr<RegexNode>> nodeStack;
        for (char c : postfix) {
            if (SYMBOLS.find(c) != string::npos || c == EPSILON) {
                nodeStack.push(make_unique<RegexNode>(SYMBOL, c));
            } else if (c == '|') {
                if (nodeStack.size() < 2) throw invalid_argument("Invalid expression for union");
                auto right = move(nodeStack.top()); nodeStack.pop();
                auto left = move(nodeStack.top()); nodeStack.pop();
                auto opNode = make_unique<RegexNode>(UNION);
                opNode->left = move(left);
                opNode->right = move(right);
                nodeStack.push(move(opNode));
            } else if (c == '-') { // 连接操作
                if (nodeStack.size() < 2) throw invalid_argument("Invalid expression for concatenation");
                auto right = move(nodeStack.top()); nodeStack.pop();
                auto left = move(nodeStack.top()); nodeStack.pop();
                auto opNode = make_unique<RegexNode>(CONCAT);
                opNode->left = move(left);
                opNode->right = move(right);
                nodeStack.push(move(opNode));
            } else if (c == '*') {
                if (nodeStack.empty()) throw invalid_argument("Invalid expression for Kleene star");
                auto child = move(nodeStack.top()); nodeStack.pop();
                auto opNode = make_unique<RegexNode>(KLEENE);
                opNode->left = move(child); // 闭包是一元操作，只使用左子节点
                nodeStack.push(move(opNode));
            }
        }
        if (nodeStack.size() != 1) throw invalid_argument("Invalid expression");
        return move(nodeStack.top());
    }

    // 递归打印树结构（深度优先遍历）
    void printTreeDFS(const RegexNode* node, const string& indent = "") {
        if (!node) return;

        // 打印当前节点
        if (node->type == SYMBOL) {
            cout << indent << "(" << node->value << ")->0" << endl;
        } else if (node->type == CONCAT) {
            cout << indent << "(-)->1" << endl;
        } else if (node->type == UNION) {
            cout << indent << "(|)->2" << endl;
        } else if (node->type == KLEENE) {
            cout << indent << "(*)->3" << endl;
        }

        // 递归打印子节点，增加缩进
        string childIndent = indent + "           ";
        if (node->left) printTreeDFS(node->left.get(), childIndent);
        if (node->right) printTreeDFS(node->right.get(), childIndent);
    }

public:
    // 构造函数：从正则表达式字符串构建树
    RegexTree(const std::string& regex) {
        if (!isValidRegex(regex)) {
            throw std::invalid_argument("Invalid regular expression");
        }
        std::string postfix = toPostfix(regex);
        root = buildTreeFromPostfix(postfix);
    }

    // 打印正则表达式（中序遍历）
    void printExpression() {
        printInOrder(root.get());
        cout << std::endl;
    }

    // 中序遍历辅助函数
    void printInOrder(const RegexNode* node) {
        if (!node) return;

        // 处理左子树
        bool leftParen = false;
        if (node->left && (node->type == UNION || node->type == CONCAT)) {
            cout << "(";
            leftParen = true;
        }
        printInOrder(node->left.get());

        // 打印当前节点
        if (node->type == SYMBOL) {
            cout << node->value;
        } else if (node->type == UNION) {
            cout << "|";
        } else if (node->type == KLEENE) {
            cout << "*";
        }

        // 处理右子树
        printInOrder(node->right.get());
        if (leftParen) {
            std::cout << ")";
        }
    }

    // 打印树形结构（深度优先遍历）
    void printTree() {
        if (!root) return;
        printTreeDFS(root.get());
    }
};

// 单元测试函数
void testRegexTree() {
    // 测试用例1：有效正则表达式 c(a|b)*
    try {
        cout << "测试1: c(a|b)*" << endl;
        RegexTree tree("c(a|b)*");
        cout << "表达式: ";
        tree.printExpression();
        cout << "树结构:" << std::endl;
        tree.printTree();
    } catch (const std::exception& e) {
        cout << "错误: " << e.what() << endl;
    }

    // 测试用例2：有效正则表达式 a|b|c
    try {
        cout << "\n测试2: a|b|c" << endl;
        RegexTree tree("a|b|c");
        cout << "表达式: ";
        tree.printExpression();
        cout << "树结构:" << endl;
        tree.printTree();
    } catch (const std::exception& e) {
        cout << "错误: " << e.what() << endl;
    }

    // 测试用例3：有效正则表达式 (ab)*c
    try {
        cout << "\n测试3: (ab)*c" << endl;
        RegexTree tree("(ab)*c");
        cout << "表达式: ";
        tree.printExpression();cout << "树结构:" << endl;
        tree.printTree();
    } catch (const std::exception& e) {
        cout << "错误: " << e.what() << endl;
    }

    // 测试用例4：无效正则表达式（非法字符）
    try {
        cout << "\n测试4: c(x|b)" << endl;
        RegexTree tree("c(x|b)");
    } catch (const std::exception& e) {
        cout << "预期错误: " << e.what() << endl;
    }

    // 测试用例5：括号不匹配
    try {
        cout << "\n测试5: c(a|b" << endl;
        RegexTree tree("c(a|b");
    } catch (const std::exception& e) {
        cout << "预期错误: " << e.what() <<endl;
    }
}

int main() {
    testRegexTree();
    return 0;
}
/*
  正则表达式：c(a|b)*
  (-)->1
           (c)->0
           (*)->3
  (c)->0
  (*)->3
           (|)->2
  (|)->2
           (a)->0
           (b)->0
  (a)->0
  (b)->0
 */
