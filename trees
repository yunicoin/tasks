#include <iostream>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int* DownWay(TreeNode* root, int M) {
    int l = 0;
    int r = 0;
    int s = 1;
    int* ret = new int[2];

    
    ret[0] = 0;  
    ret[1] = M;    
    if (!root) {
        return ret;
    }

    
    if (root->left) {
        int* left_result = DownWay(root->left, M);
        l = left_result[0];
        M = std::max(M, left_result[1]);
        // Если значение не совпадает, путь прерывается
        if (root->left->val != root->val) {
            l = 0;
        }
        delete[] left_result;  
    }

    
    if (root->right) {
        int* right_result = DownWay(root->right, M);
        r = right_result[0];
        M = std::max(M, right_result[1]);
        // Если значение не совпадает, путь прерывается
        if (root->right->val != root->val) {
            r = 0;
        }
        delete[] right_result;  
    }

    s = std::max(l, r) + 1;  
    ret[0] = s;
    ret[1] = std::max(r + l + 1, M);  

    return ret;
}

void read(TreeNode* root) {
    int left_val, right_val;
    std::cin >> left_val >> right_val;

    
    if (left_val != 0) {
        root->left = new TreeNode(left_val);
        read(root->left);
    }

    
    if (right_val != 0) {
        root->right = new TreeNode(right_val);
        read(root->right);
    }
}

int Process(TreeNode* root) {
    int* result = DownWay(root, 0);
    int diameter = result[1];
    delete[] result;  
    return diameter;
}


void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    int c;
    std::cin >> c;
    TreeNode* root = new TreeNode(c);
    read(root);

    std::cout << Process(root) << std::endl;

    deleteTree(root);  
    return 0;
}
