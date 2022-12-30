//
// Created by JKearnsl on 01.05.2022.
//
//
// Объект бинарного дерева поиска
//
//

#ifndef BTSEARCHER_TREE_H
#define BTSEARCHER_TREE_H

#include <string>
#include <vector>

struct node {
    int key{};
    struct node *left{};
    struct node *right{};
};

class Tree {
    public:
        node* getRoot();
        bool save(const std::string& filepath = "./three.bts");
        int load(const std::string& filepath);
        void clear();

        // Обходы
        std::vector<int> getInorderValues();
        std::vector<int> getPostorderValues();
        std::vector<int> getPreorderValues();

        // Основные методы
        bool insert(int value);
        bool remove(int value);
        bool search(int value);


    private:
        node* root;

        bool _search(node* _node, int value);
        node* _add_node(node* _node, int key);
        node* _deleteNode(node*& _node, int key);

        // Обходы дерева
        void _preorder(node *_node, std::vector<int> &result);
        void _postorder(node *_node, std::vector<int> &result);
        void _inorder(node *_node, std::vector<int> &result);

        // utils
        node* _minValueNode(node* node);
        bool is_empty_file(const std::string& file_path);
        bool is_file_not_exist(const std::string& file_path);

};


#endif //BTSEARCHER_TREE_H
